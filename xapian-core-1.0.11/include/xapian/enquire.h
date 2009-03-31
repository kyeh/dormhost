/** \file enquire.h
 * \brief API for running queries
 */
/* Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2001,2002 Ananova Ltd
 * Copyright 2002,2003,2004,2005,2006,2007,2009 Olly Betts
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

#ifndef XAPIAN_INCLUDED_ENQUIRE_H
#define XAPIAN_INCLUDED_ENQUIRE_H

#include <string>

#include <xapian/base.h>
#include <xapian/deprecated.h>
#include <xapian/sorter.h>
#include <xapian/types.h>
#include <xapian/termiterator.h>
#include <xapian/visibility.h>

namespace Xapian {

class Database;
class Document;
class ErrorHandler;
class ExpandDecider;
class MSetIterator;
class Query;
class Weight;

/** A match set (MSet).
 *  This class represents (a portion of) the results of a query.
 */
class XAPIAN_VISIBILITY_DEFAULT MSet {
    public:
	class Internal;
	/// @internal Reference counted internals.
	Xapian::Internal::RefCntPtr<Internal> internal;

	/// @internal Constructor for internal use.
	explicit MSet(MSet::Internal * internal_);

	/// Create an empty Xapian::MSet.
	MSet();

	/// Destroy a Xapian::MSet.
	~MSet();

	/// Copying is allowed (and is cheap).
	MSet(const MSet & other);

	/// Assignment is allowed (and is cheap).
	void operator=(const MSet &other);

	/** Fetch the document info for a set of items in the MSet.
	 *
	 *  This method causes the documents in the range specified by the
	 *  iterators to be fetched from the database, and cached in the
	 *  Xapian::MSet object.  This has little effect when performing a
	 *  search across a local database, but will greatly speed up
	 *  subsequent access to the document contents when the documents are
	 *  stored in a remote database.
	 *
	 *  The iterators must be over this Xapian::MSet - undefined behaviour
	 *  will result otherwise.
	 *
	 *  @param begin   MSetIterator for first item to fetch.
	 *  @param end     MSetIterator for item after last item to fetch.
	 */
	void fetch(const MSetIterator &begin, const MSetIterator &end) const;

	/** Fetch the single item specified.
	 */
	void fetch(const MSetIterator &item) const;

	/** Fetch all the items in the MSet.
	 */
	void fetch() const;

	/** This converts the weight supplied to a percentage score.
	 *  The return value will be in the range 0 to 100, and will be 0 if
	 *  and only if the item did not match the query at all.
	 */
	Xapian::percent convert_to_percent(Xapian::weight wt) const;

	/// Return the percentage score for a particular item.
	Xapian::percent convert_to_percent(const MSetIterator &it) const;

	/** Return the term frequency of the given query term.
	 *
	 *  @param tname The term to look for.
	 *
	 *  @exception Xapian::InvalidArgumentError is thrown if the term was
	 *	       not in the query.
	 */
	Xapian::doccount get_termfreq(const std::string &tname) const;

	/** Return the term weight of the given query term.
	 *
	 *  @param tname The term to look for.
	 *
	 *  @exception  Xapian::InvalidArgumentError is thrown if the term was
	 *		not in the query.
	 */
	Xapian::weight get_termweight(const std::string &tname) const;

	/** The index of the first item in the result which was put into the
	 *  MSet.
	 *
	 *  This corresponds to the parameter "first" specified in
	 *  Xapian::Enquire::get_mset().  A value of 0 corresponds to the
	 *  highest result being the first item in the MSet.
	 */
	Xapian::doccount get_firstitem() const;

	/** A lower bound on the number of documents in the database which
	 *  match the query.
	 *
	 *  This figure takes into account collapsing of duplicates,
	 *  and weighting cutoff values.
	 *
	 *  This number is usually considerably less than the actual number
	 *  of documents which match the query.
	 */
	Xapian::doccount get_matches_lower_bound() const;

	/** An estimate for the number of documents in the database which
	 *  match the query.
	 *
	 *  This figure takes into account collapsing of duplicates,
	 *  and weighting cutoff values.
	 *
	 *  This value is returned because there is sometimes a request to
	 *  display such information.  However, our experience is that
	 *  presenting this value to users causes them to worry about the
	 *  large number of results, rather than how useful those at the top
	 *  of the result set are, and is thus undesirable.
	 */
	Xapian::doccount get_matches_estimated() const;

	/** An upper bound on the number of documents in the database which
	 *  match the query.
	 *
	 *  This figure takes into account collapsing of duplicates,
	 *  and weighting cutoff values.
	 *
	 *  This number is usually considerably greater than the actual
	 *  number of documents which match the query.
	 */
	Xapian::doccount get_matches_upper_bound() const;

	/** The maximum possible weight in the MSet.
	 *
	 *  This weight is likely not to be attained in the set of results,
	 *  but represents an upper bound on the weight which a document
	 *  could attain for the given query.
	 */
	Xapian::weight get_max_possible() const;

	/** The greatest weight which is attained by any document in the
	 *  database.
	 *
	 *  If firstitem == 0 and the primary ordering is by relevance, this is
	 *  the weight of the first entry in the MSet. 
	 *
	 *  If no documents are found by the query, this will be 0.
	 *
	 *  Note that calculation of max_attained requires calculation
	 *  of at least one result item - therefore, if no items were
	 *  requested when the query was performed (by specifying
	 *  maxitems = 0 in Xapian::Enquire::get_mset()), this value will be 0.
	 */
	Xapian::weight get_max_attained() const;

	/** The number of items in this MSet */
	Xapian::doccount size() const;

	/** Required to allow use as an STL container. */
	Xapian::doccount max_size() const { return size(); }

