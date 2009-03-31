/** @file flint_spelling.cc
 * @brief Spelling correction data for a flint database.
 */
/* Copyright (C) 2004,2005,2006,2007 Olly Betts
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

#include <xapian/error.h>
#include <xapian/types.h>

#include "expandweight.h"
#include "flint_spelling.h"
#include "flint_utils.h"
#include "omassert.h"
#include "ortermlist.h"

#include <algorithm>
#include <map>
#include <queue>
#include <vector>
#include <set>
#include <string>

using namespace std;

// We XOR the length values with this so that they are more likely to coincide
// with lower case ASCII letters, which are likely to be common.  This means
// that zlib should do a better job of compressing tag values - in tests, this
// gave 5% better compression.
#define MAGIC_XOR_VALUE 96

class PrefixCompressedStringItor {
    const unsigned char * p;
    size_t left;
    string current;

    PrefixCompressedStringItor(const unsigned char * p_, size_t left_,
			       const string &current_)
	: p(p_), left(left_), current(current_) { }

  public:
    PrefixCompressedStringItor(const std::string & s)
	: p(reinterpret_cast<const unsigned char *>(s.data())),
	  left(s.size()) {
	if (left) {
	    operator++();
	} else {
	    p = NULL;
	}
    }

    const string & operator*() const {
	return current;
    }

    PrefixCompressedStringItor operator++(int) {
	const unsigned char * old_p = p;
	size_t old_left = left;
	string old_current = current;
	operator++();
	return PrefixCompressedStringItor(old_p, old_left, old_current);
    }

    PrefixCompressedStringItor & operator++() {
	if (left == 0) {
	    p = NULL;
	} else {
	    if (!current.empty()) {
		current.resize(*p++ ^ MAGIC_XOR_VALUE);
		--left;
	    }
	    size_t add;
	    if (left == 0 || (add = *p ^ MAGIC_XOR_VALUE) >= left)
		throw Xapian::DatabaseCorruptError("Bad spelling data (too little left)");
	    current.append(reinterpret_cast<const char *>(p + 1), add);
	    p += add + 1;
	    left -= add + 1;
	}
	return *this;
    }

    bool at_end() const {
	return p == NULL;
    }
};

class PrefixCompressedStringWriter {
    string current;
    string & out;

  public:
    PrefixCompressedStringWriter(string & out_) : out(out_) { }

    void append(const string & word) {
	// If this isn't the first entry, see how much of the previous one
	// we can reuse.
	if (!current.empty()) {
	    size_t len = min(current.size(), word.size());
	    size_t i;
	    for (i = 0; i < len; ++i) {
		if (current[i] != word[i]) break;
	    }
	    out += char(i ^ MAGIC_XOR_VALUE);
	    out += char((word.size() - i) ^ MAGIC_XOR_VALUE);
	    out.append(word.data() + i, word.size() - i);
	} else {
	    out += char(word.size() ^ MAGIC_XOR_VALUE);
	    out += word;
	}
	current = word;
    }
};

void
FlintSpellingTable::merge_changes()
{
    map<fragment, set<string> >::const_iterator i;
    for (i = termlist_deltas.begin(); i != termlist_deltas.end(); ++i) {
	string key = i->first;
	const set<string> & changes = i->second;

	set<string>::const_iterator d = changes.begin();
	Assert(d != changes.end());

	string updated;
	string current;
	PrefixCompressedStringWriter out(updated);
	if (get_exact_entry(key, current)) {
	    PrefixCompressedStringItor in(current);
	    updated.reserve(current.size()); // FIXME plus some?
	    while (!in.at_end()) {
		const string & word = *in;
		if (word < *d) {
		    out.append(word);
		    ++in;
		} else if (word > *d) {
		    out.append(*d++);
		    if (d == changes.end()) break;
		    continue;
		} else {
		    // If an existing entry is in the changes list, that means
		    // we should remove it.
		    ++in;
		    ++d;
		}
	    }
	    if (!in.at_end()) {
		// FIXME : easy to optimise this to a fix-up and substring copy.
		while (!in.at_end()) {
		    out.append(*in++);
		}
	    }
	}
	while (d != changes.end()) {
	    out.append(*d++);
	}
	if (!updated.empty()) {
	    add(key, updated);
	} else {
	    del(key);
	}
    }
    termlist_deltas.clear();

    map<string, Xapian::termcount>::const_iterator j;
    for (j = wordfreq_changes.begin(); j != wordfreq_changes.end(); ++j) {
	string key = "W" + j->first;
	if (j->second) {
	    add(key, pack_uint_last(j->second));
	} else {
	    del(key);
	}
    }
    wordfreq_changes.clear();
}

void
FlintSpellingTable::add_fragment(fragment frag, const string & word)
{
    map<fragment, set<string> >::iterator i = termlist_deltas.find(frag);
    if (i == termlist_deltas.end()) {
	i = termlist_deltas.insert(make_pair(frag, set<string>())).first;
    }
    i->second.insert(word);
}

void
FlintSpellingTable::add_word(const string & word, Xapian::termcount freqinc)
{
    if (word.size() <= 1) return;

    map<string, Xapian::termcount>::iterator i = wordfreq_changes.find(word);
    if (i != wordfreq_changes.end()) {
	// Word "word" already exists and has been modified.
	if (i->second) {
	    i->second += freqinc;
	    return;
	}
    } else {
	string key = "W" + word;
	string data;
	if (get_exact_entry(key, data)) {
	    // Word "word" already exists, so increment its count.
	    Xapian::termcount freq;
	    const char * p = data.data();
	    if (!unpack_uint_last(&p, p + data.size(), &freq) || freq == 0) {
		throw Xapian::DatabaseCorruptError("Bad spelling word freq");
	    }
	    wordfreq_changes[word] = freq + freqinc;
	    return;
	}
    }

    // New word - need to create trigrams for it.
    if (i != wordfreq_changes.end()) {
	i->second = freqinc;
    } else {
	wordfreq_changes[word] = freqinc;
    }

    fragment buf;
    // Head:
    buf[0] = 'H';
    buf[1] = word[0];
    buf[2] = word[1];
    buf[3] = '\0';
    add_fragment(buf, word);

    // Tail:
    buf[0] = 'T';
    buf[1] = word[word.size() - 2];
    buf[2] = word[word.size() - 1];
    buf[3] = '\0';
    add_fragment(buf, word);

    if (word.size() <= 4) {
	// We also generate 'bookends' for two, three, and four character
	// terms so we can handle transposition of the middle two characters
	// of a four character word, substitution or deletion of the middle
	// character of a three character word, or insertion in the middle of a
	// two character word.
	// 'Bookends':
	buf[0] = 'B';
	buf[1] = word[0];
	buf[3] = '\0';
	add_fragment(buf, word);
    }
    if (word.size() > 2) {
	// Middles:
	buf[0] = 'M';
	for (size_t start = 0; start <= word.size() - 3; ++start) {
	    memcpy(buf.data + 1, word.data() + start, 3);
	    add_fragment(buf, word);
	}
    }
}

void
FlintSpellingTable::remove_fragment(fragment frag, const string & word)
{
    map<fragment, set<string> >::iterator i = termlist_deltas.find(frag);
    if (i != termlist_deltas.end()) {
	i->second.erase(word);
    }
}

void
FlintSpellingTable::remove_word(const string & word, Xapian::termcount freqdec)
{
    map<string, Xapian::termcount>::iterator i = wordfreq_changes.find(word);
    if (i != wordfreq_changes.end()) {
	if (i->second == 0) {
	    // Word has already been deleted.
	    return;
	}
	// Word "word" exists and has been modified.
	if (freqdec < i->second) {
	    i->second -= freqdec;
	    return;
	}
    }

    {
	string key = "W" + word;
	string data;
	if (!get_exact_entry(key, data)) {
	    // This word doesn't exist.
	    return;
	}

	Xapian::termcount freq;
	const char *p = data.data();
	if (!unpack_uint_last(&p, p + data.size(), &freq)) {
	    throw Xapian::DatabaseCorruptError("Bad spelling word freq");
	}
	if (freqdec < freq) {
	    wordfreq_changes[word] = freq - freqdec;
	    return;
	}
    }

    // Mark word as deleted, and remove its fragment entries.
    wordfreq_changes[word] = 0;

    fragment buf;
    // Head:
    buf[0] = 'H';
    buf[1] = word[0];
    buf[2] = word[1];
    buf[3] = '\0';
    remove_fragment(buf, word);

    // Tail:
    buf[0] = 'T';
    buf[1] = word[word.size() - 2];
    buf[2] = word[word.size() - 1];
    buf[3] = '\0';
    remove_fragment(buf, word);

    if (word.size() <= 4) {
	// 'Bookends':
	buf[0] = 'B';
	buf[1] = word[0];
	buf[3] = '\0';
	remove_fragment(buf, word);
    }
    if (word.size() > 2) {
	// Middles:
	buf[0] = 'M';
	for (size_t start = 0; start <= word.size() - 3; ++start) {
	    memcpy(buf.data + 1, word.data() + start, 3);
	    remove_fragment(buf, word);
	}
    }
}

struct TermListGreaterApproxSize {
    bool operator()(const TermList *a, const TermList *b) {
	return a->get_approx_size() > b->get_approx_size();
    }
};

TermList *
FlintSpellingTable::open_termlist(const string & word)
{
    if (word.size() <= 1) return NULL;

    // Merge any pending changes to disk, but don't call commit() so they
    // won't be switched live.
    if (!wordfreq_changes.empty()) merge_changes();

    // Build a priority queue of TermList objects which returns those of
    // greatest approximate size first.
    priority_queue<TermList*, vector<TermList*>, TermListGreaterApproxSize> pq;
    try {
	string data;
	fragment buf;

	// Head:
	buf[0] = 'H';
	buf[1] = word[0];
	buf[2] = word[1];
	if (get_exact_entry(string(buf), data))
	    pq.push(new FlintSpellingTermList(data));

	// Tail:
	buf[0] = 'T';
	buf[1] = word[word.size() - 2];
	buf[2] = word[word.size() - 1];
	if (get_exact_entry(string(buf), data))
	    pq.push(new FlintSpellingTermList(data));

	if (word.size() <= 4) {
	    // We also generate 'bookends' for two, three, and four character
	    // terms so we can handle transposition of the middle two
	    // characters of a four character word, substitution or deletion of
	    // the middle character of a three character word, or insertion in
	    // the middle of a two character word.
	    buf[0] = 'B';
	    buf[1] = word[0];
	    buf[3] = '\0';
	    if (get_exact_entry(string(buf), data))
		pq.push(new FlintSpellingTermList(data));
	}
	if (word.size() > 2) {
	    // Middles:
	    buf[0] = 'M';
	    for (size_t start = 0; start <= word.size() - 3; ++start) {
		memcpy(buf.data + 1, word.data() + start, 3);
		if (get_exact_entry(string(buf), data))
		    pq.push(new FlintSpellingTermList(data));
	    }

	    if (word.size() == 3) {
		// For three letter words, we generate the two "single
		// transposition" forms too, so that we can produce good
		// spelling suggestions.
		// ABC -> BAC
		buf[1] = word[1];
		buf[2] = word[0];
		if (get_exact_entry(string(buf), data))
		    pq.push(new FlintSpellingTermList(data));
		// ABC -> ACB
		buf[1] = word[0];
		buf[2] = word[2];
		buf[3] = word[1];
		if (get_exact_entry(string(buf), data))
		    pq.push(new FlintSpellingTermList(data));
	    }
	} else {
	    Assert(word.size() == 2);
	    // For two letter words, we generate H and T terms for the
	    // transposed form so that we can produce good spelling
	    // suggestions.
	    // AB -> BA
	    buf[0] = 'H';
	    buf[1] = word[1];
	    buf[2] = word[0];
	    if (get_exact_entry(string(buf), data))
		pq.push(new FlintSpellingTermList(data));
	    buf[0] = 'T';
	    if (get_exact_entry(string(buf), data))
		pq.push(new FlintSpellingTermList(data));
	}

	if (pq.empty()) return NULL;

	// Build up an OrTermList tree by combine leaves and/or branches in
	// pairs.  The tree is balanced by the approximated sizes of the leaf
	// FlintSpellingTermList objects - the way the tree is built are very
	// similar to how an optimal Huffman code is often constructed.
	//
	// Balancing the tree like this should tend to minimise the amount of
	// work done.
	while (pq.size() > 1) {
	    // Build the tree such that left is always >= right so that
	    // OrTermList can rely on this when trying to minimise work.
	    TermList * termlist = pq.top();
	    pq.pop();

	    termlist = new OrTermList(pq.top(), termlist);
	    pq.pop();
	    pq.push(termlist);
	}

	return pq.top();
    } catch (...) {
	// Make sure we delete all the TermList objects to avoid leaking
	// memory.
	while (!pq.empty()) {
	    delete pq.top();
	    pq.pop();
	}
	throw;
    }
}

Xapian::doccount
FlintSpellingTable::get_word_frequency(const string & word) const
{
    map<string, Xapian::termcount>::const_iterator i;
    i = wordfreq_changes.find(word);
    if (i != wordfreq_changes.end()) {
	// Modified frequency for word:
	return i->second;
    }

    string key = "W" + word;
    string data;
    if (get_exact_entry(key, data)) {
	// Word "word" already exists.
	Xapian::termcount freq;
	const char *p = data.data();
	if (!unpack_uint_last(&p, p + data.size(), &freq)) {
	    throw Xapian::DatabaseCorruptError("Bad spelling word freq");
	}
	return freq;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////

Xapian::termcount
FlintSpellingTermList::get_approx_size() const
{
    // This is only used to decide how to build a OR-tree of TermList objects
    // so we just need to return "sizes" which are ordered roughly correctly.
    return data.size();
}

std::string
FlintSpellingTermList::get_termname() const
{
    return current_term;
}

Xapian::termcount
FlintSpellingTermList::get_wdf() const
{
    return 1;
}

Xapian::doccount
FlintSpellingTermList::get_termfreq() const
{
    return 1;
}

Xapian::termcount
FlintSpellingTermList::get_collection_freq() const
{
    return 1;
}

TermList *
FlintSpellingTermList::next()
{
    if (p == data.size()) {
	p = 0;
	data.resize(0);
	return NULL;
    }
    if (!current_term.empty()) {
	if (p == data.size())
	    throw Xapian::DatabaseCorruptError("Bad spelling termlist");
	current_term.resize(byte(data[p++]) ^ MAGIC_XOR_VALUE);
    }
    size_t add;
    if (p == data.size() ||
	(add = byte(data[p]) ^ MAGIC_XOR_VALUE) >= data.size() - p)
	throw Xapian::DatabaseCorruptError("Bad spelling termlist");
    current_term.append(data.data() + p + 1, add);
    p += add + 1;
    return NULL;
}

bool
FlintSpellingTermList::at_end() const
{
    return data.empty();
}

Xapian::termcount
FlintSpellingTermList::positionlist_count() const
{
    throw Xapian::UnimplementedError("FlintSpellingTermList::positionlist_count() not implemented");
}

Xapian::PositionIterator
FlintSpellingTermList::positionlist_begin() const
{
    throw Xapian::UnimplementedError("FlintSpellingTermList::positionlist_begin() not implemented");
}
