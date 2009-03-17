/* inmemory_database.cc
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2002,2003,2004,2005,2006,2007,2009 Olly Betts
 * Copyright 2006 Richard Boulton
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include <config.h>
#include <stdio.h>

#include "omdebug.h"

#include "emptypostlist.h"
#include "expandweight.h"
#include "inmemory_database.h"
#include "inmemory_document.h"
#include "inmemory_alltermslist.h"
#include "utils.h"

#include <string>
#include <vector>
#include <map>
#include <list>

#include <xapian/error.h>
#include <xapian/valueiterator.h>

using std::make_pair;

inline void
InMemoryTerm::add_posting(const InMemoryPosting & post)
{
    // Add document to right place in list
    vector<InMemoryPosting>::iterator p;
    p = lower_bound(docs.begin(), docs.end(),
		    post, InMemoryPostingLessThan());
    if (p == docs.end() || InMemoryPostingLessThan()(post, *p)) {
	docs.insert(p, post);
    } else if (!p->valid) {
	*p = post;
    } else {
	(*p).merge(post);
    }
}

inline void
InMemoryDoc::add_posting(const InMemoryTermEntry & post)
{
    // Add document to right place in list
    vector<InMemoryTermEntry>::iterator p;
    p = lower_bound(terms.begin(), terms.end(),
		    post, InMemoryTermEntryLessThan());
    if (p == terms.end() || InMemoryTermEntryLessThan()(post, *p)) {
	terms.insert(p, post);
    } else {
	(*p).merge(post);
    }
}

//////////////
// Postlist //
//////////////

InMemoryPostList::InMemoryPostList(Xapian::Internal::RefCntPtr<const InMemoryDatabase> db_,
				   const InMemoryTerm & term)
	: pos(term.docs.begin()),
	  end(term.docs.end()),
	  termfreq(term.term_freq),
	  started(false),
	  db(db_)
{
    // InMemoryPostLists cannot be empty
    Assert(pos != end);
    while (pos != end && !pos->valid) ++pos;
}

Xapian::doccount
InMemoryPostList::get_termfreq() const
{
    return termfreq;
}

Xapian::docid
InMemoryPostList::get_docid() const
{
    //DebugMsg(tname << ".get_docid()");
    Assert(started);
    Assert(!at_end());
    //DebugMsg(" = " << (*pos).did << endl);
    return (*pos).did;
}

PostList *
InMemoryPostList::next(Xapian::weight /*w_min*/)
{
    if (started) {
	Assert(!at_end());
	++pos;
	while (pos != end && !pos->valid) ++pos;
    } else {
	started = true;
    }
    return NULL;
}

PostList *
InMemoryPostList::skip_to(Xapian::docid did, Xapian::weight w_min)
{
    //DebugMsg(tname << ".skip_to(" << did << ")" << endl);
    // FIXME - see if we can make more efficient, perhaps using better
    // data structure.  Note, though, that a binary search of
    // the remaining list may NOT be a good idea (search time is then
    // O(log {length of list}), as opposed to O(distance we want to skip)
    // Since we will frequently only be skipping a short distance, this
    // could well be worse.
    started = true;
    Assert(!at_end());
    while (!at_end() && (*pos).did < did) {
	(void) next(w_min);
    }
    return NULL;
}

bool
InMemoryPostList::at_end() const
{
    return (pos == end);
}

string
InMemoryPostList::get_description() const
{
    return "InMemoryPostList" + om_tostring(termfreq);
}

Xapian::doclength
InMemoryPostList::get_doclength() const
{
    return db->get_doclength(get_docid());
}

PositionList *
InMemoryPostList::read_position_list()
{
    mypositions.set_data(pos->positions);
    return &mypositions;
}

PositionList *
InMemoryPostList::open_position_list() const
{
    return new InMemoryPositionList(pos->positions);
}

Xapian::termcount
InMemoryPostList::get_wdf() const
{
    return (*pos).wdf;
}

//////////////
// Termlist //
//////////////

