/* andmaybepostlist.cc: Merged postlist; items from one list, weights from both
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2003,2004,2005 Olly Betts
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

#include "andmaybepostlist.h"
#include "andpostlist.h"
#include "omassert.h"
#include "omdebug.h"

PostList *
AndMaybePostList::process_next_or_skip_to(Xapian::weight w_min, PostList *ret)
{
    DEBUGCALL(MATCH, PostList *, "AndMaybePostList::process_next_or_skip_to", w_min << ", " << ret);
    handle_prune(l, ret);
    if (l->at_end()) {
	// once l is over, so is the AND MAYBE
	lhead = 0;
	RETURN(NULL);
    }

    lhead = l->get_docid();
    if (lhead <= rhead) RETURN(NULL);

    skip_to_handling_prune(r, lhead, w_min - lmax, matcher);
    if (r->at_end()) {
	PostList *tmp = l;
	l = NULL;
	RETURN(tmp);
    }
    rhead = r->get_docid();
    RETURN(NULL);
}

PostList *
AndMaybePostList::next(Xapian::weight w_min)
{
    DEBUGCALL(MATCH, PostList *, "AndMaybePostList::next", w_min);
    if (w_min > lmax) {
	// we can replace the AND MAYBE with an AND
	PostList *ret;
	DEBUGLINE(MATCH, "AND MAYBE -> AND");
	ret = new AndPostList(l, r, matcher, dbsize, true);
	l = r = NULL;
	skip_to_handling_prune(ret, std::max(lhead, rhead) + 1, w_min, matcher);
	RETURN(ret);
    }
    RETURN(process_next_or_skip_to(w_min, l->next(w_min - rmax)));
}

PostList *
AndMaybePostList::skip_to(Xapian::docid did, Xapian::weight w_min)
{
    DEBUGCALL(MATCH, PostList *, "AndMaybePostList::skip_to", did << ", " << w_min);
    if (w_min > lmax) {
	// we can replace the AND MAYBE with an AND
	PostList *ret;
	DEBUGLINE(MATCH, "AND MAYBE -> AND (in skip_to)");
	ret = new AndPostList(l, r, matcher, dbsize, true);
	did = std::max(did, std::max(lhead, rhead));
	l = r = NULL;
	skip_to_handling_prune(ret, did, w_min, matcher);
	RETURN(ret);
    }

    // exit if we're already past the skip point (or at it)
    if (did <= lhead) RETURN(NULL);

    RETURN(process_next_or_skip_to(w_min, l->skip_to(did, w_min - rmax)));
}

Xapian::doccount
AndMaybePostList::get_termfreq_max() const
{
    DEBUGCALL(MATCH, Xapian::doccount, "AndMaybePostList::get_termfreq_max", "");
    // Termfreq is exactly that of left hand branch.
    RETURN(l->get_termfreq_max());
}

Xapian::doccount
AndMaybePostList::get_termfreq_min() const
{
    DEBUGCALL(MATCH, Xapian::doccount, "AndMaybePostList::get_termfreq_min", "");
    // Termfreq is exactly that of left hand branch.
    RETURN(l->get_termfreq_min());
}

Xapian::doccount
AndMaybePostList::get_termfreq_est() const
{
    DEBUGCALL(MATCH, Xapian::doccount, "AndMaybePostList::get_termfreq_est", "");
    // Termfreq is exactly that of left hand branch.
    RETURN(l->get_termfreq_est());
}

Xapian::docid
AndMaybePostList::get_docid() const
{
    DEBUGCALL(MATCH, Xapian::docid, "AndMaybePostList::get_docid", "");
    Assert(lhead != 0 && rhead != 0); // check we've started
    RETURN(lhead);
}

// only called if we are doing a probabilistic AND MAYBE
Xapian::weight
AndMaybePostList::get_weight() const
{
    DEBUGCALL(MATCH, Xapian::weight, "AndMaybePostList::get_weight", "");
    Assert(lhead != 0 && rhead != 0); // check we've started
    if (lhead == rhead) RETURN(l->get_weight() + r->get_weight());
    RETURN(l->get_weight());
}

// only called if we are doing a probabilistic operation
Xapian::weight
AndMaybePostList::get_maxweight() const
{
    DEBUGCALL(MATCH, Xapian::weight, "AndMaybePostList::get_maxweight", "");
    RETURN(lmax + rmax);
}

Xapian::weight
AndMaybePostList::recalc_maxweight()
{
    DEBUGCALL(MATCH, Xapian::weight, "AndMaybePostList::recalc_maxweight", "");
    lmax = l->recalc_maxweight();
    rmax = r->recalc_maxweight();
    RETURN(AndMaybePostList::get_maxweight());
}

bool
AndMaybePostList::at_end() const
{
    DEBUGCALL(MATCH, bool, "AndMaybePostList::at_end", "");
    RETURN(lhead == 0);
}

std::string
AndMaybePostList::get_description() const
{
    return "(" + l->get_description() + " AndMaybe " + r->get_description() +
	   ")";
}

Xapian::doclength
AndMaybePostList::get_doclength() const
{
    DEBUGCALL(MATCH, Xapian::doclength, "AndMaybePostList::get_doclength", "");
    Assert(lhead != 0 && rhead != 0); // check we've started
    if (lhead == rhead) AssertEqDouble(l->get_doclength(), r->get_doclength());
    RETURN(l->get_doclength());
}
