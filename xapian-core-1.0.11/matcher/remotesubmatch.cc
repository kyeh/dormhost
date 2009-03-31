/** @file remotesubmatch.cc
 *  @brief SubMatch class for a remote database.
 */
/* Copyright (C) 2006,2007 Olly Betts
 * Copyright (C) 2007,2008 Lemur Consulting Ltd
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
#include "remotesubmatch.h"

#include "msetpostlist.h"
#include "omdebug.h"
#include "remote-database.h"
#include "stats.h"

RemoteSubMatch::RemoteSubMatch(RemoteDatabase *db_, bool decreasing_relevance_)
	: db(db_), decreasing_relevance(decreasing_relevance_)
{
    DEBUGCALL(MATCH, void, "RemoteSubMatch",
	      db_ << ", " << decreasing_relevance_);
}

bool
RemoteSubMatch::prepare_match(bool nowait, Stats & total_stats)
{
    DEBUGCALL(MATCH, bool, "RemoteSubMatch::prepare_match", nowait);
    Stats remote_stats;
    if (!db->get_remote_stats(nowait, remote_stats)) RETURN(false);
    total_stats += remote_stats;
    RETURN(true);
}

void
RemoteSubMatch::start_match(Xapian::doccount first,
			    Xapian::doccount maxitems,
			    Xapian::doccount check_at_least,
			    const Stats & total_stats)
{
    DEBUGCALL(MATCH, void, "RemoteSubMatch::start_match",
	      first << ", " << maxitems << ", " << check_at_least);
    db->send_global_stats(first, maxitems, check_at_least, total_stats);
}

PostList *
RemoteSubMatch::get_postlist_and_term_info(MultiMatch *,
	map<string, Xapian::MSet::Internal::TermFreqAndWeight> * termfreqandwts)
{
    DEBUGCALL(MATCH, PostList *, "RemoteSubMatch::get_postlist_and_term_info",
	      "[matcher], " << (void*)termfreqandwts);
    Xapian::MSet mset;
    db->get_mset(mset);
    percent_factor = mset.internal->percent_factor;
    if (termfreqandwts) *termfreqandwts = mset.internal->termfreqandwts;
    return new MSetPostList(mset, decreasing_relevance);
}
