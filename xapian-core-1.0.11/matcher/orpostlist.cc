/* orpostlist.cc: OR of two posting lists
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2001,2002 Ananova Ltd
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

#include "orpostlist.h"
#include "andpostlist.h"
#include "andmaybepostlist.h"
#include "omassert.h"
#include "omdebug.h"

#include <algorithm>

OrPostList::OrPostList(PostList *left_,
		       PostList *right_,
		       MultiMatch *matcher_,
		       Xapian::doccount dbsize_)
	: BranchPostList(left_, right_, matcher_),
	  lhead(0), rhead(0), lmax(0), rmax(0), minmax(0), dbsize(dbsize_)
{
    DEBUGCALL(MATCH, void, "OrPostList", left_ << ", " << right_ << ", " << matcher_ << ", " << dbsize_);
}

PostList *
OrPostList::next(Xapian::weight w_min)
{
    DEBUGCALL(MATCH, PostList *, "OrPostList::next", w_min);
    if (w_min > minmax) {
	// we can replace the OR with another operator
	PostList *ret;
	if (w_min > lmax) {
	    if (w_min > rmax) {
		DEBUGLINE(MATCH, "OR -> AND");
		ret = new AndPostList(l, r, matcher, dbsize, true);
		skip_to_handling_prune(ret, std::max(lhead, rhead) + 1, w_min,
				       matcher);
	    } else {
		DEBUGLINE(MATCH, "OR -> AND MAYBE (1)");
		ret = new AndMaybePostList(r, l, matcher, dbsize, rhead, lhead);
		next_handling_prune(ret, w_min, matcher);
	    }
	} else {
	    // w_min > rmax since w_min > minmax but not (w_min > lmax)
	    Assert(w_min > rmax);
	    DEBUGLINE(MATCH, "OR -> AND MAYBE (2)");
	    ret = new AndMaybePostList(l, r, matcher, dbsize, lhead, rhead);
	    next_handling_prune(ret, w_min, matcher);
	}

	l = r = NULL;
	RETURN(ret);
    }

    bool ldry = false;
    bool rnext = false;

    if (lhead <= rhead) {
        if (lhead == rhead) rnext = true;
        next_handling_prune(l, w_min - rmax, matcher);
	if (l->at_end()) ldry = true;
    } else {
	rnext = true;
    }

    if (rnext) {
        next_handling_prune(r, w_min - lmax, matcher);
        if (r->at_end()) {
	    PostList *ret = l;
	    l = NULL;
	    RETURN(ret);
	}
	rhead = r->get_docid();
    }

    if (!ldry) {
	lhead = l->get_docid();
	RETURN(NULL);
    }

    PostList *ret = r;
    r = NULL;
    RETURN(ret);
}

PostList *
OrPostList::skip_to(Xapian::docid did, Xapian::weight w_min)
{
    DEBUGCALL(MATCH, PostList *, "OrPostList::skip_to", did << ", " << w_min);
    if (w_min > minmax) {
	// we can replace the OR with another operator
	PostList *ret;
	if (w_min > lmax) {
	    if (w_min > rmax) {
		DEBUGLINE(MATCH, "OR -> AND (in skip_to)");
		ret = new AndPostList(l, r, matcher, dbsize, true);
		did = std::max(did, std::max(lhead, rhead));
	    } else {
		DEBUGLINE(MATCH, "OR -> AND MAYBE (in skip_to) (1)");
		ret = new AndMaybePostList(r, l, matcher, dbsize, rhead, lhead);
		did = std::max(did, rhead);
	    }
	} else {
	    // w_min > rmax since w_min > minmax but not (w_min > lmax)
	    Assert(w_min > rmax);
	    DEBUGLINE(MATCH, "OR -> AND MAYBE (in skip_to) (2)");
	    ret = new AndMaybePostList(l, r, matcher, dbsize, lhead, rhead);
	    did = std::max(did, lhead);
	}

	l = r = NULL;
	skip_to_handling_prune(ret, did, w_min, matcher);
	RETURN(ret);
    }

    bool ldry = false;
    if (lhead < did) {
	skip_to_handling_prune(l, did, w_min - rmax, matcher);
	ldry = l->at_end();
    }

    if (rhead < did) {
	skip_to_handling_prune(r, did, w_min - lmax, matcher);

	if (r->at_end()) {
	    PostList *ret = l;
	    l = NULL;
	    RETURN(ret);
	}
	rhead = r->get_docid();
    }

    if (!ldry) {
	lhead = l->get_docid();
	RETURN(NULL);
    }

    PostList *ret = r;
    r = NULL;
    RETURN(ret);
}

Xapian::doccount
OrPostList::get_termfreq_max() const
{
    DEBUGCALL(MATCH, Xapian::doccount, "OrPostList::get_termfreq_max", "");
    RETURN(std::min(l->get_termfreq_max() + r->get_termfreq_max(), dbsize));
}

Xapian::doccount
OrPostList::get_termfreq_min() const
{
    DEBUGCALL(MATCH, Xapian::doccount, "OrPostList::get_termfreq_min", "");
    RETURN(std::max(l->get_termfreq_min(), r->get_termfreq_min()));
}

Xapian::doccount
OrPostList::get_termfreq_est() const
{
    DEBUGCALL(MATCH, Xapian::doccount, "OrPostList::get_termfreq_est", "");
    // Estimate assuming independence:
    // P(l or r) = P(l) + P(r) - P(l) . P(r)
    double lest = static_cast<double>(l->get_termfreq_est());
    double rest = static_cast<double>(r->get_termfreq_est());
    double est = lest + rest - (lest * rest / dbsize);
    RETURN(static_cast<Xapian::doccount>(est + 0.5));
}

Xapian::docid
OrPostList::get_docid() const
{
    DEBUGCALL(MATCH, Xapian::docid, "OrPostList::get_docid", "");
    Assert(lhead != 0 && rhead != 0); // check we've started
    RETURN(std::min(lhead, rhead));
}

// only called if we are doing a probabilistic OR
Xapian::weight
OrPostList::get_weight() const
{
    DEBUGCALL(MATCH, Xapian::weight, "OrPostList::get_weight", "");
    Assert(lhead != 0 && rhead != 0); // check we've started
    if (lhead < rhead) RETURN(l->get_weight());
    if (lhead > rhead) RETURN(r->get_weight());
    RETURN(l->get_weight() + r->get_weight());
}

// only called if we are doing a probabilistic operation
Xapian::weight
OrPostList::get_maxweight() const
{
    DEBUGCALL(MATCH, Xapian::weight, "OrPostList::get_maxweight", "");
    RETURN(lmax + rmax);
}

Xapian::weight
OrPostList::recalc_maxweight()
{
    DEBUGCALL(MATCH, Xapian::weight, "OrPostList::recalc_maxweight", "");
    lmax = l->recalc_maxweight();
    rmax = r->recalc_maxweight();
    minmax = std::min(lmax, rmax);
    RETURN(OrPostList::get_maxweight());
}

bool
OrPostList::at_end() const
{
    DEBUGCALL(MATCH, bool, "OrPostList::at_end", "");
    // Can never really happen - OrPostList next/skip_to autoprune
    AssertParanoid(!(l->at_end()) && !(r->at_end()));
    RETURN(false);
}

std::string
OrPostList::get_description() const
{
    return "(" + l->get_description() + " Or " + r->get_description() + ")";
}

Xapian::doclength
OrPostList::get_doclength() const
{
    DEBUGCALL(MATCH, Xapian::doclength, "OrPostList::get_doclength", "");
    Xapian::doclength doclength;

    Assert(lhead != 0 && rhead != 0); // check we've started
    if (lhead > rhead) {
	doclength = r->get_doclength();
	DEBUGLINE(MATCH, "OrPostList::get_doclength() [right docid=" 
		  << rhead << "] = " << doclength);
    } else {
	doclength = l->get_doclength();
	DEBUGLINE(MATCH, "OrPostList::get_doclength() [left docid="
		  << lhead << "] = " << doclength);
    }

    RETURN(doclength);
}
