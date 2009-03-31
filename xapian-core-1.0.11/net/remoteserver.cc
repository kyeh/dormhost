/** @file remoteserver.cc
 *  @brief Xapian remote backend server base class
 */
/* Copyright (C) 2006,2007,2008 Olly Betts
 * Copyright (C) 2006,2007 Lemur Consulting Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <config.h>
#include "remoteserver.h"

#include <xapian/database.h>
#include <xapian/enquire.h>
#include <xapian/error.h>
#include <xapian/valueiterator.h>

#include "safeerrno.h"
#include <signal.h>
#include <stdlib.h>

#include "autoptr.h"
#include "multimatch.h"
#include "omassert.h"
#include "omtime.h"
#include "serialise.h"
#include "serialise-double.h"
#include "stats.h"
#include "utils.h"

/// Class to throw when we receive the connection closing message.
struct ConnectionClosed { };

RemoteServer::RemoteServer(const std::vector<std::string> &dbpaths,
			   int fdin_, int fdout_,
			   Xapian::timeout active_timeout_,
			   Xapian::timeout idle_timeout_,
			   bool writable)
    : RemoteConnection(fdin_, fdout_, ""),
      db(NULL), wdb(NULL),
      active_timeout(active_timeout_), idle_timeout(idle_timeout_)
{
    // Catch errors opening the database and propagate them to the client.
    try {
	if (writable) {
	    AssertEq(dbpaths.size(), 1); // Expecting exactly one database.
	    wdb = new Xapian::WritableDatabase(dbpaths[0], Xapian::DB_CREATE_OR_OPEN);
	    db = wdb;
	} else {
	    db = new Xapian::Database;
	    vector<std::string>::const_iterator i;
	    for (i = dbpaths.begin(); i != dbpaths.end(); ++i)
		db->add_database(Xapian::Database(*i));
	}

	// Build a better description than Database::get_description() gives.
	// FIXME: improve Database::get_description() and then just use that
	// instead.
	context = dbpaths[0];
	vector<std::string>::const_iterator i(dbpaths.begin());
	for (++i; i != dbpaths.end(); ++i) {
	    context += ' ';
	    context += *i;
	}
    } catch (const Xapian::Error &err) {
	// Propagate the exception to the client.
	send_message(REPLY_EXCEPTION, serialise_error(err));
	// And rethrow it so our caller can log it and close the connection.
	throw;
    }

#ifndef __WIN32__
    // It's simplest to just ignore SIGPIPE.  We'll still know if the
    // connection dies because we'll get EPIPE back from write().
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	throw Xapian::NetworkError("Couldn't set SIGPIPE to SIG_IGN", errno);
#endif

    // Send greeting message.
    string message;
    message += char(XAPIAN_REMOTE_PROTOCOL_MAJOR_VERSION);
    message += char(XAPIAN_REMOTE_PROTOCOL_MINOR_VERSION);
    message += encode_length(db->get_doccount());
    message += encode_length(db->get_lastdocid());
    message += (db->has_positions() ? '1' : '0');
    message += serialise_double(db->get_avlength());
    send_message(REPLY_GREETING, message);

    // Register weighting schemes.
    Xapian::Weight * weight;
    weight = new Xapian::BM25Weight();
    wtschemes[weight->name()] = weight;
    weight = new Xapian::BoolWeight();
    wtschemes[weight->name()] = weight;
    weight = new Xapian::TradWeight();
    wtschemes[weight->name()] = weight;
}

RemoteServer::~RemoteServer()
{
    delete db;
    // wdb is either NULL or equal to db, so we shouldn't delete it too!

    map<string, Xapian::Weight*>::const_iterator i;
    for (i = wtschemes.begin(); i != wtschemes.end(); ++i) {
	delete i->second;
    }
}

message_type
RemoteServer::get_message(Xapian::timeout timeout, string & result,
			  message_type required_type)
{
    unsigned int type;
    OmTime end_time;
    if (timeout)
	end_time = OmTime::now() + timeout;
    type = RemoteConnection::get_message(result, end_time);

    // Handle "shutdown connection" message here.
    if (type == MSG_SHUTDOWN) throw ConnectionClosed();
    if (type >= MSG_MAX) {
	string errmsg("Invalid message type ");
	errmsg += om_tostring(type);
	throw Xapian::NetworkError(errmsg);
    }
    if (required_type != MSG_MAX && type != unsigned(required_type)) {
	string errmsg("Expecting message type ");
	errmsg += om_tostring(required_type);
	errmsg += ", got ";
	errmsg += om_tostring(type);
	throw Xapian::NetworkError(errmsg);
    }
    return static_cast<message_type>(type);
}

void
RemoteServer::send_message(reply_type type, const string &message)
{
    OmTime end_time;
    if (active_timeout)
	end_time = OmTime::now() + active_timeout;
    unsigned char type_as_char = static_cast<unsigned char>(type);
    RemoteConnection::send_message(type_as_char, message, end_time);
}

typedef void (RemoteServer::* dispatch_func)(const string &);

void
RemoteServer::run()
{
    while (true) {
	try {
	    /* This list needs to be kept in the same order as the list of
	     * message types in "remoteprotocol.h". Note that messages at the
	     * end of the list in "remoteprotocol.h" can be omitted if they
	     * don't correspond to dispatch actions.
	     */
	    static const dispatch_func dispatch[] = {
		&RemoteServer::msg_allterms,
		&RemoteServer::msg_collfreq,
		&RemoteServer::msg_document,
		&RemoteServer::msg_termexists,
		&RemoteServer::msg_termfreq,
		&RemoteServer::msg_keepalive,
		&RemoteServer::msg_doclength,
		&RemoteServer::msg_query,
		&RemoteServer::msg_termlist,
		&RemoteServer::msg_positionlist,
		&RemoteServer::msg_postlist,
		&RemoteServer::msg_reopen,
		&RemoteServer::msg_update,
		&RemoteServer::msg_adddocument,
		&RemoteServer::msg_cancel,
		&RemoteServer::msg_deletedocument_pre_30_2,
		&RemoteServer::msg_deletedocumentterm,
		&RemoteServer::msg_flush,
		&RemoteServer::msg_replacedocument,
		&RemoteServer::msg_replacedocumentterm,
		NULL, // MSG_GETMSET - used during a conversation.
		NULL, // MSG_SHUTDOWN - handled by get_message().
		&RemoteServer::msg_deletedocument
	    };

	    string message;
	    size_t type = get_message(idle_timeout, message);
	    if (type >= sizeof(dispatch)/sizeof(dispatch[0]) ||
		dispatch[type] == NULL) {
		string errmsg("Unexpected message type ");
		errmsg += om_tostring(type);
		throw Xapian::InvalidArgumentError(errmsg);
	    }
	    (this->*(dispatch[type]))(message);
	} catch (const Xapian::NetworkTimeoutError & e) {
	    try {
		// We've had a timeout, so the client may not be listening, so
		// if we can't send the message right away, just exit and the
		// client will cope.
		RemoteConnection::send_message(REPLY_EXCEPTION, serialise_error(e), OmTime::now());
	    } catch (...) {
	    }
	    // And rethrow it so our caller can log it and close the
	    // connection.
	    throw;
	} catch (const Xapian::NetworkError) {
	    // All other network errors mean we are fatally confused and are
	    // unlikely to be able to communicate further across this
	    // connection.  So we don't try to propagate the error to the
	    // client, but instead just rethrow the exception so our caller can
	    // log it and close the connection.
	    throw;
	} catch (const Xapian::Error &e) {
	    // Propagate the exception to the client, then return to the main
	    // message handling loop.
	    send_message(REPLY_EXCEPTION, serialise_error(e));
	} catch (ConnectionClosed &) {
	    return;
	} catch (...) {
	    // Propagate an unknown exception to the client.
	    send_message(REPLY_EXCEPTION, "");
	    // And rethrow it so our caller can log it and close the
	    // connection.
	    throw;
	}
    }
}