InMemoryTermList::InMemoryTermList(Xapian::Internal::RefCntPtr<const InMemoryDatabase> db_,
				   Xapian::docid did_,
				   const InMemoryDoc & doc,
				   Xapian::termcount len)
	: pos(doc.terms.begin()), end(doc.terms.end()), terms(doc.terms.size()),
	  started(false), db(db_), did(did_), document_length(len)
{
    DEBUGLINE(DB, "InMemoryTermList::InMemoryTermList(): " <<
	          terms << " terms starting from " << pos->tname);
}

Xapian::termcount
InMemoryTermList::get_wdf() const
{
    Assert(started);
    Assert(!at_end());
    return (*pos).wdf;
}

Xapian::doccount
InMemoryTermList::get_termfreq() const
{
    Assert(started);
    Assert(!at_end());

    return db->get_termfreq((*pos).tname);
}

Xapian::termcount
InMemoryTermList::get_approx_size() const
{
    return terms;
}

void
InMemoryTermList::accumulate_stats(Xapian::Internal::ExpandStats & stats) const
{
    Assert(started);
    Assert(!at_end());
    stats.accumulate(InMemoryTermList::get_wdf(), document_length,
		     InMemoryTermList::get_termfreq(),
		     db->get_doccount());
}

string
InMemoryTermList::get_termname() const
{
    Assert(started);
    Assert(!at_end());
    return (*pos).tname;
}

TermList *
InMemoryTermList::next()
{
    if (started) {
	Assert(!at_end());
	pos++;
    } else {
	started = true;
    }
    return NULL;
}

bool
InMemoryTermList::at_end() const
{
    Assert(started);
    return (pos == end);
}

Xapian::termcount
InMemoryTermList::positionlist_count() const
{
    return db->positionlist_count(did, (*pos).tname);
}

Xapian::PositionIterator
InMemoryTermList::positionlist_begin() const
{
    return Xapian::PositionIterator(db->open_position_list(did, (*pos).tname));
}

/////////////////////////////
// InMemoryAllDocsPostList //
/////////////////////////////

InMemoryAllDocsPostList::InMemoryAllDocsPostList(Xapian::Internal::RefCntPtr<const InMemoryDatabase> db_)
	: did(0), db(db_)
{
}

Xapian::doccount
InMemoryAllDocsPostList::get_termfreq() const
{
    return db->totdocs;
}

Xapian::docid
InMemoryAllDocsPostList::get_docid() const
{
    Assert(did > 0);
    Assert(did <= db->termlists.size());
    Assert(db->termlists[did - 1].is_valid);
    return did;
}

Xapian::doclength
InMemoryAllDocsPostList::get_doclength() const
{
    return db->get_doclength(did);
}

Xapian::termcount
InMemoryAllDocsPostList::get_wdf() const
{
    return 1;
}

PositionList *
InMemoryAllDocsPostList::read_position_list()
{
    throw Xapian::UnimplementedError("Can't open position list for all docs iterator");
}

PositionList *
InMemoryAllDocsPostList::open_position_list() const
{
    throw Xapian::UnimplementedError("Can't open position list for all docs iterator");
}

PostList *
InMemoryAllDocsPostList::next(Xapian::weight /*w_min*/)
{
    Assert(!at_end());
    do {
       ++did;
    } while (did <= db->termlists.size() && !db->termlists[did - 1].is_valid);
    return NULL;
}

PostList *
InMemoryAllDocsPostList::skip_to(Xapian::docid did_, Xapian::weight /*w_min*/)
{
    Assert(!at_end());
    if (did <= did_) {
	did = did_;
	while (did <= db->termlists.size() && !db->termlists[did - 1].is_valid) {
	    ++did;
	}
    }
    return NULL;
}

bool
InMemoryAllDocsPostList::at_end() const
{
    return (did > db->termlists.size());
}

string
InMemoryAllDocsPostList::get_description() const
{
    return "InMemoryAllDocsPostList" + om_tostring(did);
}

///////////////////////////
// Actual database class //
///////////////////////////

InMemoryDatabase::InMemoryDatabase()
	: totdocs(0), totlen(0), positions_present(false)
{
    // Updates are applied immediately so we can't support transactions.
    transaction_state = TRANSACTION_UNIMPLEMENTED;
}

