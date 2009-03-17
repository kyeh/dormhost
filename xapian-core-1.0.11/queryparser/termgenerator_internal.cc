/** @file: termgenerator_internal.cc
 * @brief TermGenerator class internals
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

#include <config.h>

#include "termgenerator_internal.h"

#include <xapian/document.h>
#include <xapian/queryparser.h>
#include <xapian/unicode.h>

#include "stringutils.h"

#include <algorithm>
#include <string>

using namespace std;

namespace Xapian {

// Put a limit on the size of terms to help prevent the index being bloated
// by useless junk terms.
static const unsigned int MAX_PROB_TERM_LENGTH = 64;
// FIXME: threshold is currently in bytes of UTF-8 representation, not unicode
// characters - what actually makes most sense here?

// FIXME: Add API to allow control of how stemming is used?

inline bool
U_isupper(unsigned ch) {
    return (ch < 128 && C_isupper((unsigned char)ch));
}

inline unsigned check_wordchar(unsigned ch) {
    if (Unicode::is_wordchar(ch)) return Unicode::tolower(ch);
    return 0;
}

inline bool
should_stem(const std::string & term)
{
    const unsigned int SHOULD_STEM_MASK =
	(1 << Unicode::LOWERCASE_LETTER) |
	(1 << Unicode::TITLECASE_LETTER) |
	(1 << Unicode::MODIFIER_LETTER) |
	(1 << Unicode::OTHER_LETTER);
    Utf8Iterator u(term);
    return ((SHOULD_STEM_MASK >> Unicode::get_category(*u)) & 1);
}

inline unsigned check_infix(unsigned ch) {
    if (ch == '\'' || ch == '&' || ch == 0xb7 || ch == 0x5f4 || ch == 0x2027) {
	// Unicode includes all these except '&' in it's word boundary rules,
	// as well as 0x2019 (which we handle below) and ':' (for Swedish
	// apparently, but we ignore this for now as it's problematic in
	// real world cases).
	return ch;
    }
    // 0x2019 is Unicode apostrophe and single closing quote.
    // 0x201b is Unicode single opening quote with the tail rising.
    if (ch == 0x2019 || ch == 0x201b) return '\'';
    return 0;
}

inline unsigned check_infix_digit(unsigned ch) {
    // This list of characters comes from Unicode's word identifying algorithm.
    switch (ch) {
	case ',':
	case '.':
	case ';':
	case 0x037e: // GREEK QUESTION MARK
	case 0x0589: // ARMENIAN FULL STOP
	case 0x060D: // ARABIC DATE SEPARATOR
	case 0x07F8: // NKO COMMA
	case 0x2044: // FRACTION SLASH
	case 0xFE10: // PRESENTATION FORM FOR VERTICAL COMMA
	case 0xFE13: // PRESENTATION FORM FOR VERTICAL COLON
	case 0xFE14: // PRESENTATION FORM FOR VERTICAL SEMICOLON
	    return ch;
    }
    return 0;
}

inline bool
is_digit(unsigned ch) {
    return (Unicode::get_category(ch) == Unicode::DECIMAL_DIGIT_NUMBER);
}

inline unsigned check_suffix(unsigned ch) {
    if (ch == '+' || ch == '#') return ch;
    // FIXME: what about '-'?
    return 0;
}

// FIXME: add API for this:
#define STOPWORDS_NONE 0
#define STOPWORDS_IGNORE 1
#define STOPWORDS_INDEX_UNSTEMMED_ONLY 2

void
TermGenerator::Internal::index_text(Utf8Iterator itor, termcount weight,
				    const string & prefix, bool with_positions)
{
    int stop_mode = STOPWORDS_INDEX_UNSTEMMED_ONLY;

    if (!stopper) stop_mode = STOPWORDS_NONE;

    while (true) {
	// Advance to the start of the next term.
	unsigned ch;
	while (true) {
	    if (itor == Utf8Iterator()) return;
	    ch = check_wordchar(*itor);
	    if (ch) break;
	    ++itor;
	}

	string term;
	// Look for initials separated by '.' (e.g. P.T.O., U.N.C.L.E).
	// Don't worry if there's a trailing '.' or not.
	if (U_isupper(*itor)) {
	    const Utf8Iterator end;
	    Utf8Iterator p = itor;
	    do {
		Unicode::append_utf8(term, Unicode::tolower(*p++));
	    } while (p != end && *p == '.' && ++p != end && U_isupper(*p));
	    // One letter does not make an acronym!  If we handled a single
	    // uppercase letter here, we wouldn't catch M&S below.
	    if (term.size() > 1) {
		// Check there's not a (lower case) letter or digit
		// immediately after it.
		if (p == end || !Unicode::is_wordchar(*p)) {
		    itor = p;
		    goto endofterm;
		}
	    }
	    term.resize(0);
	}

	while (true) {
	    unsigned prevch;
	    do {
		Unicode::append_utf8(term, ch);
		prevch = ch;
		if (++itor == Utf8Iterator()) goto endofterm;
		ch = check_wordchar(*itor);
	    } while (ch);

	    Utf8Iterator next(itor);
	    ++next;
	    if (next == Utf8Iterator()) break;
	    unsigned nextch = check_wordchar(*next);
	    if (!nextch) break;
	    unsigned infix_ch = *itor;
	    if (is_digit(prevch) && is_digit(*next)) {
		infix_ch = check_infix_digit(infix_ch);
	    } else {
		// Handle things like '&' in AT&T, apostrophes, etc.
		infix_ch = check_infix(infix_ch);
	    }
	    if (!infix_ch) break;
	    Unicode::append_utf8(term, infix_ch);
	    ch = nextch;
	    itor = next;
	}

	{
	    size_t len = term.size();
	    unsigned count = 0;
	    while ((ch = check_suffix(*itor))) {
		if (++count > 3) {
		    term.resize(len);
		    break;
		}
		Unicode::append_utf8(term, ch);
		if (++itor == Utf8Iterator()) goto endofterm;
	    }
	}

endofterm:
	if (term.size() > MAX_PROB_TERM_LENGTH) continue;

	if (stop_mode == STOPWORDS_IGNORE && (*stopper)(term)) continue;

	if (with_positions) {
	    doc.add_posting(prefix + term, ++termpos, weight);
	} else {
	    doc.add_term(prefix + term, weight);
	}
	if ((flags & FLAG_SPELLING) && prefix.empty()) db.add_spelling(term);

	if (!stemmer.internal.get()) continue;

	if (stop_mode == STOPWORDS_INDEX_UNSTEMMED_ONLY && (*stopper)(term))
	    continue;

	// Note, this uses the lowercased term, but that's OK as we only
	// want to avoid stemming terms starting with a digit.
	if (!should_stem(term)) continue;

	// Add stemmed form without positional information.
	string stem("Z");
	stem += prefix;
	stem += stemmer(term);
	doc.add_term(stem, weight);
    }
}

}
