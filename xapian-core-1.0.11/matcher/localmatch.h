/** @file localmatch.h
 *  @brief SubMatch class for a local database.
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

#ifndef XAPIAN_INCLUDED_LOCALMATCH_H
#define XAPIAN_INCLUDED_LOCALMATCH_H

#include "database.h"
#include "omqueryinternal.h"
#include "rset.h"
#include "submatch.h"

namespace Xapian { class Weight; }

#include <map>
#include <vector>

class Stats;

class LocalSubMatch : public SubMatch {
    /// Don't allow assignment.
    void operator=(const LocalSubMatch &);

    /// Don't allow copying.
    LocalSubMatch(const LocalSubMatch &);

    /** The statistics for the collection.
     */
    const Stats * stats;

    /// The original query before any rearrangement.
    Xapian::Query::Internal orig_query;

    /// The query length (used by some weighting schemes).
    Xapian::termcount qlen;

    /// The (sub-)Database we're searching.
    const Xapian::Database::Internal *db;

    /** The (sub-)RSet (used to calculate R and r).
     *
     *  R and r are used in probabilistic weighting formulae.
     */
    RSetI rset;

    /// Weight object (used as a factory by calling create on it).
    const Xapian::Weight * wt_factory;

    /// The termfreqs and weights of terms used in orig_query.
    std::map<string, Xapian::MSet::Internal::TermFreqAndWeight> term_info;

  public:
    /// Constructor.
    LocalSubMatch(const Xapian::Database::Internal *db,
		  const Xapian::Query::Internal * query,
		  Xapian::termcount qlen,
		  const Xapian::RSet & omrset,
		  const Xapian::Weight *wt_factory);

    /// Fetch and collate statistics.
    bool prepare_match(bool nowait, Stats & total_stats);

    /// Start the match.
    void start_match(Xapian::doccount first,
		     Xapian::doccount maxitems,
		     Xapian::doccount check_at_least,
		     const Stats & total_stats);

    /// Get PostList and term info.
    PostList * get_postlist_and_term_info(MultiMatch *matcher,
	std::map<string, Xapian::MSet::Internal::TermFreqAndWeight> *termfreqandwts);

    /** Convert an OP_LEAF query to a PostList.
     *
     *  This is called by QueryOptimiser when it reaches an OP_LEAF query.
     */
    PostList * postlist_from_op_leaf_query(const Xapian::Query::Internal *query,
					   double factor);
};

#endif /* XAPIAN_INCLUDED_LOCALMATCH_H */