InMemoryDatabase::~InMemoryDatabase()
{
    dtor_called();
}

LeafPostList *
InMemoryDatabase::open_post_list(const string & tname) const
{
    if (tname.empty()) {
	if (termlists.empty())
	    return new EmptyPostList();
	Xapian::Internal::RefCntPtr<const InMemoryDatabase> ptrtothis(this);
	return new InMemoryAllDocsPostList(ptrtothis);
    }
    map<string, InMemoryTerm>::const_iterator i = postlists.find(tname);
    if (i == postlists.end() || i->second.term_freq == 0)
	return new EmptyPostList();

    Xapian::Internal::RefCntPtr<const InMemoryDatabase> ptrtothis(this);
    LeafPostList * pl = new InMemoryPostList(ptrtothis, i->second);
    Assert(!pl->at_end());
    return pl;
}

bool
InMemoryDatabase::doc_exists(Xapian::docid did) const
{
    return (did > 0 && did <= termlists.size() && termlists[did - 1].is_valid);
}

Xapian::doccount
InMemoryDatabase::get_termfreq(const string & tname) const
{
    map<string, InMemoryTerm>::const_iterator i = postlists.find(tname);
    if (i == postlists.end()) return 0;
    return i->second.term_freq;
}

Xapian::termcount
InMemoryDatabase::get_collection_freq(const string &tname) const
{
    map<string, InMemoryTerm>::const_iterator i = postlists.find(tname);
    if (i == postlists.end()) return 0;
    return i->second.collection_freq;
}

Xapian::doccount
InMemoryDatabase::get_doccount() const
{
    return totdocs;
}

Xapian::docid
InMemoryDatabase::get_lastdocid() const
{
    return termlists.size();
}

Xapian::doclength
InMemoryDatabase::get_avlength() const
{
    if (totdocs == 0) return 0;
    return Xapian::doclength(totlen) / totdocs;
}

Xapian::doclength
InMemoryDatabase::get_doclength(Xapian::docid did) const
{
    if (!doc_exists(did)) {
	throw Xapian::DocNotFoundError(string("Docid ") + om_tostring(did) +
				 string(" not found"));
    }
    return doclengths[did - 1];
}

TermList *
InMemoryDatabase::open_term_list(Xapian::docid did) const
{
    if (did == 0) throw Xapian::InvalidArgumentError("Docid 0 invalid");
    if (!doc_exists(did)) {
	// FIXME: the docid in this message will be local, not global
	throw Xapian::DocNotFoundError(string("Docid ") + om_tostring(did) +
				 string(" not found"));
    }
    return new InMemoryTermList(Xapian::Internal::RefCntPtr<const InMemoryDatabase>(this), did,
				termlists[did - 1], doclengths[did - 1]);
}

Xapian::Document::Internal *
InMemoryDatabase::open_document(Xapian::docid did, bool /*lazy*/) const
{
    // we're never lazy so ignore that flag
    if (did == 0) throw Xapian::InvalidArgumentError("Docid 0 invalid");
    if (!doc_exists(did)) {
	// FIXME: the docid in this message will be local, not global
	throw Xapian::DocNotFoundError(string("Docid ") + om_tostring(did) +
				 string(" not found"));
    }
    return new InMemoryDocument(this, did, doclists[did - 1],
				valuelists[did - 1]);
}

std::string
InMemoryDatabase::get_metadata(const std::string & key) const
{
    map<string, string>::const_iterator i = metadata.find(key);
    if (i == metadata.end())
	return string();
    return i->second;
}

void
InMemoryDatabase::set_metadata(const std::string & key,
			       const std::string & value)
{
    if (!value.empty()) {
	metadata[key] = value;
    } else {
	metadata.erase(key);
    }
}

Xapian::termcount
InMemoryDatabase::positionlist_count(Xapian::docid did,
				     const string & tname) const
{
    if (!doc_exists(did)) {
	return 0;
    }
    const InMemoryDoc &doc = termlists[did-1];

    vector<InMemoryTermEntry>::const_iterator i;
    for (i = doc.terms.begin(); i != doc.terms.end(); ++i) {
	if (i->tname == tname) {
	    return i->positions.size();
	}
    }
    return 0;
}

