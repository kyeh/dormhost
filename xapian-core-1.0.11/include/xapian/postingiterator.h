/** \file postingiterator.h
 * \brief Classes for iterating through posting lists
 */
/* Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2003,2004,2005,2007 Olly Betts
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

#ifndef XAPIAN_INCLUDED_POSTINGITERATOR_H
#define XAPIAN_INCLUDED_POSTINGITERATOR_H

#include <iterator>
#include <string>

#include <xapian/base.h>
#include <xapian/types.h>
#include <xapian/positioniterator.h>
#include <xapian/visibility.h>

namespace Xapian {

class Database;

/** @internal A wrapper class for a docid which returns the docid if
 *  dereferenced with *.  We need this to implement input_iterator semantics.
 */
class DocIDWrapper {
    private:
	docid did;
    public:
	explicit DocIDWrapper(docid did_) : did(did_) { }
	docid operator*() const { return did; }
};

/** An iterator pointing to items in a list of postings.
 */
class XAPIAN_VISIBILITY_DEFAULT PostingIterator {
    public:
	class Internal;
	/// @private @internal Reference counted internals.
	Xapian::Internal::RefCntPtr<Internal> internal;

    private:
	friend class Database; // So Database can construct us

	explicit PostingIterator(Internal *internal_);

    public:
        friend bool operator==(const PostingIterator &a,
			       const PostingIterator &b);

	/// Default constructor - for declaring an uninitialised iterator
	PostingIterator();

	/// Destructor
        ~PostingIterator();

        /** Copying is allowed.  The internals are reference counted, so
	 *  copying is also cheap.
	 */
	PostingIterator(const PostingIterator &other);

        /** Assignment is allowed.  The internals are reference counted,
	 *  so assignment is also cheap.
	 */
	void operator=(const PostingIterator &other);

	PostingIterator & operator++();

	DocIDWrapper operator++(int) {
	    Xapian::docid tmp = **this;
	    operator++();
	    return DocIDWrapper(tmp);
	}

	/** Skip the iterator to document did, or the first document after did
	 *  if did isn't in the list of documents being iterated.
	 */
	void skip_to(Xapian::docid did);

// Get the weight of the posting at the current position: will
// need to set a weight object for this to work.
// Xapian::weight get_weight() const;

	/// Get the document id at the current position in the postlist.
	Xapian::docid operator *() const;

	/** Get the length of the document at the current position in the
	 *  postlist.
	 *
	 *  This information may be stored in the postlist, in which case
	 *  this lookup should be extremely fast (indeed, not require further
	 *  disk access).  If the information is not present in the postlist,
	 *  it will be retrieved from the database, at a greater performance
	 *  cost.
	 */
	Xapian::doclength get_doclength() const;

	/** Get the within document frequency of the document at the
	 *  current position in the postlist.
	 */
        Xapian::termcount get_wdf() const;

	/** Return PositionIterator pointing to start of positionlist for
	 *  current document.
	 */
	PositionIterator positionlist_begin() const;

	/** Return PositionIterator pointing to end of positionlist for
	 *  current document.
	 */
	PositionIterator positionlist_end() const {
	    return PositionIterator(NULL);
	}

	// Don't expose these methods here.  A container iterator doesn't
	// provide a method to find the size of the container...
	// Xapian::doccount get_termfreq() const;
	// Xapian::termcount get_collection_freq() const;

	/// Return a string describing this object.
	std::string get_description() const;

	/// Allow use as an STL iterator
	//@{
	typedef std::input_iterator_tag iterator_category;
	typedef Xapian::docid value_type;
	typedef Xapian::doccount_diff difference_type;
	typedef Xapian::docid * pointer;
	typedef Xapian::docid & reference;
	//@}
};

/// Test equality of two PostingIterators
inline bool operator==(const PostingIterator &a, const PostingIterator &b)
{
    return (a.internal.get() == b.internal.get());
}

/// Test inequality of two PostingIterators
inline bool operator!=(const PostingIterator &a, const PostingIterator &b)
{
    return !(a == b);
}

}

#endif /* XAPIAN_INCLUDED_POSTINGITERATOR_H */
