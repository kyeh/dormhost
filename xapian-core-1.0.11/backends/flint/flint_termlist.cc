/* flint_termlist.cc: Termlists in a flint database
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2002,2003,2004,2006,2007 Olly Betts
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

#include <xapian/error.h>

#include "expandweight.h"
#include "flint_positionlist.h"
#include "flint_termlist.h"
#include "flint_utils.h"
#include "omassert.h"
#include "utils.h"

using namespace std;

FlintTermList::FlintTermList(Xapian::Internal::RefCntPtr<const FlintDatabase> db_,
			     Xapian::docid did_)
	: db(db_), did(did_), current_wdf(0), current_termfreq(0)
{
    DEBUGCALL(DB, void, "FlintTermList",
	      "[RefCntPtr<const FlintDatabase>], " << did_);

    if (!db->termlist_table.get_exact_entry(flint_docid_to_key(did), data))
	throw Xapian::DocNotFoundError("No termlist for document " + om_tostring(did));

    pos = data.data();
    end = pos + data.size();

    if (pos == end) {
	doclen = 0;
	termlist_size = 0;
	return;
    }

    // Read doclen
    if (!unpack_uint(&pos, end, &doclen)) {
	const char *msg;
	if (pos == 0) {
	    msg = "Too little data for doclen in termlist";
	} else {
	    msg = "Overflowed value for doclen in termlist";
	}
	throw Xapian::DatabaseCorruptError(msg);
    }

    // Read termlist_size
    if (!unpack_uint(&pos, end, &termlist_size)) {
	const char *msg;
	if (pos == 0) {
	    msg = "Too little data for list size in termlist";
	} else {
	    msg = "Overflowed value for list size in termlist";
	}
	throw Xapian::DatabaseCorruptError(msg);
    }

    // See comment in FlintTermListTable::set_termlist() in
    // flint_termlisttable.cc for an explanation of this!
    if (pos != end && *pos == '0') ++pos;
}

flint_doclen_t
FlintTermList::get_doclength() const
{
    DEBUGCALL(DB, flint_doclen_t, "FlintTermList::get_doclength", "");
    RETURN(doclen);
}

Xapian::termcount
FlintTermList::get_approx_size() const
{
    DEBUGCALL(DB, Xapian::termcount, "FlintTermList::get_approx_size", "");
    RETURN(termlist_size);
}

void
FlintTermList::accumulate_stats(Xapian::Internal::ExpandStats & stats) const
{
    DEBUGCALL(DB, void, "FlintTermList::accumulate_stats", "[stats&]");
    Assert(!at_end());
    stats.accumulate(current_wdf, doclen, get_termfreq(), db->get_doccount());
}

string
FlintTermList::get_termname() const
{
    DEBUGCALL(DB, string, "FlintTermList::get_termname", "");
    RETURN(current_term);
}

Xapian::termcount
FlintTermList::get_wdf() const
{
    DEBUGCALL(DB, Xapian::termcount, "FlintTermList::get_wdf", "");
    RETURN(current_wdf);
}

Xapian::doccount
FlintTermList::get_termfreq() const
{
    DEBUGCALL(DB, Xapian::doccount, "FlintTermList::get_termfreq", "");
    if (current_termfreq == 0)
	current_termfreq = db->get_termfreq(current_term);
    RETURN(current_termfreq);
}

TermList *
FlintTermList::next()
{
    DEBUGCALL(DB, TermList *, "FlintTermList::next", "");
    Assert(!at_end());
    if (pos == end) {
	pos = NULL;
	RETURN(NULL);
    }

    // Reset to 0 to indicate that the termfreq needs to be read.
    current_termfreq = 0;

    bool wdf_in_reuse = false;
    if (!current_term.empty()) {
	// Find out how much of the previous term to reuse.
	size_t len = static_cast<unsigned char>(*pos++);
	if (len > current_term.size()) {
	    // The wdf is also stored in the "reuse" byte.
	    wdf_in_reuse = true;
	    size_t divisor = current_term.size() + 1;
	    current_wdf = len / divisor - 1;
	    len %= divisor;
	}
	current_term.resize(len);
    }

    // Append the new tail to form the next term.
    size_t append_len = static_cast<unsigned char>(*pos++);
    current_term.append(pos, append_len);
    pos += append_len;

    // Read the wdf if it wasn't packed into the reuse byte.
    if (!wdf_in_reuse && !unpack_uint(&pos, end, &current_wdf)) {
	const char *msg;
	if (pos == 0) {
	    msg = "Too little data for wdf in termlist";
	} else {
	    msg = "Overflowed value for wdf in termlist";
	}
	throw Xapian::DatabaseCorruptError(msg);
    }

    RETURN(NULL);
}

bool
FlintTermList::at_end() const
{
    DEBUGCALL(DB, bool, "FlintTermList::at_end", "");
    RETURN(pos == NULL);
}

Xapian::termcount
FlintTermList::positionlist_count() const
{
    DEBUGCALL(DB, Xapian::termcount, "FlintTermList::positionlist_count", "");
    RETURN(db->position_table.positionlist_count(did, current_term));
}

Xapian::PositionIterator
FlintTermList::positionlist_begin() const
{
    DEBUGCALL(DB, Xapian::PositionIterator, "FlintTermList::positionlist_begin", "");
    return Xapian::PositionIterator(
	    new FlintPositionList(&db->position_table, did, current_term));
}