void
RemoteServer::msg_allterms(const string &message)
{
    const char *p = message.data();
    const char *p_end = p + message.size();
    string prefix(p, p_end - p);

    const Xapian::TermIterator end = db->allterms_end(prefix);
    for (Xapian::TermIterator t = db->allterms_begin(prefix); t != end; ++t) {
	string item = encode_length(t.get_termfreq());
	item += *t;
	send_message(REPLY_ALLTERMS, item);
    }

    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_termlist(const string &message)
{
    const char *p = message.data();
    const char *p_end = p + message.size();
    Xapian::docid did = decode_length(&p, p_end, false);

    send_message(REPLY_DOCLENGTH, serialise_double(db->get_doclength(did)));
    const Xapian::TermIterator end = db->termlist_end(did);
    for (Xapian::TermIterator t = db->termlist_begin(did); t != end; ++t) {
	string item = encode_length(t.get_wdf());
	item += encode_length(t.get_termfreq());
	item += *t;
	send_message(REPLY_TERMLIST, item);
    }

    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_positionlist(const string &message)
{
    const char *p = message.data();
    const char *p_end = p + message.size();
    Xapian::docid did = decode_length(&p, p_end, false);
    string term(p, p_end - p);

    Xapian::termpos lastpos = static_cast<Xapian::termpos>(-1);
    const Xapian::PositionIterator end = db->positionlist_end(did, term);
    for (Xapian::PositionIterator i = db->positionlist_begin(did, term);
	 i != end; ++i) {
	Xapian::termpos pos = *i;
	send_message(REPLY_POSITIONLIST, encode_length(pos - lastpos - 1));
	lastpos = pos;
    }

    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_postlist(const string &message)
{
    const char *p = message.data();
    const char *p_end = p + message.size();
    string term(p, p_end - p);

    Xapian::doccount termfreq = db->get_termfreq(term);
    Xapian::termcount collfreq = db->get_collection_freq(term);
    send_message(REPLY_POSTLISTSTART, encode_length(termfreq) + encode_length(collfreq));

    Xapian::docid lastdocid = 0;
    const Xapian::PostingIterator end = db->postlist_end(term);
    for (Xapian::PostingIterator i = db->postlist_begin(term);
	 i != end; ++i) {

	Xapian::docid newdocid = *i;
	string reply = encode_length(newdocid - lastdocid - 1);
	reply += encode_length(i.get_wdf());
	// FIXME: get_doclength should always return an integer value, but
	// Xapian::doclength is a double.  We could improve the compression
	// here by casting to an int and serialising that instead, but it's
	// probably not worth doing since the plan is to stop storing the
	// document length in the posting lists anyway, at which point the
	// remote protocol should stop passing it since it will be more
	// expensive to do so.
	reply += serialise_double(i.get_doclength());

	send_message(REPLY_POSTLISTITEM, reply);
	lastdocid = newdocid;
    }

    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_reopen(const string & msg)
{
    db->reopen();
    msg_update(msg);
}

void
RemoteServer::msg_update(const string &)
{
    // reopen() doesn't do anything for a WritableDatabase, so there's
    // no harm in calling it unconditionally.
    db->reopen();

    string message = encode_length(db->get_doccount());
    message += encode_length(db->get_lastdocid());
    message += (db->has_positions() ? '1' : '0');
    message += serialise_double(db->get_avlength());
    send_message(REPLY_UPDATE, message);
}

void
RemoteServer::msg_query(const string &message_in)
{
    const char *p = message_in.c_str();
    const char *p_end = p + message_in.size();
    size_t len;

    // Unserialise the Query.
    len = decode_length(&p, p_end, true);
    AutoPtr<Xapian::Query::Internal> query(Xapian::Query::Internal::unserialise(string(p, len)));
    p += len;

    // Unserialise assorted Enquire settings.
    Xapian::termcount qlen = decode_length(&p, p_end, false);

    Xapian::valueno collapse_key = decode_length(&p, p_end, false);

    if (p_end - p < 4 || *p < '0' || *p > '2') {
	throw Xapian::NetworkError("bad message (docid_order)");
    }
    Xapian::Enquire::docid_order order;
    order = static_cast<Xapian::Enquire::docid_order>(*p++ - '0');

    Xapian::valueno sort_key = decode_length(&p, p_end, false);

    if (*p < '0' || *p > '3') {
	throw Xapian::NetworkError("bad message (sort_by)");
    }
    Xapian::Enquire::Internal::sort_setting sort_by;
    sort_by = static_cast<Xapian::Enquire::Internal::sort_setting>(*p++ - '0');

    if (*p < '0' || *p > '1') {
	throw Xapian::NetworkError("bad message (sort_value_forward)");
    }
    bool sort_value_forward(*p++ != '0');

    int percent_cutoff = *p++;
    if (percent_cutoff < 0 || percent_cutoff > 100) {
	throw Xapian::NetworkError("bad message (percent_cutoff)");
    }

    Xapian::weight weight_cutoff = unserialise_double(&p, p_end);
    if (weight_cutoff < 0) {
	throw Xapian::NetworkError("bad message (weight_cutoff)");
    }

    // Unserialise the Weight object.
    len = decode_length(&p, p_end, true);
    map<string, Xapian::Weight *>::const_iterator i;
    i = wtschemes.find(string(p, len));
    if (i == wtschemes.end()) {
	throw Xapian::InvalidArgumentError("Weighting scheme " + string(p, len) + " not registered");
    }
    p += len;

    len = decode_length(&p, p_end, true);
    AutoPtr<Xapian::Weight> wt(i->second->unserialise(string(p, len)));
    p += len;

    // Unserialise the RSet object.
    Xapian::RSet rset = unserialise_rset(string(p, p_end - p));

    Stats local_stats;
    MultiMatch match(*db, query.get(), qlen, &rset, collapse_key,
		     percent_cutoff, weight_cutoff, order,
		     sort_key, sort_by, sort_value_forward, NULL,
		     NULL, local_stats, wt.get());

    send_message(REPLY_STATS, serialise_stats(local_stats));

    string message;
#if 0 // Reinstate this when major protocol version increases to 31.
    get_message(active_timeout, message, MSG_GETMSET);
#else
    char type = get_message(active_timeout, message);
    if (rare(type != MSG_GETMSET)) {
	if (type != MSG_GETMSET_PRE_30_5 && type != MSG_GETMSET_PRE_30_3) {
	    string errmsg("Expecting message type ");
	    errmsg += om_tostring(MSG_GETMSET_PRE_30_3);
	    errmsg += " or ";
	    errmsg += om_tostring(MSG_GETMSET_PRE_30_5);
	    errmsg += " or ";
	    errmsg += om_tostring(MSG_GETMSET);
	    errmsg += ", got ";
	    errmsg += om_tostring(type);
	    throw Xapian::NetworkError(errmsg);
	}
    }
#endif
    p = message.c_str();
    p_end = p + message.size();

    Xapian::termcount first = decode_length(&p, p_end, false);
    Xapian::termcount maxitems = decode_length(&p, p_end, false);

    Xapian::termcount check_at_least = 0;
    if (type != MSG_GETMSET_PRE_30_3) {
	check_at_least = decode_length(&p, p_end, false);
    }

    message.erase(0, message.size() - (p_end - p));
    Stats total_stats(unserialise_stats(message));

    Xapian::MSet mset;
    match.get_mset(first, maxitems, check_at_least, mset, total_stats, 0, 0);

    if (type == MSG_GETMSET_PRE_30_3 || type == MSG_GETMSET_PRE_30_5) {
	send_message(REPLY_RESULTS_PRE_30_5, serialise_mset_pre_30_5(mset));
    } else {
	send_message(REPLY_RESULTS, serialise_mset(mset));
    }
}

void
RemoteServer::msg_document(const string &message)
{
    const char *p = message.data();
    const char *p_end = p + message.size();
    Xapian::docid did = decode_length(&p, p_end, false);

    Xapian::Document doc = db->get_document(did);

    send_message(REPLY_DOCDATA, doc.get_data());

    Xapian::ValueIterator i;
    for (i = doc.values_begin(); i != doc.values_end(); ++i) {
	string item = encode_length(i.get_valueno());
	item += *i;
	send_message(REPLY_VALUE, item);
    }
    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_keepalive(const string &)
{
    // Ensure *our* database stays alive, as it may contain remote databases!
    db->keep_alive();
    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_termexists(const string &term)
{
    send_message((db->term_exists(term) ? REPLY_TERMEXISTS : REPLY_TERMDOESNTEXIST), "");
}

void
RemoteServer::msg_collfreq(const string &term)
{
    send_message(REPLY_COLLFREQ, encode_length(db->get_collection_freq(term)));
}

void
RemoteServer::msg_termfreq(const string &term)
{
    send_message(REPLY_TERMFREQ, encode_length(db->get_termfreq(term)));
}

void
RemoteServer::msg_doclength(const string &message)
{
    const char *p = message.data();
    const char *p_end = p + message.size();
    Xapian::docid did = decode_length(&p, p_end, false);
    // FIXME: get_doclength should always return an integer, but
    // Xapian::doclength is a double...
    send_message(REPLY_DOCLENGTH, serialise_double(db->get_doclength(did)));
}

void
RemoteServer::msg_flush(const string &)
{
    if (!wdb)
	throw Xapian::InvalidOperationError("Server is read-only");

    wdb->flush();

    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_cancel(const string &)
{
    if (!wdb)
	throw Xapian::InvalidOperationError("Server is read-only");

    // We can't call cancel since that's an internal method, but this
    // has the same effect with minimal additional overhead.
    wdb->begin_transaction(false);
    wdb->cancel_transaction();
}

void
RemoteServer::msg_adddocument(const string & message)
{
    if (!wdb)
	throw Xapian::InvalidOperationError("Server is read-only");

    Xapian::docid did = wdb->add_document(unserialise_document(message));

    send_message(REPLY_ADDDOCUMENT, encode_length(did));
}

// FIXME: eliminate this method when we move to remote major 31.
void
RemoteServer::msg_deletedocument_pre_30_2(const string & message)
{
    if (!wdb)
	throw Xapian::InvalidOperationError("Server is read-only");

    const char *p = message.data();
    const char *p_end = p + message.size();
    Xapian::docid did = decode_length(&p, p_end, false);

    wdb->delete_document(did);
}

void
RemoteServer::msg_deletedocument(const string & message)
{
    msg_deletedocument_pre_30_2(message);

    send_message(REPLY_DONE, "");
}

void
RemoteServer::msg_deletedocumentterm(const string & message)
{
    if (!wdb)
	throw Xapian::InvalidOperationError("Server is read-only");

    wdb->delete_document(message);
}

void
RemoteServer::msg_replacedocument(const string & message)
{
    if (!wdb)
	throw Xapian::InvalidOperationError("Server is read-only");

    const char *p = message.data();
    const char *p_end = p + message.size();
    Xapian::docid did = decode_length(&p, p_end, false);

    wdb->replace_document(did, unserialise_document(string(p, p_end)));
}

void
RemoteServer::msg_replacedocumentterm(const string & message)
{
    if (!wdb)
	throw Xapian::InvalidOperationError("Server is read-only");

    const char *p = message.data();
    const char *p_end = p + message.size();
    size_t len = decode_length(&p, p_end, true);
    string unique_term(p, len);
    p += len;

    Xapian::docid did = wdb->replace_document(unique_term, unserialise_document(string(p, p_end)));

    send_message(REPLY_ADDDOCUMENT, encode_length(did));
}