	/** Test if this MSet is empty */
	bool empty() const;

	/** Swap the MSet we point to with another */
	void swap(MSet & other);

	/** Iterator for the terms in this MSet */
	MSetIterator begin() const;

	/** End iterator corresponding to begin() */
	MSetIterator end() const;

	/** Iterator pointing to the last element of this MSet */
	MSetIterator back() const;

	/** This returns the document at position i in this MSet object.
	 *
	 *  Note that this is not the same as the document at rank i in the
	 *  query, unless the "first" parameter to Xapian::Enquire::get_mset
	 *  was 0.  Rather, it is the document at rank i + first.
	 *
	 *  In other words, the offset is into the documents represented by
	 *  this object, not into the set of documents matching the query.
	 */
	MSetIterator operator[](Xapian::doccount i) const;

	/// Allow use as an STL container
	//@{
	typedef MSetIterator value_type; // FIXME: not assignable...
	typedef MSetIterator iterator;
	typedef MSetIterator const_iterator;
	typedef MSetIterator & reference; // Hmm
	typedef MSetIterator & const_reference;
	typedef MSetIterator * pointer; // Hmm
	typedef Xapian::doccount_diff difference_type;
	typedef Xapian::doccount size_type;
	//@}

	/// Return a string describing this object.
	std::string get_description() const;
};

/** An iterator pointing to items in an MSet.
 *  This is used for access to individual results of a match.
 */
class XAPIAN_VISIBILITY_DEFAULT MSetIterator {
    private:
	friend class MSet;
	friend bool operator==(const MSetIterator &a, const MSetIterator &b);
	friend bool operator!=(const MSetIterator &a, const MSetIterator &b);

	MSetIterator(Xapian::doccount index_, const MSet & mset_)
	    : index(index_), mset(mset_) { }

	Xapian::doccount index;
	MSet mset;

    public:
	/** Create an uninitialised iterator; this cannot be used, but is
	 *  convenient syntactically.
	 */
	MSetIterator() : index(0), mset() { }

	~MSetIterator() { }

	/// Copying is allowed (and is cheap).
	MSetIterator(const MSetIterator &other) {
	    index = other.index;
	    mset = other.mset;
	}

	/// Assignment is allowed (and is cheap).
	void operator=(const MSetIterator &other) {
	    index = other.index;
	    mset = other.mset;
	}

	/// Advance the iterator.
	MSetIterator & operator++() {
	    ++index;
	    return *this;
	}

	/// Advance the iterator (postfix variant).
	MSetIterator operator++(int) {
	    MSetIterator tmp = *this;
	    ++index;
	    return tmp;
	}

	/// Decrement the iterator.
	MSetIterator & operator--() {
	    --index;
	    return *this;
	}

	/// Decrement the iterator (postfix variant).
	MSetIterator operator--(int) {
	    MSetIterator tmp = *this;
	    --index;
	    return tmp;
	}

	/// Get the document ID for the current position.
	Xapian::docid operator*() const;

	/** Get a Xapian::Document object for the current position.
	 *
	 *  This method returns a Xapian::Document object which provides the
	 *  information about the document pointed to by the MSetIterator.
	 *
	 *  If the underlying database has suitable support, using this call
	 *  (rather than asking the database for a document based on its
	 *  document ID) will enable the system to ensure that the correct
	 *  data is returned, and that the document has not been deleted
	 *  or changed since the query was performed.
	 *
	 *  @return     A Xapian::Document object containing the document data.
	 *
	 *  @exception Xapian::DocNotFoundError The document specified could not
	 *	       be found in the database.
	 */
	Xapian::Document get_document() const;

	/** Get the rank of the document at the current position.
	 *
	 *  The rank is the position that this document is at in the ordered
	 *  list of results of the query.  The result is 0-based - i.e. the
	 *  top-ranked document has a rank of 0.
	 */
	Xapian::doccount get_rank() const {
	    return mset.get_firstitem() + index;
	}

	/// Get the weight of the document at the current position
	Xapian::weight get_weight() const;

	/** Get the collapse key for this document.
	 */
	std::string get_collapse_key() const;

	/** Get an estimate of the number of documents that have been collapsed
	 *  into this one.
	 *
	 *  The estimate will always be less than or equal to the actual
	 *  number of other documents satisfying the match criteria with the
	 *  same collapse key as this document.
	 *
	 *  This method may return 0 even though there are other documents with
	 *  the same collapse key which satisfying the match criteria.  However
	 *  if this method returns non-zero, there definitely are other such
	 *  documents.  So this method may be used to inform the user that
	 *  there are "at least N other matches in this group", or to control
	 *  whether to offer a "show other documents in this group" feature
	 *  (but note that it may not offer it in every case where it would
	 *  show other documents).
	 */
	Xapian::doccount get_collapse_count() const;

	/** This returns the weight of the document as a percentage score.
	 *
	 *  The return value will be an integer in the range 0 to 100:  0
	 *  meaning that the item did not match the query at all.
	 *
	 *  The intention is that the highest weighted document will get 100
	 *  if it matches all the weight-contributing terms in the query.
	 *  However, currently it may get a lower percentage score if you
	 *  use a MatchDecider and the sorting is primarily by value.
	 *  In this case, the percentage for a particular document may vary
	 *  depending on the first, max_size, and checkatleast parameters
	 *  passed to Enquire::get_mset() (this bug is hard to fix without
	 *  having to apply the MatchDecider to potentially many more
	 *  documents, which is potentially costly).
	 */
	Xapian::percent get_percent() const;

	/// Return a string describing this object.
	std::string get_description() const;

