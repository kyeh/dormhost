/* multi_postlist.cc: interface to multiple database access
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002,2003,2004,2005,2007 Olly Betts
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

#include "omassert.h"
#include "omdebug.h"
#include "multi_postlist.h"
#include "utils.h"

#ifdef XAPIAN_DEBUG_PARANOID
#include "xapian/database.h"
#endif

MultiPostList::MultiPostList(std::vector<LeafPostList *> & pls,
			     const Xapian::Database &this_db_)
	: postlists(pls),
	  this_db(this_db_),
	  finished(false),
	  currdoc(0),
	  freq_initialised(false)
{
    multiplier = pls.size();
}


MultiPostList::~MultiPostList()
{
    std::vector<LeafPostList *>::iterator i;
    for (i = postlists.begin(); i != postlists.end(); i++) {
	delete *i;
    }
    postlists.clear();
}

Xapian::doccount
MultiPostList::get_termfreq() const
{
    if (freq_initialised) return termfreq;
    DEBUGLINE(DB, "Calculating multiple term frequencies");

    // Calculate and remember the termfreq
    termfreq = 0;
    std::vector<LeafPostList *>::const_iterator i;
    for (i = postlists.begin(); i != postlists.end(); i++) {
	termfreq += (*i)->get_termfreq();
    }

    freq_initialised = true;
    return termfreq;
}

Xapian::docid
MultiPostList::get_docid() const
{
    DEBUGCALL(DB, Xapian::docid, "MultiPostList::get_docid", "");
    Assert(!at_end());
    Assert(currdoc != 0);
    RETURN(currdoc);
}

Xapian::doclength
MultiPostList::get_doclength() const
{
    DEBUGCALL(DB, Xapian::doclength, "MultiPostList::get_doclength", "");
    Assert(!at_end());
    Assert(currdoc != 0);
    Xapian::doclength result = postlists[(currdoc - 1) % multiplier]->get_doclength();
    AssertEqParanoid(result, this_db.get_doclength(get_docid()));
    RETURN(result);
}

Xapian::termcount
MultiPostList::get_wdf() const
{
    return postlists[(currdoc - 1) % multiplier]->get_wdf();
}

PositionList *
MultiPostList::read_position_list()
{
    return postlists[(currdoc - 1) % multiplier]->read_position_list();
}

PositionList *
MultiPostList::open_position_list() const
{
    return postlists[(currdoc - 1) % multiplier]->open_position_list();
}

PostList *
MultiPostList::next(Xapian::weight w_min)
{
    DEBUGCALL(DB, PostList *, "MultiPostList::next", w_min);
    Assert(!at_end());

    Xapian::docid newdoc = 0;
    Xapian::docid offset = 1;
    std::vector<LeafPostList *>::iterator i;
    for (i = postlists.begin(); i != postlists.end(); i++) {
	if (!(*i)->at_end()) {
	    Xapian::docid id = ((*i)->get_docid() - 1) * multiplier + offset;
	    // Check if it needs to be advanced
	    if (currdoc >= id) {
		(*i)->next(w_min);
		if (!(*i)->at_end()) {
		    id = ((*i)->get_docid() - 1) * multiplier + offset;
		    if (newdoc == 0 || id < newdoc) newdoc = id;
		}
	    } else {
		if (newdoc == 0 || id < newdoc) newdoc = id;
	    }
	}
	offset++;
    }
    if (newdoc) {
	DEBUGLINE(DB, "MultiPostList::next() newdoc=" << newdoc <<
		  " (olddoc=" << currdoc << ")");
	currdoc = newdoc;
    } else {
	DEBUGLINE(DB, "MultiPostList::next() finished" <<
		  " (olddoc=" << currdoc << ")");
	finished = true;
    }
    RETURN(NULL);
}

PostList *
MultiPostList::skip_to(Xapian::docid did, Xapian::weight w_min)
{
    DEBUGCALL(DB, PostList *, "MultiPostList::skip_to", did << ", " << w_min);
    Assert(!at_end());
    Xapian::docid newdoc = 0;
    Xapian::docid offset = 0;
    Xapian::docid realdid = (did - 1) / multiplier + 2;
    Xapian::doccount dbnumber = (did - 1) % multiplier;
    std::vector<LeafPostList *>::iterator i;
    for (i = postlists.begin(); i != postlists.end(); i++) {	
	if (offset == dbnumber) --realdid;
	++offset;
	Assert((realdid - 1) * multiplier + offset >= did);
	Assert((realdid - 1) * multiplier + offset < did + multiplier);
	if (!(*i)->at_end()) {
	    (*i)->skip_to(realdid, w_min);
	    if (!(*i)->at_end()) {
		Xapian::docid id = ((*i)->get_docid() - 1) * multiplier + offset;
		if (newdoc == 0 || id < newdoc) newdoc = id;
	    }
	}
    }
    if (newdoc) {
	currdoc = newdoc;
    } else {
	finished = true;
    }
    RETURN(NULL);
}

bool
MultiPostList::at_end() const
{
    return finished;
}

std::string
MultiPostList::get_description() const
{
    std::string desc = "[";

    std::vector<LeafPostList *>::const_iterator i;
    for (i = postlists.begin(); i != postlists.end(); i++) {
	desc += (*i)->get_description();
	if (i != postlists.end()) desc += ",";
    }

    return desc + "]:" + om_tostring(get_termfreq());
}
