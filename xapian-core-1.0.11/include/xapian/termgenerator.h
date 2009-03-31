/** @file termgenerator.h
 * @brief parse free text and generate terms
 */
/* Copyright (C) 2007 Olly Betts
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

#ifndef XAPIAN_INCLUDED_TERMGENERATOR_H
#define XAPIAN_INCLUDED_TERMGENERATOR_H

#include <xapian/base.h>
#include <xapian/types.h>
#include <xapian/unicode.h>
#include <xapian/visibility.h>

#include <string>

namespace Xapian {

class Document;
class Stem;
class Stopper;
class WritableDatabase;

/** Parses a piece of text and generate terms.
 *
 * This module takes a piece of text and parses it to produce words which are
 * then used to generate suitable terms for indexing.  The terms generated are
 * suitable for use with Query objects produced by the QueryParser class.
 */
class XAPIAN_VISIBILITY_DEFAULT TermGenerator {
  public:
    /// @private @internal Class representing the TermGenerator internals.
    class Internal;
    /// @private @internal Reference counted internals.
    Xapian::Internal::RefCntPtr<Internal> internal;

    /// Copy constructor.
    TermGenerator(const TermGenerator & o);

    /// Assignment.
    TermGenerator & operator=(const TermGenerator & o);

    /// Default constructor.
    TermGenerator();

    /// Destructor.
    ~TermGenerator();

    /// Set the Xapian::Stem object to be used for generating stemmed terms.
    void set_stemmer(const Xapian::Stem & stemmer);

    /// Set the Xapian::Stopper object to be used for identifying stopwords.
    void set_stopper(const Xapian::Stopper *stop = NULL);

    /// Set the current document.
    void set_document(const Xapian::Document & doc);

    /// Get the current document.
    const Xapian::Document & get_document() const;

    /// Set the database to index spelling data to.
    void set_database(const Xapian::WritableDatabase &db);

    /// Flags to OR together and pass to TermGenerator::set_flags().
    enum flags {
	/// Index data required for spelling correction.
	FLAG_SPELLING = 128 // Value matches QueryParser flag.
    };

    /** Set flags.
     *
     *  The new value of flags is: (flags & mask) ^ toggle
     *
     *  To just set the flags, pass the new flags in toggle and the
     *  default value for mask.
     *
     *  @param toggle	Flags to XOR.
     *  @param mask	Flags to AND with first.
     *
     *  @return		The old flags setting.
     */
    flags set_flags(flags toggle, flags mask = flags(0));

    /** Index some text.
     *
     * @param weight	The wdf increment (default 1).
     * @param prefix	The term prefix to use (default is no prefix).
     */
    void index_text(const Xapian::Utf8Iterator & itor,
		    Xapian::termcount weight = 1,
		    const std::string & prefix = "");

    /** Index some text in a std::string.
     *
     * @param weight	The wdf increment (default 1).
     * @param prefix	The term prefix to use (default is no prefix).
     */
    void index_text(const std::string & text,
		    Xapian::termcount weight = 1,
		    const std::string & prefix = "") {
	return index_text(Utf8Iterator(text), weight, prefix);
    }

    /** Index some text without positional information.
     *
     * Just like index_text, but no positional information is generated.  This
     * means that the database will be significantly smaller, but that phrase
     * searching and NEAR won't be supported.
     */
    void index_text_without_positions(const Xapian::Utf8Iterator & itor,
				      Xapian::termcount weight = 1,
				      const std::string & prefix = "");

    /** Index some text in a std::string without positional information.
     *
     * Just like index_text, but no positional information is generated.  This
     * means that the database will be significantly smaller, but that phrase
     * searching and NEAR won't be supported.
     */
    void index_text_without_positions(const std::string & text,
				      Xapian::termcount weight = 1,
				      const std::string & prefix = "") {
	return index_text_without_positions(Utf8Iterator(text), weight, prefix);
    }

    /** Increase the termpos used by index_text by @a delta.
     *
     *  This can be used to prevent phrase searches from spanning two
     *  unconnected blocks of text (e.g. the title and body text).
     */
    void increase_termpos(Xapian::termcount delta = 100);

    /// Get the current term position.
    Xapian::termcount get_termpos() const;

    /// Set the current term position.
    void set_termpos(Xapian::termcount termpos);

    /// Return a string describing this object.
    std::string get_description() const;
};

}

#endif // XAPIAN_INCLUDED_TERMGENERATOR_H
