/** \file termiterator.h
 * \brief Classes for iterating through term lists
 */
/* Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2003,2004,2005,2006,2007 Olly Betts
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

#ifndef XAPIAN_INCLUDED_TERMITERATOR_H
#define XAPIAN_INCLUDED_TERMITERATOR_H

#include <iterator>
#include <string>

#include <xapian/base.h>
#include <xapian/types.h>
#include <xapian/positioniterator.h>
#include <xapian/visibility.h>

namespace Xapian {

class Database;

/** @internal A wrapper class for a termname which returns the termname if
 *  dereferenced with *.  We need this to implement input_iterator semantics.
 */
class TermNameWrapper {
    private:
	std::string tname;
    public:
	explicit TermNameWrapper(const std::string & tname_) : tname(tname_) { }
	const std::string & operator*() const { return tname; }
};

/** An iterator pointing to items in a list of terms.
 */
class XAPIAN_VISIBILITY_DEFAULT TermIterator {
    public:
	class Internal;
	/// @internal Reference counted internals.
	Xapian::Internal::RefCntPtr<Internal> internal;

	/// @internal Reference counted internals.
	explicit TermIterator(Internal *internal_);

	/// Default constructor - for declaring an uninitialised iterator.
	TermIterator();

	/// Destructor.
	~TermIterator();

	/** Copying is allowed.  The internals are reference counted, so
	 *  copying is also cheap.
	 */
	TermIterator(const TermIterator &other);

	/** Assignment is allowed.  The internals are reference counted,
	 *  so assignment is also cheap.
	 */
	void operator=(const TermIterator &other);

	/// Return the current term.
	std::string operator *() const;

	TermIterator & operator++();

	TermNameWrapper operator++(int) {
	    std::string tmp = **this;
	    operator++();
	    return TermNameWrapper(tmp);
	}

	/** Skip the iterator to term tname, or the first term after tname
	 *  if tname isn't in the list of terms being iterated.
	 */
	void skip_to(const std::string & tname);

	/** Return the wdf of the current term (if meaningful).
	 *
	 *  The wdf (within document frequency) is the number of occurences
	 *  of a term in a particular document.
	 */
	Xapian::termcount get_wdf() const;

	/** Return the term frequency of the current term (if meaningful).
	 *
	 *  The term frequency is the number of documents which a term indexes.
	 */
	Xapian::doccount get_termfreq() const;

	/** Return length of positionlist for current term.
	 */
	Xapian::termcount positionlist_count() const;

	/** Return PositionIterator pointing to start of positionlist for
	 *  current term.
	 */
	PositionIterator positionlist_begin() const;

	/** Return PositionIterator pointing to end of positionlist for
	 *  current term.
	 */
	PositionIterator positionlist_end() const {
	    return PositionIterator(NULL);
	}

	/// Return a string describing this object.
	std::string get_description() const;

	/// Allow use as an STL iterator
	//@{
	typedef std::input_iterator_tag iterator_category;
	typedef std::string value_type;
	typedef Xapian::termcount_diff difference_type;
	typedef std::string * pointer;
	typedef std::string & reference;
	//@}
};

inline bool
operator==(const TermIterator &a, const TermIterator &b)
{
    return (a.internal.get() == b.internal.get());
}

inline bool
operator!=(const TermIterator &a, const TermIterator &b)
{
    return !(a == b);
}

}

#endif /* XAPIAN_INCLUDED_TERMITERATOR_H */
