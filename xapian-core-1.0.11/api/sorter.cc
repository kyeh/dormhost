/** @file sorter.cc
 * @brief Build sort keys for MSet ordering
 */
/* Copyright (C) 2007 Olly Betts
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <config.h>

#include <xapian/sorter.h>

#include <string>
#include <vector>

using namespace std;

namespace Xapian {

Sorter::~Sorter() { }

string
MultiValueSorter::operator()(const Xapian::Document & doc) const
{
    string result;

    vector<pair<Xapian::valueno, bool> >::const_iterator i = valnos.begin();
    // Don't crash if valnos is empty.
    if (rare(i == valnos.end())) return result;

    while (true) {
	// All values (except for the last if it's sorted forwards) need to
	// be adjusted.
	//
	// FIXME: allow Xapian::BAD_VALNO to mean "relevance?"
	string v = doc.get_value(i->first);
	bool reverse_sort = !i->second;

	if (++i == valnos.end() && !reverse_sort) {
	    // No need to adjust the last value if it's sorted forwards.
	    result += v;
	    break;
	}

	if (reverse_sort) {
	    // For a reverse ordered value, we subtract each byte from '\xff',
	    // except for '\0' which we convert to "\xff\0".  We insert
	    // "\xff\xff" after the encoded value.
	    for (string::const_iterator j = v.begin(); j != v.end(); ++j) {
		unsigned char ch(*j);
		result += char(255 - ch);
		if (ch == 0) result += '\0';
	    }
	    result.append("\xff\xff", 2);
	    if (i == valnos.end()) break;
	} else {
	    // For a forward ordered value (unless it's the last value), we
	    // convert any '\0' to "\0\xff".  We insert "\0\0" after the
	    // encoded value.
	    string::size_type j = 0, nul;
	    while ((nul = v.find('\0', j)) != string::npos) {
		++nul;
		result.append(v, j, nul - j);
		result += '\xff';
		j = nul;
	    }
	    result.append(v, j, string::npos);
	    result.append("\0", 2);
	}
    }
    return result;
}

MultiValueSorter::~MultiValueSorter() { }

}