	/// Allow use as an STL iterator
	//@{
	typedef std::bidirectional_iterator_tag iterator_category; // FIXME: could enhance to be a randomaccess_iterator
	typedef Xapian::docid value_type;
	typedef Xapian::doccount_diff difference_type;
	typedef Xapian::docid * pointer;
	typedef Xapian::docid & reference;
	//@}
};

inline bool operator==(const MSetIterator &a, const MSetIterator &b)
{
    return (a.index == b.index);
}

inline bool operator!=(const MSetIterator &a, const MSetIterator &b)
{
    return (a.index != b.index);
}

class ESetIterator;

/** Class representing an ordered set of expand terms (an ESet).
 *  This set represents the results of an expand operation, which is
 *  performed by Xapian::Enquire::get_eset().
 */
class XAPIAN_VISIBILITY_DEFAULT ESet {
    public:
	class Internal;
	/// @internal Reference counted internals.
	Xapian::Internal::RefCntPtr<Internal> internal;

	/// Construct an empty ESet
	ESet();

	/// Destructor.
	~ESet();

	/// Copying is allowed (and is cheap).
	ESet(const ESet & other);

	/// Assignment is allowed (and is cheap).
	void operator=(const ESet &other);

	/** A lower bound on the number of terms which are in the full
	 *  set of results of the expand.  This will be greater than or
	 *  equal to size()
	 */
	Xapian::termcount get_ebound() const;

	/** The number of terms in this E-Set */
	Xapian::termcount size() const;

	/** Required to allow use as an STL container. */
	Xapian::termcount max_size() const { return size(); }

	/** Test if this E-Set is empty */
	bool empty() const;

	/** Swap the E-Set we point to with another */
	void swap(ESet & other);

	/** Iterator for the terms in this E-Set */
	ESetIterator begin() const;

	/** End iterator corresponding to begin() */
	ESetIterator end() const;

	/** Iterator pointing to the last element of this E-Set */
	ESetIterator back() const;

	/** This returns the term at position i in this E-Set.  */
	ESetIterator operator[](Xapian::termcount i) const;

	/// Return a string describing this object.
	std::string get_description() const;
};

/** Iterate through terms in the ESet */
class XAPIAN_VISIBILITY_DEFAULT ESetIterator {
    private:
	friend class ESet;
	friend bool operator==(const ESetIterator &a, const ESetIterator &b);
	friend bool operator!=(const ESetIterator &a, const ESetIterator &b);

	ESetIterator(Xapian::termcount index_, const ESet & eset_)
	    : index(index_), eset(eset_) { }

	Xapian::termcount index;
	ESet eset;

    public:
	/** Create an uninitialised iterator; this cannot be used, but is
	 *  convenient syntactically.
	 */
	ESetIterator() : index(0), eset() { }

	~ESetIterator() { }

	/// Copying is allowed (and is cheap).
	ESetIterator(const ESetIterator &other) {
	    index = other.index;
	    eset = other.eset;
	}

	/// Assignment is allowed (and is cheap).
	void operator=(const ESetIterator &other) {
	    index = other.index;
	    eset = other.eset;
	}

	/// Advance the iterator.
	ESetIterator & operator++() {
	    ++index;
	    return *this;
	}

	/// Advance the iterator (postfix variant).
	ESetIterator operator++(int) {
	    ESetIterator tmp = *this;
	    ++index;
	    return tmp;
	}

	/// Decrement the iterator.
	ESetIterator & operator--() {
	    --index;
	    return *this;
	}

	/// Decrement the iterator (postfix variant).
	ESetIterator operator--(int) {
	    ESetIterator tmp = *this;
	    --index;
	    return tmp;
	}

	/// Get the term for the current position
	const std::string & operator *() const;

	/// Get the weight of the term at the current position
	Xapian::weight get_weight() const;

	/// Return a string describing this object.
	std::string get_description() const;

	/// Allow use as an STL iterator
	//@{
	typedef std::bidirectional_iterator_tag iterator_category; // FIXME: go for randomaccess_iterator!
	typedef std::string value_type;
	typedef Xapian::termcount_diff difference_type;
	typedef std::string * pointer;
	typedef std::string & reference;
	//@}
};

inline bool operator==(const ESetIterator &a, const ESetIterator &b)
{
    return (a.index == b.index);
}

inline bool operator!=(const ESetIterator &a, const ESetIterator &b)
{
    return (a.index != b.index);
}

/** A relevance set (R-Set).
 *  This is the set of documents which are marked as relevant, for use
 *  in modifying the term weights, and in performing query expansion.
 */
class XAPIAN_VISIBILITY_DEFAULT RSet {
    public:
	/// Class holding details of RSet
	class Internal;

	/// @internal Reference counted internals.
	Xapian::Internal::RefCntPtr<Internal> internal;

	/// Copy constructor
	RSet(const RSet &rset);

	/// Assignment operator
	void operator=(const RSet &rset);

	/// Default constructor
	RSet();

	/// Destructor
	~RSet();

	/** The number of documents in this R-Set */
	Xapian::doccount size() const;

	/** Test if this R-Set is empty */
	bool empty() const;

	/// Add a document to the relevance set.
	void add_document(Xapian::docid did);

	/// Add a document to the relevance set.
	void add_document(const Xapian::MSetIterator & i) { add_document(*i); }

	/// Remove a document from the relevance set.
	void remove_document(Xapian::docid did);

	/// Remove a document from the relevance set.
	void remove_document(const Xapian::MSetIterator & i) { remove_document(*i); }

	/// Test if a given document in the relevance set.
	bool contains(Xapian::docid did) const;

	/// Test if a given document in the relevance set.
	bool contains(const Xapian::MSetIterator & i) const { return contains(*i); }

