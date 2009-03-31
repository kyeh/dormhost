/** \file document.h
 * \brief API for working with documents
 */
/* Copyright 1999,2000,2001 BrightStation PLC
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

#ifndef XAPIAN_INCLUDED_DOCUMENT_H
#define XAPIAN_INCLUDED_DOCUMENT_H

#include <string>

#include <xapian/base.h>
#include <xapian/types.h>
#include <xapian/termiterator.h>
#include <xapian/visibility.h>

namespace Xapian {

class ValueIterator;

/// A document in the database - holds data, values, terms, and postings
class XAPIAN_VISIBILITY_DEFAULT Document {
    public:
	class Internal;
	/// @private @internal Reference counted internals.
	Xapian::Internal::RefCntPtr<Internal> internal;

	/** @private @internal Constructor is only used by internal classes.
	 *
	 *  @param internal_ pointer to internal opaque class
	 */
	explicit Document(Internal *internal_);

	/** Copying is allowed.  The internals are reference counted, so
	 *  copying is cheap.
	 */
	Document(const Document &other);

	/** Assignment is allowed.  The internals are reference counted,
	 *  so assignment is cheap.
	 */
	void operator=(const Document &other);

	/// Make a new empty Document
	Document();

	/// Destructor
	~Document();

	/** Get value by number.
	 *
	 *  Returns an empty string if no value with the given number is present
	 *  in the document.
	 *
	 *  @param valueno The number of the value.
	 */
	std::string get_value(Xapian::valueno valueno) const;

	/** Add a new value.  It will replace any existing value with the
	 *  same number.
	 */
	void add_value(Xapian::valueno valueno, const std::string &value);

	/// Remove any value with the given number.
	void remove_value(Xapian::valueno valueno);

	/// Remove all values associated with the document.
	void clear_values();

	/** Get data stored in the document.
	 *  This is a potentially expensive operation, and shouldn't normally
	 *  be used in a match decider functor.  Put data for use by match
	 *  deciders in a value instead.
	 */
	std::string get_data() const;

	/// Set data stored in the document.
	void set_data(const std::string &data);

	/** Add an occurrence of a term at a particular position.
	 *
	 *  Multiple occurrences of the term at the same position are
	 *  represented only once in the positional information, but do
	 *  increase the wdf.
	 *
	 *  If the term is not already in the document, it will be added to
	 *  it.
	 *
	 *  @param tname     The name of the term.
	 *  @param tpos      The position of the term.
	 *  @param wdfinc    The increment that will be applied to the wdf
	 *                   for this term.
	 */
	void add_posting(const std::string & tname,
			 Xapian::termpos tpos,
			 Xapian::termcount wdfinc = 1);

	/** Add a term to the document, without positional information.
	 *
	 *  Any existing positional information for the term will be left
	 *  unmodified.
	 *
	 *  @param tname     The name of the term.
	 *  @param wdfinc    The increment that will be applied to the wdf
	 *                   for this term.
	 */
	void add_term(const std::string & tname, Xapian::termcount wdfinc = 1);

	/** Remove a posting of a term from the document.
	 *
	 *  Note that the term will still index the document even if all
	 *  occurrences are removed.  To remove a term from a document
	 *  completely, use remove_term().
	 *
	 *  @param tname     The name of the term.
	 *  @param tpos      The position of the term.
	 *  @param wdfdec    The decrement that will be applied to the wdf
	 *                   when removing this posting.  The wdf will not go
	 *                   below the value of 0.
	 *
	 *  @exception Xapian::InvalidArgumentError will be thrown if the term
	 *  is not at the position specified in the position list for this term
	 *  in this document.
	 *
	 *  @exception Xapian::InvalidArgumentError will be thrown if the term
	 *  is not in the document
	 */
	void remove_posting(const std::string & tname,
			    Xapian::termpos tpos,
			    Xapian::termcount wdfdec = 1);

	/** Remove a term and all postings associated with it.
	 *
	 *  @param tname  The name of the term.
	 *
	 *  @exception Xapian::InvalidArgumentError will be thrown if the term
	 *  is not in the document
	 */
	void remove_term(const std::string & tname);

	/// Remove all terms (and postings) from the document.
	void clear_terms();

	/** The length of the termlist - i.e. the number of different terms
	 *  which index this document.
	 */
	Xapian::termcount termlist_count() const;

	/// Iterator for the terms in this document.
	TermIterator termlist_begin() const;

	/// Equivalent end iterator for termlist_begin().
	TermIterator termlist_end() const {
	    return TermIterator(NULL);
	}

	/// Count the values in this document.
	Xapian::termcount values_count() const;

	/// Iterator for the values in this document.
	ValueIterator values_begin() const;

	/// Equivalent end iterator for values_begin().
	ValueIterator values_end() const;

	/** Get the document id which is associated with this document (if any).
	 *
	 *  NB If multiple databases are being searched together, then this
	 *  will be the document id in the individual database, not the merged
	 *  database!
	 *
	 *  @return If this document came from a database, return the document
	 *	    id in that database.  Otherwise, return 0.
	 */
	docid get_docid() const;

	/// Return a string describing this object.
	std::string get_description() const;
};

}

#endif // XAPIAN_INCLUDED_DOCUMENT_H
