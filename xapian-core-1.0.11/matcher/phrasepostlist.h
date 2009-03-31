/* phrasepostlist.h: Return only items where terms are near or form a phrase
 *
 * ----START-LICENCE----
 * Copyright 1999,2000,2001 BrightStation PLC
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 * -----END-LICENCE-----
 */

#ifndef OM_HGUARD_PHRASEPOSTLIST_H
#define OM_HGUARD_PHRASEPOSTLIST_H

#include "selectpostlist.h"
#include <vector>

typedef Xapian::PositionIterator::Internal PositionList;

/** A postlist comprising several postlists NEARed together.
 *
 *  This postlist returns a posting if and only if it is in all of the
 *  sub-postlists and all the terms occur within a specified distance of
 *  each other somewhere in the document.  The weight for a posting is the
 *  sum of the weights of the sub-postings.
 */
class NearPostList : public SelectPostList {
    private:
        Xapian::termpos window;
	std::vector<PostList *> terms;

    	bool test_doc();
        bool do_test(std::vector<PositionList *> &plists, Xapian::termcount i,
		     Xapian::termcount min, Xapian::termcount max);
    public:
	std::string get_description() const;
	Xapian::termcount get_wdf() const;

	Xapian::doccount get_termfreq_est() const
	{
	    // No idea how to estimate this - FIXME
	    return source->get_termfreq_est() / 2;
	}

        NearPostList(PostList *source_, Xapian::termpos window_,
		     std::vector<PostList *> terms_)
		: SelectPostList(source_)
        {
	    window = window_;
	    terms = terms_;
	}
};

/** A postlist comprising several postlists PHRASEd together.
 *
 *  This postlist returns a posting if and only if it is in all of the
 *  sub-postlists and all the terms occur IN THE GIVEN ORDER within a
 *  specified distance of each other somewhere in the document.  The weight
 *  for a posting is the sum of the weights of the sub-postings.
 */
class PhrasePostList : public SelectPostList {
    private:
        Xapian::termpos window;
	std::vector<PostList *> terms;

    	bool test_doc();
        bool do_test(std::vector<PositionList *> &plists, Xapian::termcount i,
		     Xapian::termcount min, Xapian::termcount max);
    public:
	std::string get_description() const;
	Xapian::termcount get_wdf() const;

	Xapian::doccount get_termfreq_est() const
	{
	    // No idea how to estimate this - FIXME
	    return source->get_termfreq_est() / 3;
	}

        PhrasePostList(PostList *source_, Xapian::termpos window_,
		       std::vector<PostList *> terms_) : SelectPostList(source_)
        {
	    window = window_;
	    terms = terms_;
	}
};

#endif /* OM_HGUARD_PHRASEPOSTLIST_H */