	/// Return a string describing this object.
	std::string get_description() const;
};

/** Base class for matcher decision functor.
 */
class XAPIAN_VISIBILITY_DEFAULT MatchDecider {
    public:
	/** Decide whether we want this document to be in the MSet.
	 *
	 *  Return true if the document is acceptable, or false if the document
	 *  should be excluded from the MSet.
	 */
	virtual bool operator()(const Xapian::Document &doc) const = 0;

	/// Destructor.
	virtual ~MatchDecider();
};

/** This class provides an interface to the information retrieval
 *  system for the purpose of searching.
 *
 *  Databases are usually opened lazily, so exceptions may not be
 *  thrown where you would expect them to be.  You should catch
 *  Xapian::Error exceptions when calling any method in Xapian::Enquire.
 *
 *  @exception Xapian::InvalidArgumentError will be thrown if an invalid
 *  argument is supplied, for example, an unknown database type.
 */
class XAPIAN_VISIBILITY_DEFAULT Enquire {
    public:
	/// Copying is allowed (and is cheap).
	Enquire(const Enquire & other);

	/// Assignment is allowed (and is cheap).
	void operator=(const Enquire & other);

	class Internal;
	/// @internal Reference counted internals.
	Xapian::Internal::RefCntPtr<Internal> internal;

	/** Create a Xapian::Enquire object.
	 *
	 *  This specification cannot be changed once the Xapian::Enquire is
	 *  opened: you must create a new Xapian::Enquire object to access a
	 *  different database, or set of databases.
	 *
	 *  The database supplied must have been initialised (ie, must not be
	 *  the result of calling the Database::Database() constructor).  If
	 *  you need to handle a situation where you have no index gracefully,
	 *  a database created with InMemory::open() can be passed here,
	 *  which represents a completely empty database.
	 *
	 *  @param database Specification of the database or databases to
	 *	   use.
	 *  @param errorhandler_  A pointer to the error handler to use.
	 *	   Ownership of the object pointed to is not assumed by the
	 *	   Xapian::Enquire object - the user should delete the
	 *	   Xapian::ErrorHandler object after the Xapian::Enquire object
	 *	   is deleted.  To use no error handler, this parameter
	 *	   should be 0.
	 *
	 *  @exception Xapian::InvalidArgumentError will be thrown if an
	 *  initialised Database object is supplied.
	 */
	explicit Enquire(const Database &database, ErrorHandler * errorhandler_ = 0);

	/** Close the Xapian::Enquire object.
	 */
	~Enquire();

	/** Set the query to run.
	 *
	 *  @param query  the new query to run.
	 *  @param qlen   the query length to use in weight calculations -
	 *	by default the sum of the wqf of all terms is used.
	 */
	void set_query(const Xapian::Query & query, Xapian::termcount qlen = 0);

	/** Get the query which has been set.
	 *  This is only valid after set_query() has been called.
	 *
	 *  @exception Xapian::InvalidArgumentError will be thrown if query has
	 *	       not yet been set.
	 */
	const Xapian::Query & get_query() const;

	/** Set the weighting scheme to use for queries.
	 *
	 *  @param weight_  the new weighting scheme.  If no weighting scheme
	 *		    is specified, the default is BM25 with the
	 *		    default parameters.
	 */
	void set_weighting_scheme(const Weight &weight_);

	/** Set the collapse key to use for queries.
	 *
	 *  @param collapse_key  value number to collapse on - at most one MSet
	 *	entry with each particular value will be returned.
	 *
	 *	The entry returned will be the best entry with that particular
	 *	value (highest weight or highest sorting key).
	 *
	 *	An example use might be to create a value for each document
	 *	containing an MD5 hash of the document contents.  Then
	 *	duplicate documents from different sources can be eliminated at
	 *	search time (it's better to eliminate duplicates at index time,
	 *	but this may not be always be possible - for example the search
	 *	may be over more than one Xapian database).
	 *
	 *	Another use is to group matches in a particular category (e.g.
	 *	you might collapse a mailing list search on the Subject: so
	 *	that there's only one result per discussion thread).  In this
	 *	case you can use get_collapse_count() to give the user some
	 *	idea how many other results there are.  And if you index the
	 *	Subject: as a boolean term as well as putting it in a value,
	 *	you can offer a link to a non-collapsed search restricted to
	 *	that thread using a boolean filter.
	 *
	 *	(default is Xapian::BAD_VALUENO which means no collapsing).
	 */
	void set_collapse_key(Xapian::valueno collapse_key);

	typedef enum {
	    ASCENDING = 1,
	    DESCENDING = 0,
	    DONT_CARE = 2
	} docid_order;

	/** Set the direction in which documents are ordered by document id
	 *  in the returned MSet.
	 *
	 *  This order only has an effect on documents which would otherwise
	 *  have equal rank.  For a weighted probabilistic match with no sort
	 *  value, this means documents with equal weight.  For a boolean match,
	 *  with no sort value, this means all documents.  And if a sort value
	 *  is used, this means documents with equal sort value (and also equal
	 *  weight if ordering on relevance after the sort).
	 *
	 * @param order  This can be:
	 * - Xapian::Enquire::ASCENDING
	 *	docids sort in ascending order (default)
	 * - Xapian::Enquire::DESCENDING
	 *	docids sort in descending order
	 * - Xapian::Enquire::DONT_CARE
	 *      docids sort in whatever order is most efficient for the backend
	 *
	 *  Note: If you add documents in strict date order, then a boolean
	 *  search - i.e. set_weighting_scheme(Xapian::BoolWeight()) - with
	 *  set_docid_order(Xapian::Enquire::DESCENDING) is a very efficient
	 *  way to perform "sort by date, newest first".
	 */
	void set_docid_order(docid_order order);

