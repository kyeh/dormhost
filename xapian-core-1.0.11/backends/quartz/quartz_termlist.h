/* quartz_termlist.h: Termlists in quartz databases
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

#ifndef OM_HGUARD_QUARTZ_TERMLIST_H
#define OM_HGUARD_QUARTZ_TERMLIST_H

#include <string>

#include <xapian/types.h>
#include "termlist.h"
#include "btree.h"
#include "database.h"

using namespace std;

class QuartzTermListTable : public Btree {
    public:
	/** Create a new table object.
	 *
	 *  This does not create the table on disk - the create() method must
	 *  be called before the table is created on disk
	 *
	 *  This also does not open the table - the open() method must be
	 *  called before use is made of the table.
	 *
	 *  @param path_          - Path at which the table is stored.
	 *  @param readonly_      - whether to open the table for read only
	 *                          access.
	 *  @param blocksize_     - Size of blocks to use.  This parameter is
	 *                          only used when creating the table.
	 */
	QuartzTermListTable(string path_, bool readonly_)
	    : Btree(path_ + "/termlist_", readonly_) { }

	/** Set the entries in the termlist.
	 *
	 *  If the termlist already exists, its contents are replaced.
	 *
	 *  @param store_termfreqs  If true, term frequencies are stored
	 *         in the termlist.  This should only be done with static
	 *         databases (which are probably generated from dumps of
	 *         dynamic databases) - updating cannot be done efficiently
	 *         while storing term frequencies.
	 */
	void set_entries(Xapian::docid did,
		    Xapian::TermIterator t, const Xapian::TermIterator &t_end,
		    quartz_doclen_t doclen, bool store_termfreqs);

	/** Clear the termlist.  After this call, the termlist for the
	 *  specified document ID will not exist.
	 */
	void delete_termlist(Xapian::docid did);
};

/** A termlist in a quartz database.
 */
class QuartzTermList : public TermList {
    private:
        // Prevent copying
        QuartzTermList(const QuartzTermList &);
        QuartzTermList & operator=(const QuartzTermList &);

	/** The database we are searching.  This pointer is held so that the
	 *  database doesn't get deleted before us.
	 */
	Xapian::Internal::RefCntPtr<const Xapian::Database::Internal> this_db;

	Xapian::docid did;

	/** The table holding the termlist.
	 */
	const Btree * table;

	/** The data for the (part of the) termlist currently being read.
	 *
	 *  FIXME: currently, we read the whole termlist as one chunk.
	 */
	string termlist_part;

	/** Position within tag that we're reading from.
	 */
	const char *pos;

	/** End of data within tag.
	 */
	const char *end;

	/** Whether we have moved past the final item yet.
	 */
	bool have_finished;


	/** The length of the document represented by the termlist.
	 */
	quartz_doclen_t doclen;

	/** The size of the termlist.
	 */
	Xapian::termcount termlist_size;

	/** Current termname.
	 */
	string current_tname;

	/** Current wdf.
	 */
	Xapian::termcount current_wdf;

	/** Whether the termlist stores term frequencies (this cannot be
	 *  done in an updatable database, but improves expand efficiency
	 *  considerably).
	 */
	bool has_termfreqs;

	/** Current term frequency.
	 *
	 *  This will have the value 0 if the term frequency is not
	 *  available (in which case it will be looked up in the database
	 *  if requested).
	 */
	mutable Xapian::doccount current_termfreq;

	/** Number of documents in database.
	 */
	Xapian::doccount doccount;

    public:
	/** Open the termlist for the specified document, for reading.
	 */
	QuartzTermList(Xapian::Internal::RefCntPtr<const Xapian::Database::Internal> this_db_,
		       const Btree * table_,
		       Xapian::docid did_,
		       Xapian::doccount doccount_);

	/** Get the length of the document represented by the termlist.
	 *
	 *  FIXME: having a static version of this available would be nice -
	 *  database could then avoid having to create a temporary termlist
	 *  object.
	 */
	quartz_doclen_t get_doclength() const;

	/** Return number of items in termlist.
	 *  (This is actually exact - it may be approximate for combined
	 *  termlists.)
	 */
	Xapian::termcount get_approx_size() const;

	/** Move to next entry.  Must be called before any of the other
	 *  methods, including at_end(), to move onto the first entry.
	 *
	 *  @return This method always returns 0.  Other values would
	 *          represent a termlist which should be used to replace
	 *          this termlist - this never happens.
	 */
	TermList * next();

	/** Check whether the termlist reader has reached the end.
	 */
	bool at_end() const;

	/** Get the current term in the termlist.
	 */
	string get_termname() const;

	/** Get the within document frequency of the current term.
	 */
	Xapian::termcount get_wdf() const;

	/** Get the term frequency of the current term - the number of
	 *  documents containing this term.  For an updateable database, this
	 *  cannot be stored in the termlist, so involves a relatively expensive
	 *  lookup in the postlist table.
	 */
	Xapian::doccount get_termfreq() const;

	/// Collate weighting information for the current term.
	void accumulate_stats(Xapian::Internal::ExpandStats & stats) const;

	Xapian::termcount positionlist_count() const;

	Xapian::PositionIterator positionlist_begin() const;
};

#endif /* OM_HGUARD_QUARTZ_TERMLIST_H */