PositionList * 
InMemoryDatabase::open_position_list(Xapian::docid did,
				     const string & tname) const
{
    if (!doc_exists(did)) {
	throw Xapian::DocNotFoundError("Document id " + om_tostring(did) +
				 " doesn't exist in inmemory database");
    }
    const InMemoryDoc &doc = termlists[did-1];

    vector<InMemoryTermEntry>::const_iterator i;
    for (i = doc.terms.begin(); i != doc.terms.end(); ++i) {
	if (i->tname == tname) {
	    return new InMemoryPositionList(i->positions);
	}
    }
    throw Xapian::RangeError("No positionlist for term in document.");
}

void
InMemoryDatabase::add_values(Xapian::docid did,
			     const map<Xapian::valueno, string> &values_)
{
    if (did > valuelists.size()) {
	valuelists.resize(did);
    }
    valuelists[did-1] = values_;
}

// We implicitly flush each modification right away, so nothing to do here.
void
InMemoryDatabase::flush()
{
}

// We implicitly flush each modification right away, so nothing to do here.
void
InMemoryDatabase::cancel()
{
}

void
InMemoryDatabase::delete_document(Xapian::docid did)
{
    if (!doc_exists(did)) {
	throw Xapian::DocNotFoundError(string("Docid ") + om_tostring(did) +
				 string(" not found"));
    }
    termlists[did-1].is_valid = false;
    doclists[did-1] = "";
    valuelists[did-1].clear();
    totlen -= doclengths[did-1];
    doclengths[did-1] = 0;
    totdocs--;
    // A crude check, but it's hard to be more precise with the current
    // InMemory structure without being very inefficient.
    if (totdocs == 0) positions_present = false;

    vector<InMemoryTermEntry>::const_iterator i;
    for (i = termlists[did - 1].terms.begin();
	 i != termlists[did - 1].terms.end();
	 ++i) {
	map<string, InMemoryTerm>::iterator t = postlists.find(i->tname);
	Assert(t != postlists.end());
	t->second.collection_freq -= i->wdf;
	--t->second.term_freq;
	vector<InMemoryPosting>::iterator posting = t->second.docs.begin();
	while (posting != t->second.docs.end()) {
	    // Just zero out erased doc ids - otherwise we need to erase
	    // in a vector (inefficient) and we break any posting lists
	    // iterating over this posting list.
	    if (posting->did == did) posting->valid = false;
	    ++posting;
	}
    }
    termlists[did-1].terms.clear();
}

void
InMemoryDatabase::replace_document(Xapian::docid did,
				   const Xapian::Document & document)
{
    DEBUGLINE(DB, "InMemoryDatabase::replace_document(): replacing doc "
	          << did);

    if (doc_exists(did)) { 
	doclists[did - 1] = "";
	valuelists[did - 1].clear();
	totlen -= doclengths[did - 1];
	totdocs--;
    } else if (did > termlists.size()) {
	termlists.resize(did);
	termlists[did - 1].is_valid = true;
	doclengths.resize(did);
	doclists.resize(did);
	valuelists.resize(did);
    } else {
	termlists[did - 1].is_valid = true;
    }

    vector<InMemoryTermEntry>::const_iterator i;
    for (i = termlists[did - 1].terms.begin();
	 i != termlists[did - 1].terms.end();
	 ++i) {
	map<string, InMemoryTerm>::iterator t = postlists.find(i->tname);
	Assert(t != postlists.end());
	t->second.collection_freq -= i->wdf;
	--t->second.term_freq;
	vector<InMemoryPosting>::iterator posting = t->second.docs.begin();
	while (posting != t->second.docs.end()) {
	    // Just invalidate erased doc ids - otherwise we need to erase
	    // in a vector (inefficient) and we break any posting lists
	    // iterating over this posting list.
	    if (posting->did == did) posting->valid = false;
	    ++posting;
	}
    }

    doclengths[did - 1] = 0;
    doclists[did - 1] = document.get_data();

    finish_add_doc(did, document);
}