	/** Set the percentage and/or weight cutoffs.
	 *
	 * @param percent_cutoff Minimum percentage score for returned
	 *	documents. If a document has a lower percentage score than this,
	 *	it will not appear in the MSet.  If your intention is to return
	 *	only matches which contain all the terms in the query, then
	 *	it's more efficient to use Xapian::Query::OP_AND instead of
	 *	Xapian::Query::OP_OR in the query than to use set_cutoff(100).
	 *	(default 0 => no percentage cut-off).
	 * @param weight_cutoff Minimum weight for a document to be returned.
	 *	If a document has a lower score that this, it will not appear
	 *	in the MSet.  It is usually only possible to choose an
	 *	appropriate weight for cutoff based on the results of a
	 *	previous run of the same query; this is thus mainly useful for
	 *	alerting operations.  The other potential use is with a user
	 *	specified weighting scheme.
	 *	(default 0 => no weight cut-off).
	 */
	void set_cutoff(Xapian::percent percent_cutoff, Xapian::weight weight_cutoff = 0);

	/** Set the sorting to be by relevance only.
	 *
	 *  This is the default.
         */
	void set_sort_by_relevance();

	/** Set the sorting to be by value only.
	 *
	 *  NB sorting of values uses a string comparison, so you'll need to
	 *  store numbers padded with leading zeros or spaces, or with the
	 *  number of digits prepended.
	 *
	 * @param sort_key  value number to sort on.
	 *
	 * @param reverse   If true, reverses the sort order.  (default true, but
	 *		    this default is confusing and deprecated in 1.1.0, so
	 *		    we recommend specifying this parameter explicitly).
         */
	void set_sort_by_value(Xapian::valueno sort_key, bool reverse = true);

	/** Set the sorting to be by key generated from values only.
	 *
	 * @param sorter    The functor to use for generating keys.
	 *
	 * @param reverse   If true, reverses the sort order.  (default true, but
	 *		    this default is confusing and deprecated in 1.1.0, so
	 *		    we recommend specifying this parameter explicitly).
         */
	void set_sort_by_key(Xapian::Sorter * sorter, bool reverse = true);

	/** Set the sorting to be by value, then by relevance for documents
	 *  with the same value.
	 *
	 *  NB sorting of values uses a string comparison, so you'll need to
	 *  store numbers padded with leading zeros or spaces, or with the
	 *  number of digits prepended.
	 *
	 * @param sort_key  value number to sort on.
	 *
	 * @param reverse   If true, reverses the sort order.  (default true, but
	 *		    this default is confusing and deprecated in 1.1.0, so
	 *		    we recommend specifying this parameter explicitly).
	 */
	void set_sort_by_value_then_relevance(Xapian::valueno sort_key,
					      bool reverse = true);

	/** Set the sorting to be by keys generated from values, then by
	 *  relevance for documents with identical keys.
	 *
	 * @param sorter    The functor to use for generating keys.
	 *
	 * @param reverse   If true, reverses the sort order.  (default true, but
	 *		    this default is confusing and deprecated in 1.1.0, so
	 *		    we recommend specifying this parameter explicitly).
	 */
	void set_sort_by_key_then_relevance(Xapian::Sorter * sorter,
					    bool reverse = true);

	/** Set the sorting to be by relevance then value.
	 *
	 *  NB sorting of values uses a string comparison, so you'll need to
	 *  store numbers padded with leading zeros or spaces, or with the
	 *  number of digits prepended.
	 *
	 *  Note that with the default BM25 weighting scheme parameters,
	 *  non-identical documents will rarely have the same weight, so
	 *  this setting will give very similar results to
	 *  set_sort_by_relevance().  It becomes more useful with particular
	 *  BM25 parameter settings (e.g. BM25Weight(1,0,1,0,0)) or custom
	 *  weighting schemes.
	 *
	 * @param sort_key  value number to sort on.
	 *
	 * @param reverse   If true, reverses the sort order.  (default true, but
	 *		    this default is confusing and deprecated in 1.1.0, so
	 *		    we recommend specifying this parameter explicitly).
	 */
	void set_sort_by_relevance_then_value(Xapian::valueno sort_key,
					      bool reverse = true);

	/** Set the sorting to be by relevance, then by keys generated from
	 *  values.
	 *
	 *  Note that with the default BM25 weighting scheme parameters,
	 *  non-identical documents will rarely have the same weight, so
	 *  this setting will give very similar results to
	 *  set_sort_by_relevance().  It becomes more useful with particular
	 *  BM25 parameter settings (e.g. BM25Weight(1,0,1,0,0)) or custom
	 *  weighting schemes.
	 *
	 * @param sorter    The functor to use for generating keys.
	 *
	 * @param reverse   If true, reverses the sort order.  (default true, but
	 *		    this default is confusing and deprecated in 1.1.0, so
	 *		    we recommend specifying this parameter explicitly).
	 */
	void set_sort_by_relevance_then_key(Xapian::Sorter * sorter,
					    bool reverse = true);

