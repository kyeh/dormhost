/* selectpostlist.cc: Parent class for classes which only return selected docs
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2003,2004,2007 Olly Betts
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

#include "selectpostlist.h"
#include "omassert.h"
#include "omdebug.h"

PostList *
SelectPostList::next(Xapian::weight w_min)
{
    DEBUGCALL(MATCH, PostList *, "SelectPostList::next", w_min);
    do {
        PostList *p = source->next(w_min);
	(void)p;
	Assert(p == NULL); // AND should never prune
    } while (!source->at_end() && !test_doc());
    RETURN(NULL);
}

PostList *
SelectPostList::skip_to(Xapian::docid did, Xapian::weight w_min)
{
    DEBUGCALL(MATCH, PostList *, "SelectPostList::skip_to", did << ", " << w_min);
    if (did > get_docid()) {
	PostList *p = source->skip_to(did, w_min);
	(void)p;
	Assert(p == NULL); // AND should never prune
        if (!source->at_end() && !test_doc())
	    RETURN(SelectPostList::next(w_min));
    }
    RETURN(NULL);
}
