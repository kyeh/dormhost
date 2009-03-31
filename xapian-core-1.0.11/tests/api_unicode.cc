/** @file api_unicode.cc
 * @brief test the Unicode and UTF-8 classes and functions.
 */
/* Copyright (C) 2006,2007,2008 Olly Betts
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

#include "api_unicode.h"

#include <xapian.h>

#include "apitest.h"
#include "testutils.h"

#include <cctype>

using namespace std;

struct testcase {
    const char * a, * b;
};

static const testcase testcases[] = {
    { "abcd", "abcd" }, // Sanity check!
    { "a\x80""bcd", "a\xc2\x80""bcd" },
    { "a\xa0", "a\xc2\xa0" },
    { 0, 0 }
};

// Test handling of invalid UTF-8 is as desired.
DEFINE_TESTCASE(utf8iterator1,!backend) {
    const testcase * p;
    for (p = testcases; p->a; ++p) {
	tout << '"' << p->a << "\" and \"" << p->b << '"' << endl;
	size_t a_len = strlen(p->a);
	Xapian::Utf8Iterator a(p->a, a_len);

	size_t b_len = strlen(p->b);
	Xapian::Utf8Iterator b(p->b, b_len);

	while (a != Xapian::Utf8Iterator() && b != Xapian::Utf8Iterator()) {
	    TEST_EQUAL(*a, *b);
	    ++a;
	    ++b;
	}

	// Test that we don't reach the end of one before the other.
	TEST(a == Xapian::Utf8Iterator());
	TEST(b == Xapian::Utf8Iterator());
    }
    return true;
}

struct testcase2 {
    const char * a;
    unsigned long n;
};

static const testcase2 testcases2[] = {
    { "a", 97 },
    { "\x80", 128 },
    { "\xa0", 160 },
    { "\xc2\x80", 128 },
    { "\xc2\xa0", 160 },
    { "\xf0\xa8\xa8\x8f", 166415 },
    { 0, 0 }
};

// Test decoding of UTF-8.
DEFINE_TESTCASE(utf8iterator2,!backend) {
    const testcase2 * p;
    for (p = testcases2; p->a; ++p) {
	Xapian::Utf8Iterator a(p->a, strlen(p->a));

	TEST(a != Xapian::Utf8Iterator());
	TEST_EQUAL(*a, p->n);
	TEST(++a == Xapian::Utf8Iterator());
    }
    return true;
}

// Test Unicode categorisation.
DEFINE_TESTCASE(unicode1,!backend) {
    using namespace Xapian;
    TEST_EQUAL(Unicode::get_category('a'), Unicode::LOWERCASE_LETTER);
    TEST_EQUAL(Unicode::get_category('0'), Unicode::DECIMAL_DIGIT_NUMBER);
    TEST_EQUAL(Unicode::get_category('$'), Unicode::CURRENCY_SYMBOL);
    TEST_EQUAL(Unicode::get_category(0xa3), Unicode::CURRENCY_SYMBOL);
    // U+0242 was added in Unicode 5.0.0.
    TEST_EQUAL(Unicode::get_category(0x242), Unicode::LOWERCASE_LETTER);
    TEST_EQUAL(Unicode::get_category(0xFFFF), Unicode::UNASSIGNED);
    // Test characters outside BMP.
    TEST_EQUAL(Unicode::get_category(0x10345), Unicode::OTHER_LETTER);
    TEST_EQUAL(Unicode::get_category(0x10FFFD), Unicode::PRIVATE_USE);
    TEST_EQUAL(Unicode::get_category(0x10FFFF), Unicode::UNASSIGNED);
    // Test some invalid Unicode values.
    TEST_EQUAL(Unicode::get_category(0x110000), Unicode::UNASSIGNED);
    TEST_EQUAL(Unicode::get_category(0xFFFFFFFF), Unicode::UNASSIGNED);
    return true;
}

DEFINE_TESTCASE(caseconvert1,!backend) {
    using namespace Xapian;
    for (unsigned ch = 0; ch < 128; ++ch) {
	if (isupper((char)ch)) {
	    TEST_EQUAL(Unicode::tolower(ch), unsigned(tolower((char)ch)));
	} else {
	    TEST_EQUAL(Unicode::tolower(ch), ch);
	}
	if (islower((char)ch)) {
	    TEST_EQUAL(Unicode::toupper(ch), unsigned(toupper((char)ch)));
	} else {
	    TEST_EQUAL(Unicode::toupper(ch), ch);
	}
    }

    // U+0242 was added in Unicode 5.0.0 as a lowercase form of U+0241.
    TEST_EQUAL(Unicode::tolower(0x242), 0x242);
    TEST_EQUAL(Unicode::toupper(0x242), 0x241);
    TEST_EQUAL(Unicode::toupper(0x241), 0x241);
    TEST_EQUAL(Unicode::tolower(0x241), 0x242);

    // Pound currency symbol:
    TEST_EQUAL(Unicode::tolower(0xa3), 0xa3);
    TEST_EQUAL(Unicode::toupper(0xa3), 0xa3);
    // Unassigned:
    TEST_EQUAL(Unicode::tolower(0xFFFF), 0xFFFF);
    TEST_EQUAL(Unicode::toupper(0xFFFF), 0xFFFF);
    // Test characters outside BMP.
    TEST_EQUAL(Unicode::tolower(0x10345), 0x10345);
    TEST_EQUAL(Unicode::toupper(0x10345), 0x10345);
    TEST_EQUAL(Unicode::tolower(0x10FFFD), 0x10FFFD);
    TEST_EQUAL(Unicode::toupper(0x10FFFD), 0x10FFFD);
    TEST_EQUAL(Unicode::tolower(0x10FFFF), 0x10FFFF);
    TEST_EQUAL(Unicode::toupper(0x10FFFF), 0x10FFFF);
    // Test some invalid Unicode values.
    TEST_EQUAL(Unicode::tolower(0x110000), 0x110000);
    TEST_EQUAL(Unicode::toupper(0x110000), 0x110000);
    TEST_EQUAL(Unicode::tolower(0xFFFFFFFF), 0xFFFFFFFF);
    TEST_EQUAL(Unicode::toupper(0xFFFFFFFF), 0xFFFFFFFF);

    return true;
}