	/** Get (a portion of) the match set for the current query.
	 *
	 *  @param first     the first item in the result set to return.
	 *		     A value of zero corresponds to the first item
	 *		     returned being that with the highest score.
	 *		     A value of 10 corresponds to the first 10 items
	 *		     being ignored, and the returned items starting
	 *		     at the eleventh.
	 *  @param maxitems  the maximum number of items to return.  If you
	 *		     want all matches, then you can pass the result
	 *		     of calling get_doccount() on the Database object
	 *		     (though if you are doing this so you can filter
	 *		     results, you are likely to get better performance
	 *		     by using Xapian's match-time filtering features
	 *		     instead).
	 *  @param checkatleast  the minimum number of items to check.  Because
	 *		     the matcher optimises, it won't consider every
	 *		     document which might match, so the total number
	 *		     of matches is estimated.  Setting checkatleast
	 *		     forces it to consider at least this many matches
	 *		     and so allows for reliable paging links.
	 *  @param omrset    the relevance set to use when performing the query.
	 *  @param mdecider  a decision functor to use to decide whether a
	 *		     given document should be put in the MSet.
	 *  @param matchspy  a decision functor to use to decide whether a
	 *		     given document should be put in the MSet.  The
	 *		     matchspy is applied to every document which is
	 *		     a potential candidate for the MSet, so if there are
	 *		     checkatleast or more such documents, the matchspy
	 *		     will see at least checkatleast.  The mdecider is
	 *		     assumed to be a relatively expensive test so may
	 *		     be applied in a lazier fashion.
	 *
	 *  @return	     A Xapian::MSet object containing the results of the
	 *		     query.
	 *
	 *  @exception Xapian::InvalidArgumentError  See class documentation.
	 */
	MSet get_mset(Xapian::doccount first, Xapian::doccount maxitems,
		      Xapian::doccount checkatleast = 0,
		      const RSet * omrset = 0,
		      const MatchDecider * mdecider = 0) const;
	MSet get_mset(Xapian::doccount first, Xapian::doccount maxitems,
		      Xapian::doccount checkatleast,
		      const RSet * omrset,
		      const MatchDecider * mdecider,
		      const MatchDecider * matchspy) const;
	MSet get_mset(Xapian::doccount first, Xapian::doccount maxitems,
		      const RSet * omrset,
		      const MatchDecider * mdecider = 0) const {
	    return get_mset(first, maxitems, 0, omrset, mdecider);
	}

	static const int INCLUDE_QUERY_TERMS = 1;
	static const int USE_EXACT_TERMFREQ = 2;
#ifndef _MSC_VER
	/// Deprecated in Xapian 1.0.0, use INCLUDE_QUERY_TERMS instead.
	XAPIAN_DEPRECATED(static const int include_query_terms) = 1;
	/// Deprecated in Xapian 1.0.0, use USE_EXACT_TERMFREQ instead.
	XAPIAN_DEPRECATED(static const int use_exact_termfreq) = 2;
#else
	// Work around MSVC stupidity (you get a warning for deprecating a
	// declaration).
	static const int include_query_terms = 1;
	static const int use_exact_termfreq = 2;
#pragma deprecated("Xapian::Enquire::include_query_terms", "Xapian::Enquire::use_exact_termfreq")
#endif

	/** Get the expand set for the given rset.
	 *
	 *  @param maxitems  the maximum number of items to return.
	 *  @param omrset    the relevance set to use when performing
	 *		     the expand operation.
	 *  @param flags     zero or more of these values |-ed together:
	 *		      - Xapian::Enquire::INCLUDE_QUERY_TERMS query
	 *			terms may be returned from expand
	 *		      - Xapian::Enquire::USE_EXACT_TERMFREQ for multi
	 *			dbs, calculate the exact termfreq; otherwise an
	 *			approximation is used which can greatly improve
	 *			efficiency, but still returns good results.
	 *  @param k	     the parameter k in the query expansion algorithm
	 *		     (default is 1.0)
	 *  @param edecider  a decision functor to use to decide whether a
	 *		     given term should be put in the ESet
	 *
	 *  @return	     An ESet object containing the results of the
	 *		     expand.
	 *
	 *  @exception Xapian::InvalidArgumentError  See class documentation.
	 */
	ESet get_eset(Xapian::termcount maxitems,
			const RSet & omrset,
			int flags = 0,
			double k = 1.0,
			const Xapian::ExpandDecider * edecider = 0) const;

	/** Get the expand set for the given rset.
	 *
	 *  @param maxitems  the maximum number of items to return.
	 *  @param omrset    the relevance set to use when performing
	 *		     the expand operation.
	 *  @param edecider  a decision functor to use to decide whether a
	 *		     given term should be put in the ESet
	 *
	 *  @return	     An ESet object containing the results of the
	 *		     expand.
	 *
	 *  @exception Xapian::InvalidArgumentError  See class documentation.
	 */
	inline ESet get_eset(Xapian::termcount maxitems, const RSet & omrset,
			       const Xapian::ExpandDecider * edecider) const {
	    return get_eset(maxitems, omrset, 0, 1.0, edecider);
	}

	/** Get terms which match a given document, by document id.
	 *
	 *  This method returns the terms in the current query which match
	 *  the given document.
	 *
	 *  It is possible for the document to have been removed from the
	 *  database between the time it is returned in an MSet, and the
	 *  time that this call is made.  If possible, you should specify
	 *  an MSetIterator instead of a Xapian::docid, since this will enable
	 *  database backends with suitable support to prevent this
	 *  occurring.
	 *
	 *  Note that a query does not need to have been run in order to
	 *  make this call.
	 *
	 *  @param did     The document id for which to retrieve the matching
	 *		   terms.
	 *
	 *  @return	   An iterator returning the terms which match the
	 *		   document.  The terms will be returned (as far as this
	 *		   makes any sense) in the same order as the terms
	 *		   in the query.  Terms will not occur more than once,
	 *		   even if they do in the query.
	 *
	 *  @exception Xapian::InvalidArgumentError  See class documentation.
	 *  @exception Xapian::DocNotFoundError      The document specified
	 *	could not be found in the database.
	 */
	TermIterator get_matching_terms_begin(Xapian::docid did) const;

