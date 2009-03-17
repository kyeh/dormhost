/** @file flint_alldocspostlist.cc
 * @brief A PostList which iterates over all documents in a FlintDatabase.
 */
/* Copyright (C) 2006,2007 Olly Betts
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

#include <string>

#include "flint_database.h"
#include "flint_alldocspostlist.h"

#include "utils.h"

using namespace std;

FlintAllDocsPostList::~FlintAllDocsPostList()
{
}

Xapian::doccount
FlintAllDocsPostList::get_termfreq() const
{
    return doccount;
}

Xapian::docid
FlintAllDocsPostList::get_docid() const
{
    return current_did;
}

Xapian::doclength
FlintAllDocsPostList::get_doclength() const
{
    DEBUGCALL(DB, Xapian::doclength, "FlintAllDocsPostList::get_doclength", "");
    Assert(current_did);

    cursor->read_tag();

    if (cursor->current_tag.empty()) RETURN(0);

    const char * pos = cursor->current_tag.data();
    const char * end = pos + cursor->current_tag.size();

    flint_doclen_t doclen;
    if (!unpack_uint(&pos, end, &doclen)) {
	const char *msg;
	if (pos == 0) {
	    msg = "Too little data for doclen in termlist";
	} else {
	    msg = "Overflowed value for doclen in termlist";
	}
	throw Xapian::DatabaseCorruptError(msg);
    }

    RETURN(doclen);
}

Xapian::termcount
FlintAllDocsPostList::get_wdf() const
{
    DEBUGCALL(DB, Xapian::termcount, "FlintAllDocsPostList::get_wdf", "");
    Assert(current_did);
    RETURN(1);
}

PostList *
FlintAllDocsPostList::read_did_from_current_key()
{
    DEBUGCALL(DB, PostList *, "FlintAllDocsPostList::read_did_from_current_key",
	      "");
    const string & key = cursor->current_key;
    const char * pos = key.data();
    const char * end = pos + key.size();
    if (!unpack_uint_preserving_sort(&pos, end, &current_did)) {
	const char *msg;
	if (pos == 0) {
	    msg = "Too little data in termlist key";
	} else {
	    msg = "Overflowed value in termlist key";
	}
	throw Xapian::DatabaseCorruptError(msg);
    }

    // Return NULL to help the compiler tail-call optimise our callers.
    RETURN(NULL);
}

PostList *
FlintAllDocsPostList::next(Xapian::weight /*w_min*/)
{
    DEBUGCALL(DB, PostList *, "FlintAllDocsPostList::next", "/*w_min*/");
    Assert(!at_end());
    if (!cursor->next()) RETURN(NULL);
    RETURN(read_did_from_current_key());
}

PostList *
FlintAllDocsPostList::skip_to(Xapian::docid did, Xapian::weight /*w_min*/)
{
    DEBUGCALL(DB, PostList *, "FlintAllDocsPostList::skip_to",
	      did << ", /*w_min*/");

    if (did <= current_did || at_end()) RETURN(NULL);

    if (cursor->find_entry_ge(pack_uint_preserving_sort(did))) {
	// The exact docid that was asked for exists.
	current_did = did;
	RETURN(NULL);
    }
    if (cursor->after_end()) RETURN(NULL);

    RETURN(read_did_from_current_key());
}

bool
FlintAllDocsPostList::at_end() const {
    DEBUGCALL(DB, bool, "FlintAllDocsPostList::at_end", "");
    RETURN(cursor->after_end());
}

string
FlintAllDocsPostList::get_description() const
{
    string desc = "FlintAllDocsPostList(did=";
    desc += om_tostring(current_did);
    desc += ",doccount=";
    desc += om_tostring(doccount);
    desc += ')';
    return desc;
}
