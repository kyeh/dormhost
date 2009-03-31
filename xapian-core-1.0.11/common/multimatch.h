/* multimatch.h: class for performing a match
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002,2003,2004,2005,2006,2007 Olly Betts
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

#ifndef OM_HGUARD_MULTIMATCH_H
#define OM_HGUARD_MULTIMATCH_H

#include "omqueryinternal.h"
#include "submatch.h"

#include <vector>
#include <map>

// Forward declaration.
class Stats;

class MultiMatch
{
    private:
	/// Vector of the items.
	std::vector<Xapian::Internal::RefCntPtr<SubMatch> > leaves;

	const Xapian::Database db;

	const Xapian::Query::Internal *query;

	Xapian::valueno collapse_key;

	int percent_cutoff;

	Xapian::weight weight_cutoff;

	Xapian::Enquire::docid_order order;

	Xapian::valueno sort_key;

	Xapian::Enquire::Internal::sort_setting sort_by;

	bool sort_value_forward;

	const Xapian::Sorter * sorter;

	/// ErrorHandler
	Xapian::ErrorHandler * errorhandler;

	/// Weighting scheme
	const Xapian::Weight * weight;

	/** Internal flag to note that w_max needs to be recalculated
	 *  while query is running.
	 */
        bool recalculate_w_max;

	/** Is each sub-database remote? */
	vector<bool> is_remote;

	/// get the collapse key
	string get_collapse_key(PostList *pl,
				Xapian::docid did, Xapian::valueno keyno,
				Xapian::Internal::RefCntPtr<Xapian::Document::Internal> &doc);

	/** get the maxweight that the postlist pl may return, calling
	 *  recalc_maxweight if recalculate_w_max is set, and unsetting it.
	 *  Must only be called on the top of the postlist tree.
	 */
        Xapian::weight getorrecalc_maxweight(PostList *pl);

	/// Copying is not permitted.
	MultiMatch(const MultiMatch &);

	/// Assignment is not permitted.
	void operator=(const MultiMatch &);

    public:
	/** MultiMatch constructor.
	 *
	 *  @param db_       The database to use.
	 *  @param query     The query
	 *  @param qlen      The query length
	 *  @param omrset    The relevance set (or NULL for no RSet)
	 *  @param errorhandler Errorhandler object
	 *  @param sorter    Xapian::Sorter functor (or NULL for no Sorter).
	 *  @param stats     The stats object to add our stats to.
	 *  @param wtscheme  Weighting scheme
	 */
	MultiMatch(const Xapian::Database &db_,
		   const Xapian::Query::Internal * query,
		   Xapian::termcount qlen,
		   const Xapian::RSet * omrset,
		   Xapian::valueno collapse_key_,
		   int percent_cutoff_,
		   Xapian::weight weight_cutoff_,
		   Xapian::Enquire::docid_order order_,
		   Xapian::valueno sort_key_,
		   Xapian::Enquire::Internal::sort_setting sort_by_,
		   bool sort_value_forward_,
		   const Xapian::Sorter * sorter_,
		   Xapian::ErrorHandler * errorhandler,
		   Stats & stats,
		   const Xapian::Weight *wtscheme);

	void get_mset(Xapian::doccount first,
		      Xapian::doccount maxitems,
		      Xapian::doccount check_at_least,
		      Xapian::MSet & mset,
		      const Stats & stats,
		      const Xapian::MatchDecider * mdecider,
		      const Xapian::MatchDecider * matchspy);

	/** Called by postlists to indicate that they've rearranged themselves
	 *  and the maxweight now possible is smaller.
	 */
        void recalc_maxweight();
};

#endif /* OM_HGUARD_MULTIMATCH_H */