Xapian::docid
InMemoryDatabase::add_document(const Xapian::Document & document)
{
    Xapian::docid did = make_doc(document.get_data());

    DEBUGLINE(DB, "InMemoryDatabase::add_document(): adding doc " << did);

    finish_add_doc(did, document);

    return did;
}

void
InMemoryDatabase::finish_add_doc(Xapian::docid did, const Xapian::Document &document)
{
    {
	map<Xapian::valueno, string> values;
	Xapian::ValueIterator k = document.values_begin();
	Xapian::ValueIterator k_end = document.values_end();
	for ( ; k != k_end; ++k) {
	    values.insert(make_pair(k.get_valueno(), *k));
	    DEBUGLINE(DB, "InMemoryDatabase::finish_add_doc(): adding value "
		      << k.get_valueno() << " -> " << *k);
	}
	add_values(did, values);
    }

    InMemoryDoc doc(true);
    Xapian::TermIterator i = document.termlist_begin();
    Xapian::TermIterator i_end = document.termlist_end();
    for ( ; i != i_end; ++i) {
	make_term(*i);

	DEBUGLINE(DB, "InMemoryDatabase::finish_add_doc(): adding term "
		  << *i);
	Xapian::PositionIterator j = i.positionlist_begin();
	Xapian::PositionIterator j_end = i.positionlist_end();

	if (j == j_end) {
	    /* Make sure the posting exists, even without a position. */
	    make_posting(&doc, *i, did, 0, i.get_wdf(), false);
	} else {
	    positions_present = true;
	    for ( ; j != j_end; ++j) {
		make_posting(&doc, *i, did, *j, i.get_wdf());
	    }
	}

	Assert(did > 0 && did <= doclengths.size());
	doclengths[did - 1] += i.get_wdf();
	totlen += i.get_wdf();
	postlists[*i].collection_freq += i.get_wdf();
	++postlists[*i].term_freq;
    }
    swap(termlists[did - 1], doc);

    totdocs++;
}

void
InMemoryDatabase::make_term(const string & tname)
{
    postlists[tname];  // Initialise, if not already there.
}

Xapian::docid
InMemoryDatabase::make_doc(const string & docdata)
{
    termlists.push_back(InMemoryDoc(true));
    doclengths.push_back(0);
    doclists.push_back(docdata);

    AssertEqParanoid(termlists.size(), doclengths.size());

    return termlists.size();
}

void InMemoryDatabase::make_posting(InMemoryDoc * doc,
				    const string & tname,
				    Xapian::docid did,
				    Xapian::termpos position,
				    Xapian::termcount wdf,
				    bool use_position)
{
    Assert(doc);
    Assert(postlists.find(tname) != postlists.end());
    Assert(did > 0 && did <= termlists.size());
    Assert(did > 0 && did <= doclengths.size());
    Assert(doc_exists(did));

    // Make the posting
    InMemoryPosting posting;
    posting.did = did;
    if (use_position) {
	posting.positions.push_back(position);
    }
    posting.wdf = wdf;
    posting.valid = true;

    // Now record the posting
    postlists[tname].add_posting(posting);

    // Make the termentry
    InMemoryTermEntry termentry;
    termentry.tname = tname;
    if (use_position) {
	termentry.positions.push_back(position);
    }
    termentry.wdf = wdf;

    // Now record the termentry
    doc->add_posting(termentry);
}

bool
InMemoryDatabase::term_exists(const string & tname) const
{
    Assert(!tname.empty());
    map<string, InMemoryTerm>::const_iterator i = postlists.find(tname);
    if (i == postlists.end()) return false;
    return (i->second.term_freq != 0);
}

bool
InMemoryDatabase::has_positions() const
{
    return positions_present;
}

TermList *
InMemoryDatabase::open_allterms(const string & prefix) const
{
    return new InMemoryAllTermsList(&postlists,
				    Xapian::Internal::RefCntPtr<const InMemoryDatabase>(this),
				    prefix);
}
