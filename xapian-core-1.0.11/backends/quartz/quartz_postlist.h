/* quartz_postlist.h: Postlists in quartz databases
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
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

#ifndef OM_HGUARD_QUARTZ_POSTLIST_H
#define OM_HGUARD_QUARTZ_POSTLIST_H

#include <map>
#include <string>

#include "autoptr.h"
#include "leafpostlist.h"
#include <xapian/database.h>
#include <xapian/visibility.h>
#include "omassert.h"
#include "quartz_types.h"
#include "quartz_positionlist.h"

using namespace std;

class Bcursor;
class QuartzDatabase;

class QPostlistChunkReader;
class QPostlistChunkWriter;

class QuartzPostListTable : public Btree {
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
	QuartzPostListTable(string path_, bool readonly_)
	    : Btree(path_ + "/postlist_", readonly_) { }

	/// Merge added, removed, and changed entries.
	void merge_changes(
	    const map<string, map<Xapian::docid, pair<char, Xapian::termcount> > > & mod_plists,
	    const map<Xapian::docid, Xapian::termcount> & doclens,
	    const map<string, pair<Xapian::termcount_diff, Xapian::termcount_diff> > & freq_deltas);

	Xapian::docid get_chunk(const string &tname,
		Xapian::docid did, bool adding,
		QPostlistChunkReader ** from, QPostlistChunkWriter **to);

	/** Returns number of docs indexed by @a term.
	 *
	 *  This is the length of the postlist.
	 */
	Xapian::doccount get_termfreq(const std::string & term) const;

	/** Returns the number of occurrences of @a term in the database.
	 *
	 *  This is the sum of the wdfs in the postlist.
	 */
	Xapian::termcount get_collection_freq(const std::string & term) const;
};

/** A postlist in a quartz database.
 */
class XAPIAN_VISIBILITY_DEFAULT QuartzPostList : public LeafPostList {
    private:
	/** The database we are searching.  This pointer is held so that the
	 *  database doesn't get deleted before us.
	 */
	Xapian::Internal::RefCntPtr<const Xapian::Database::Internal> this_db;

	/// The table containing the postlist.
	const Btree * table;

	/// The table containing positionlists.
	const Btree * positiontable;

	/// The termname for this postlist.
	string tname;

	/// Cursor pointing to current chunk of postlist.
	AutoPtr<Bcursor> cursor;

	/// True if this is the last chunk.
	bool is_last_chunk;

	/// The first document id in this chunk.
	Xapian::docid first_did_in_chunk;

	/// The last document id in this chunk.
	Xapian::docid last_did_in_chunk;

	/// Position of iteration through current chunk.
	const char * pos;

	/// Pointer to byte after end of current chunk.
	const char * end;

	/// Document id we're currently at.
	Xapian::docid did;

	/// The (absolute) length of the current document.
	quartz_doclen_t doclength;

	/// The wdf of the current document.
	Xapian::termcount wdf;

	/// Whether we've run off the end of the list yet.
	bool is_at_end;

	/// Whether we've started reading the list yet.
	bool have_started;

	/// The number of entries in the posting list.
	Xapian::doccount number_of_entries;

	/// The position list object for this posting list.
	QuartzPositionList positionlist;

	/// Copying is not allowed.
	QuartzPostList(const QuartzPostList &);

	/// Assignment is not allowed.
	void operator=(const QuartzPostList &);

	/** Move to the next item in the chunk, if possible.
	 *  If already at the end of the chunk, returns false.
	 */
	bool next_in_chunk();

	/** Move to the next chunk.
	 *
	 *  If there are no more chunks in this postlist, this will set
	 *  is_at_end to true.
	 */
	void next_chunk();

	/** Return true if the given document ID lies in the range covered
	 *  by the current chunk.  This does not say whether the document ID
	 *  is actually present.  It will return false if the document ID
	 *  is greater than the last document ID in the chunk, even if it is
	 *  less than the first document ID in the next chunk: it is possible
	 *  for no chunk to contain a particular document ID.
	 */
	bool current_chunk_contains(Xapian::docid desired_did);

	/** Move to chunk containing the specified document ID.
	 *
	 *  This moves to the chunk whose starting document ID is
	 *  <= desired_did, but such that the next chunk's starting
	 *  document ID is > desired_did.
	 *
	 *  It is thus possible that current_chunk_contains(desired_did)
	 *  will return false after this call, since the document ID
	 *  might lie after the end of this chunk, but before the start
	 *  of the next chunk.
	 */
	void move_to_chunk_containing(Xapian::docid desired_did);

	/** Scan forward in the current chunk for the specified document ID.
	 *
	 *  This is particularly efficient if the desired document ID is
	 *  greater than the last in the chunk - it then skips straight
	 *  to the end.
	 *
	 *  @return true if we moved to a valid document,
	 *	    false if we reached the end of the chunk.
	 */
	bool move_forward_in_chunk_to_at_least(Xapian::docid desired_did);

    public:
	/// Default constructor.
	QuartzPostList(Xapian::Internal::RefCntPtr<const Xapian::Database::Internal> this_db_,
		       const Btree * table_,
		       const Btree * positiontable_,
		       const string & tname);

	/// Destructor.
	~QuartzPostList();

	/** Returns number of docs indexed by this term.
	 *
	 *  This is the length of the postlist.
	 */
	Xapian::doccount get_termfreq() const { return number_of_entries; }

	/// Returns the current docid.
	Xapian::docid get_docid() const { Assert(have_started); return did; }

	/// Returns the length of current document.
	Xapian::doclength get_doclength() const {
	    Assert(have_started);
	    return static_cast<Xapian::doclength>(doclength);
	}

	/** Returns the Within Document Frequency of the term in the current
	 *  document.
	 */
	Xapian::termcount get_wdf() const { Assert(have_started); return wdf; }

	/** Get the list of positions of the term in the current document.
	 */
	PositionList *read_position_list();

	/** Get the list of positions of the term in the current document.
	 */
	PositionList * open_position_list() const;

	/// Move to the next document.
	PostList * next(Xapian::weight w_min);

	/// Skip to next document with docid >= docid.
	PostList * skip_to(Xapian::docid desired_did, Xapian::weight w_min);

	/// Return true if and only if we're off the end of the list.
	bool at_end() const { return is_at_end; }

	/// Get a description of the document.
	std::string get_description() const;

	/// Read the number of entries and the collection frequency.
	static void read_number_of_entries(const char ** posptr,
					   const char * end,
					   Xapian::termcount * number_of_entries_ptr,
					   Xapian::termcount * collection_freq_ptr);
};

#endif /* OM_HGUARD_QUARTZ_POSTLIST_H */