	/** End iterator corresponding to get_matching_terms_begin() */
	TermIterator get_matching_terms_end(Xapian::docid /*did*/) const {
	    return TermIterator(NULL);
	}

	/** Get terms which match a given document, by match set item.
	 *
	 *  This method returns the terms in the current query which match
	 *  the given document.
	 *
	 *  If the underlying database has suitable support, using this call
	 *  (rather than passing a Xapian::docid) will enable the system to
	 *  ensure that the correct data is returned, and that the document
	 *  has not been deleted or changed since the query was performed.
	 *
	 *  @param it   The iterator for which to retrieve the matching terms.
	 *
	 *  @return	An iterator returning the terms which match the
	 *		   document.  The terms will be returned (as far as this
	 *		   makes any sense) in the same order as the terms
	 *		   in the query.  Terms will not occur more than once,
	 *		   even if they do in the query.
	 *
	 *  @exception Xapian::InvalidArgumentError  See class documentation.
	 *  @exception Xapian::DocNotFoundError      The document specified
	 *	could not be found in the database.
	 */
	TermIterator get_matching_terms_begin(const MSetIterator &it) const;

	/** End iterator corresponding to get_matching_terms_begin() */
	TermIterator get_matching_terms_end(const MSetIterator &/*it*/) const {
	    return TermIterator(NULL);
	}

	/** Register a MatchDecider.
	 *
	 * This is used to associate a name with a matchdecider.
	 *
	 * @deprecated This method is deprecated.  It was added long ago with
	 * the intention that it would allow the remote backend to support
	 * use of MatchDecider objects, but there's a better approach.
	 *
	 * @param name		The name to register this matchdecider as.
	 * @param mdecider	The matchdecider.  If omitted, then remove
	 *			any matchdecider registered with this name.
	 */
	XAPIAN_DEPRECATED(
	void register_match_decider(const std::string &name,
				    const MatchDecider *mdecider = NULL));

	/// Return a string describing this object.
	std::string get_description() const;
};

}

class RemoteServer;
class ScaleWeight;

namespace Xapian {

/// Abstract base class for weighting schemes
class XAPIAN_VISIBILITY_DEFAULT Weight {
    friend class Enquire; // So Enquire can clone us
    friend class ::RemoteServer; // So RemoteServer can clone us - FIXME
    friend class ::ScaleWeight;
    public:
	class Internal;
    protected:
	Weight(const Weight &);
    private:
	void operator=(Weight &);

	/** Return a new weight object of this type.
	 *
	 * A subclass called FooWeight taking parameters param1 and param2
	 * should implement this as:
	 *
	 * virtual FooWeight * clone() const {
	 *     return new FooWeight(param1, param2);
	 * }
	 */
	virtual Weight * clone() const = 0;

    protected:
	const Internal * internal; // Weight::Internal == Stats
	Xapian::doclength querysize;
	Xapian::termcount wqf;
	std::string tname;

    public:
	// FIXME:1.1: initialise internal to NULL here
	Weight() { }
	virtual ~Weight();

	/** Create a new weight object of the same type as this and initialise
	 *  it with the specified statistics.
	 *
	 *  You shouldn't call this method yourself - it's called by
	 *  Enquire.
	 *
	 *  @param internal_  Object to ask for collection statistics.
	 *  @param querysize_ Query size.
	 *  @param wqf_       Within query frequency of term this object is
	 *		      associated with.
	 *  @param tname_     Term which this object is associated with.
	 */
	Weight * create(const Internal * internal_, Xapian::doclength querysize_,
			Xapian::termcount wqf_, const std::string & tname_) const;

	/** Name of the weighting scheme.
	 *
	 *  If the subclass is called FooWeight, this should return "Foo".
	 */
	virtual std::string name() const = 0;

	/// Serialise object parameters into a string.
	virtual std::string serialise() const = 0;

	/// Create object given string serialisation returned by serialise().
	virtual Weight * unserialise(const std::string &s) const = 0;

	/** Get a weight which is part of the sum over terms being performed.
	 *  This returns a weight for a given term and document.  These
	 *  weights are summed to give a total weight for the document.
	 *
	 *  @param wdf the within document frequency of the term.
	 *  @param len the (unnormalised) document length.
	 */
	virtual Xapian::weight get_sumpart(Xapian::termcount wdf,
				      Xapian::doclength len) const = 0;

	/** Gets the maximum value that get_sumpart() may return.  This
	 *  is used in optimising searches, by having the postlist tree
	 *  decay appropriately when parts of it can have limited, or no,
	 *  further effect.
	 */
	virtual Xapian::weight get_maxpart() const = 0;

	/** Get an extra weight for a document to add to the sum calculated
	 *  over the query terms.
	 *  This returns a weight for a given document, and is used by some
	 *  weighting schemes to account for influence such as document
	 *  length.
	 *
	 *  @param len the (unnormalised) document length.
	 */
	virtual Xapian::weight get_sumextra(Xapian::doclength len) const = 0;

	/** Gets the maximum value that get_sumextra() may return.  This
	 *  is used in optimising searches.
	 */
	virtual Xapian::weight get_maxextra() const = 0;

	/// return false if the weight object doesn't need doclength
	virtual bool get_sumpart_needs_doclength() const; /* { return true; } */
};

/// Boolean weighting scheme (everything gets 0)
class XAPIAN_VISIBILITY_DEFAULT BoolWeight : public Weight {
    public:
	BoolWeight * clone() const;
	BoolWeight() { }
	~BoolWeight();
	std::string name() const;
	std::string serialise() const;
	BoolWeight * unserialise(const std::string & s) const;
	Xapian::weight get_sumpart(Xapian::termcount wdf, Xapian::doclength len) const;
	Xapian::weight get_maxpart() const;

	Xapian::weight get_sumextra(Xapian::doclength len) const;
	Xapian::weight get_maxextra() const;

	bool get_sumpart_needs_doclength() const;
};

/** BM25 weighting scheme
 *
 * BM25 weighting options : The BM25 formula is \f[
 *	\frac{k_{2}.n_{q}}{1+L_{d}}+\sum_{t}\frac{(k_{3}+1)q_{t}}{k_{3}+q_{t}}.\frac{(k_{1}+1)f_{t,d}}{k_{1}((1-b)+bL_{d})+f_{t,d}}.w_{t}
 * \f] where
 *   - \f$w_{t}\f$ is the termweight of term t
 *   - \f$f_{t,d}\f$ is the within document frequency of term t in document d
 *   - \f$q_{t}\f$ is the within query frequency of term t
 *   - \f$L_{d}\f$ is the normalised length of document d
 *   - \f$n_{q}\f$ is the size of the query
 *   - \f$k_{1}\f$, \f$k_{2}\f$, \f$k_{3}\f$ and \f$b\f$ are user specified parameters
 */
class XAPIAN_VISIBILITY_DEFAULT BM25Weight : public Weight {
    private:
	mutable Xapian::weight termweight;
	mutable Xapian::doclength lenpart;

	double k1, k2, k3, b;
	Xapian::doclength min_normlen;

	mutable bool weight_calculated;

	void calc_termweight() const;

    public:
	/** Construct a BM25 weight.
	 *
	 * @param k1 governs the importance of within document frequency.
	 *		  Must be >= 0.  0 means ignore wdf.  Default is 1.
	 * @param k2 compensation factor for the high wdf values in
	 *		  large documents.  Must be >= 0.  0 means no
	 *		  compensation.  Default is 0.
	 * @param k3 governs the importance of within query frequency.
	 *		  Must be >= 0.  0 means ignore wqf.  Default is 1.
	 * @param b Relative importance of within document frequency and
	 *		  document length.  Must be >= 0 and <= 1.  Default
	 *		  is 0.5.
	 * @param min_normlen specifies a cutoff on the minimum value that
	 *		  can be used for a normalised document length -
	 *		  smaller values will be forced up to this cutoff.
	 *		  This prevents very small documents getting a huge
	 *		  bonus weight.  Default is 0.5.
	 */
	BM25Weight(double k1_, double k2_, double k3_, double b_,
		   double min_normlen_)
		: k1(k1_), k2(k2_), k3(k3_), b(b_), min_normlen(min_normlen_),
		  weight_calculated(false)
	{
	    if (k1 < 0) k1 = 0;
	    if (k2 < 0) k2 = 0;
	    if (k3 < 0) k3 = 0;
	    if (b < 0) b = 0; else if (b > 1) b = 1;
	}
	BM25Weight() : k1(1), k2(0), k3(1), b(0.5), min_normlen(0.5),
		       weight_calculated(false) { }

	BM25Weight * clone() const;
	~BM25Weight() { }
	std::string name() const;
	std::string serialise() const;
	BM25Weight * unserialise(const std::string & s) const;
	Xapian::weight get_sumpart(Xapian::termcount wdf, Xapian::doclength len) const;
	Xapian::weight get_maxpart() const;

	Xapian::weight get_sumextra(Xapian::doclength len) const;
	Xapian::weight get_maxextra() const;

	bool get_sumpart_needs_doclength() const;
};

/** Traditional probabilistic weighting scheme.
 *
 * This class implements the Traditional Probabilistic Weighting scheme, as
 * described by the early papers on Probabilistic Retrieval.  BM25 generally
 * gives better results.
 *
 * The Traditional weighting scheme formula is \f[
 *	\sum_{t}\frac{f_{t,d}}{k.L_{d}+f_{t,d}}.w_{t}
 * \f] where
 *   - \f$w_{t}\f$ is the termweight of term t
 *   - \f$f_{t,d}\f$ is the within document frequency of term t in document d
 *   - \f$L_{d}\f$ is the normalised length of document d
 *   - \f$k\f$ is a user specifiable parameter
 *
 * TradWeight(k) is equivalent to BM25Weight(k, 0, 0, 1, 0), except that
 * the latter returns weights (k+1) times larger.
 */
class XAPIAN_VISIBILITY_DEFAULT TradWeight : public Weight {
    private:
	mutable Xapian::weight termweight;
	mutable Xapian::doclength lenpart;

	double param_k;

	mutable bool weight_calculated;

	void calc_termweight() const;

    public:
	/** Construct a TradWeight
	 *
	 * @param k  parameter governing the importance of within
	 *	     document frequency and document length - any non-negative
	 *	     number (0 meaning to ignore wdf and doc length when
	 *	     calculating weights).  Default is 1.
	 */
	explicit TradWeight(double k) : param_k(k), weight_calculated(false) {
	    if (param_k < 0) param_k = 0;
	}

	TradWeight() : param_k(1.0), weight_calculated(false) { }

	TradWeight * clone() const;
	~TradWeight() { }
	std::string name() const;
	std::string serialise() const;
	TradWeight * unserialise(const std::string & s) const;

	Xapian::weight get_sumpart(Xapian::termcount wdf, Xapian::doclength len) const;
	Xapian::weight get_maxpart() const;

	Xapian::weight get_sumextra(Xapian::doclength len) const;
	Xapian::weight get_maxextra() const;

	bool get_sumpart_needs_doclength() const;
};

}

#endif /* XAPIAN_INCLUDED_ENQUIRE_H */
