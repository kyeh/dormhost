/* api_nodb.cc: tests which don't use any of the backends
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2002,2003,2004,2005,2006,2007 Olly Betts
 * Copyright 2006 Lemur Consulting Ltd
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

#include <config.h>

#include "api_nodb.h"

#include <string>
#include <vector>
#include "autoptr.h"

#include <xapian.h>

#include "apitest.h"
#include "testsuite.h"
#include "testutils.h"
#include "utils.h"

#include <list>

using namespace std;

// always succeeds
DEFINE_TESTCASE(trivial1, !backend) {
    return true;
}

// tests that get_query_terms() returns the terms in the right order
DEFINE_TESTCASE(getqterms1, !backend) {
    list<string> answers_list;
    answers_list.push_back("one");
    answers_list.push_back("two");
    answers_list.push_back("three");
    answers_list.push_back("four");

    Xapian::Query myquery(Xapian::Query::OP_OR,
	    Xapian::Query(Xapian::Query::OP_AND,
		    Xapian::Query("one", 1, 1),
		    Xapian::Query("three", 1, 3)),
	    Xapian::Query(Xapian::Query::OP_OR,
		    Xapian::Query("four", 1, 4),
		    Xapian::Query("two", 1, 2)));

    list<string> list1;
    {
	Xapian::TermIterator t;
	for (t = myquery.get_terms_begin(); t != myquery.get_terms_end(); ++t)
	    list1.push_back(*t);
    }
    TEST(list1 == answers_list);
    list<string> list2(myquery.get_terms_begin(), myquery.get_terms_end());
    TEST(list2 == answers_list);
    return true;
}

// tests that get_query_terms() doesn't SEGV on an empty query
// (regression test for bug in 0.9.0)
DEFINE_TESTCASE(getqterms2, !backend) {
    Xapian::Query empty_query;
    TEST_EQUAL(empty_query.get_terms_begin(), empty_query.get_terms_end());
    return true;
}

// tests that empty queries work correctly
DEFINE_TESTCASE(emptyquery2, !backend) {
    // test that Query::empty() is true for an empty query.
    TEST(Xapian::Query().empty());
    // test that an empty query has length 0
    TEST(Xapian::Query().get_length() == 0);
    vector<Xapian::Query> v;
    TEST(Xapian::Query(Xapian::Query::OP_OR, v.begin(), v.end()).empty());
    TEST(Xapian::Query(Xapian::Query::OP_OR, v.begin(), v.end()).get_length() == 0);
    return true;
}

/// Regression test for behaviour for an empty query with AND_NOT.
DEFINE_TESTCASE(emptyquery3, !backend) {
    static const Xapian::Query::op ops[] = {
	Xapian::Query::OP_AND,
	Xapian::Query::OP_OR,
	Xapian::Query::OP_XOR,
	Xapian::Query::OP_AND_MAYBE,
	Xapian::Query::OP_AND_NOT
    };

    for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); ++i) {
	tout << "Testing op #" << i << endl;
	Xapian::Query empty;
	Xapian::Query q("test");
	Xapian::Query qcombine(ops[i], empty, q);
	tout << qcombine.get_description() << endl;
	Xapian::Query qcombine2(ops[i], q, empty);
	tout << qcombine2.get_description() << endl;
    }

    return true;
}

// tests that query lengths are calculated correctly
DEFINE_TESTCASE(querylen1, !backend) {
    // test that a simple query has the right length
    Xapian::Query myquery;
    myquery = Xapian::Query(Xapian::Query::OP_OR,
		      Xapian::Query("foo"),
		      Xapian::Query("bar"));
    myquery = Xapian::Query(Xapian::Query::OP_AND,
		      myquery,
		      Xapian::Query(Xapian::Query::OP_OR,
			      Xapian::Query("wibble"),
			      Xapian::Query("spoon")));

    TEST_EQUAL(myquery.get_length(), 4);
    TEST(!myquery.empty());
    return true;
}

// tests that query lengths are calculated correctly
DEFINE_TESTCASE(querylen2, !backend) {
    // test with an even bigger and strange query
    string terms[3] = {
	"foo",
	"bar",
	"baz"
    };
    Xapian::Query queries[3] = {
	Xapian::Query("wibble"),
	Xapian::Query("wobble"),
	Xapian::Query(Xapian::Query::OP_OR, string("jelly"), string("belly"))
    };

    Xapian::Query myquery;
    vector<string> v1(terms, terms + 3);
    vector<Xapian::Query> v2(queries, queries + 3);
    vector<Xapian::Query *> v3;
    AutoPtr<Xapian::Query> dynquery1(new Xapian::Query(Xapian::Query::OP_AND,
					   string("ball"),
					   string("club")));
    AutoPtr<Xapian::Query> dynquery2(new Xapian::Query("ring"));
    v3.push_back(dynquery1.get());
    v3.push_back(dynquery2.get());

    Xapian::Query myq1 = Xapian::Query(Xapian::Query::OP_AND, v1.begin(), v1.end());
    tout << "myq1=" << myq1 << "\n";
    TEST_EQUAL(myq1.get_length(), 3);

    Xapian::Query myq2_1 = Xapian::Query(Xapian::Query::OP_OR, v2.begin(), v2.end());
    tout << "myq2_1=" << myq2_1 << "\n";
    TEST_EQUAL(myq2_1.get_length(), 4);

    Xapian::Query myq2_2 = Xapian::Query(Xapian::Query::OP_AND, v3.begin(), v3.end());
    tout << "myq2_2=" << myq2_2 << "\n";
    TEST_EQUAL(myq2_2.get_length(), 3);

    Xapian::Query myq2 = Xapian::Query(Xapian::Query::OP_OR, myq2_1, myq2_2);
    tout << "myq2=" << myq2 << "\n";
    TEST_EQUAL(myq2.get_length(), 7);

    myquery = Xapian::Query(Xapian::Query::OP_OR, myq1, myq2);
    tout << "myquery=" << myquery << "\n";
    TEST_EQUAL(myquery.get_length(), 10);

    return true;
}

// tests that queries validate correctly
DEFINE_TESTCASE(queryvalid1, !backend) {
    vector<Xapian::Query> v1;
    // Need two arguments
    TEST_EXCEPTION(Xapian::InvalidArgumentError,
		   Xapian::Query(Xapian::Query::OP_AND_NOT, v1.begin(), v1.end()));
    tout << "ANDNOT () checked" << endl;
    v1.push_back(Xapian::Query("bad"));
    TEST_EXCEPTION(Xapian::InvalidArgumentError,
		   Xapian::Query(Xapian::Query::OP_AND_NOT, v1.begin(), v1.end()));
    tout << "ANDNOT (\"bad\") checked" << endl;
    v1.clear();
    v1.push_back(Xapian::Query());
    TEST_EXCEPTION(Xapian::InvalidArgumentError,
		   Xapian::Query(Xapian::Query::OP_AND_NOT, v1.begin(), v1.end()));
    tout << "ANDNOT (Xapian::Query()) checked" << endl;
    Xapian::Query q2(Xapian::Query::OP_XOR, Xapian::Query("foo"), Xapian::Query("bar"));
    tout << "XOR (\"foo\", \"bar\") checked" << endl;
    return true;
}

// tests that collapsing of queries includes subqueries
DEFINE_TESTCASE(subqcollapse1, !backend) {
    Xapian::Query queries1[3] = {
	Xapian::Query("wibble"),
	Xapian::Query("wobble"),
	Xapian::Query(Xapian::Query::OP_OR, string("jelly"), string("belly"))
    };

    Xapian::Query queries2[3] = {
	Xapian::Query(Xapian::Query::OP_AND, string("jelly"), string("belly")),
	Xapian::Query("wibble"),
	Xapian::Query("wobble")
    };

    vector<Xapian::Query> vec1(queries1, queries1 + 3);
    Xapian::Query myquery1(Xapian::Query::OP_OR, vec1.begin(), vec1.end());
    TEST_EQUAL(myquery1.get_description(),
	       "Xapian::Query((wibble OR wobble OR jelly OR belly))");

    vector<Xapian::Query> vec2(queries2, queries2 + 3);
    Xapian::Query myquery2(Xapian::Query::OP_AND, vec2.begin(), vec2.end());
    TEST_EQUAL(myquery2.get_description(),
	       "Xapian::Query((jelly AND belly AND wibble AND wobble))");

    return true;
}

// test behaviour when creating a query from an empty vector
DEFINE_TESTCASE(emptyquerypart1, !backend) {
    vector<string> emptyterms;
    Xapian::Query query(Xapian::Query::OP_OR, emptyterms.begin(), emptyterms.end());
    TEST(Xapian::Query(Xapian::Query::OP_AND, query, Xapian::Query("x")).empty());
    TEST(Xapian::Query(Xapian::Query::OP_AND, query, Xapian::Query("x")).get_length() == 0);
    TEST(!Xapian::Query(Xapian::Query::OP_OR, query, Xapian::Query("x")).empty());
    TEST(Xapian::Query(Xapian::Query::OP_OR, query, Xapian::Query("x")).get_length() == 1);
    return true;
}

DEFINE_TESTCASE(singlesubq1, !backend) {
    vector<string> oneterm;
    oneterm.push_back("solo");
    Xapian::Query q_eliteset(Xapian::Query::OP_ELITE_SET, oneterm.begin(), oneterm.end(), 1);
    Xapian::Query q_near(Xapian::Query::OP_NEAR, oneterm.begin(), oneterm.end(), 1);
    Xapian::Query q_phrase(Xapian::Query::OP_PHRASE, oneterm.begin(), oneterm.end(), 1);
    return true;
}

DEFINE_TESTCASE(stemlangs1, !backend) {
    string langs = Xapian::Stem::get_available_languages();
    tout << "available languages '" << langs << "'" << endl;
    TEST(!langs.empty());

    // Also test the language codes.
    langs += " da nl en fi fr de hu it no pt ro ru es sv tr";

    string::size_type i = 0;
    while (true) {
	string::size_type spc = langs.find(' ', i);
	// The only spaces in langs should be a single one between each pair
	// of language names.
	TEST_NOT_EQUAL(i, spc);

	// Try making a stemmer for this language.  We should be able to create
	// it without an exception being thrown.
	string language = langs.substr(i, spc - i);
	tout << "checking language code '" << language << "' works" << endl;
	Xapian::Stem stemmer(language);

	if (spc == string::npos) break;
	i = spc + 1;
    }

    // Check that we get an exception for a bogus language name.
    TEST_EXCEPTION(Xapian::InvalidArgumentError, Xapian::Stem stemmer("bogus"));

    // Stem("") should give an object which doesn't change any input.
    Xapian::Stem stem_nothing = Xapian::Stem("");

    return true;
}

// Some simple tests of the built in weighting schemes.
DEFINE_TESTCASE(weight1, !backend) {
    Xapian::Weight * wt;

    Xapian::BoolWeight boolweight;
    TEST_EQUAL(boolweight.name(), "Bool");
    wt = Xapian::BoolWeight().unserialise(boolweight.serialise());
    TEST_EQUAL(boolweight.serialise(), wt->serialise());
    delete wt;

    Xapian::TradWeight tradweight_dflt;
    Xapian::TradWeight tradweight(1.0);
    TEST_EQUAL(tradweight.name(), "Trad");
    TEST_EQUAL(tradweight_dflt.serialise(), tradweight.serialise());
    wt = Xapian::TradWeight().unserialise(tradweight.serialise());
    TEST_EQUAL(tradweight.serialise(), wt->serialise());
    delete wt;

    Xapian::TradWeight tradweight2(2.0);
    TEST_NOT_EQUAL(tradweight.serialise(), tradweight2.serialise());

    Xapian::BM25Weight bm25weight_dflt;
    Xapian::BM25Weight bm25weight(1, 0, 1, 0.5, 0.5);
    TEST_EQUAL(bm25weight.name(), "BM25");
    TEST_EQUAL(bm25weight_dflt.serialise(), bm25weight.serialise());
    wt = Xapian::BM25Weight().unserialise(bm25weight.serialise());
    TEST_EQUAL(bm25weight.serialise(), wt->serialise());
    delete wt;

    Xapian::BM25Weight bm25weight2(1, 0.5, 1, 0.5, 0.5);
    TEST_NOT_EQUAL(bm25weight.serialise(), bm25weight2.serialise());

    return true;
}

// Regression test.
DEFINE_TESTCASE(nosuchdb1, !backend) {
    // This is a "nodb" test because it doesn't test a particular backend.
    TEST_EXCEPTION(Xapian::DatabaseOpeningError,
		   Xapian::Database db("NOsuChdaTabASe"));
    return true;
}

// Feature tests for value manipulations.
DEFINE_TESTCASE(addvalue1, !backend) {
    // Regression test for add_value on an existing value (bug#82).
    Xapian::Document doc;
    doc.add_value(1, "original");
    doc.add_value(1, "replacement");
    TEST_EQUAL(doc.get_value(1), "replacement");

    doc.add_value(2, "too");
    doc.add_value(3, "free");
    doc.add_value(4, "for");

    doc.remove_value(2);
    doc.remove_value(4);
    TEST_EQUAL(doc.get_value(0), "");
    TEST_EQUAL(doc.get_value(1), "replacement");
    TEST_EQUAL(doc.get_value(2), "");
    TEST_EQUAL(doc.get_value(3), "free");
    TEST_EQUAL(doc.get_value(4), "");

    return true;
}

// tests that the collapsing on termpos optimisation gives correct query length
DEFINE_TESTCASE(poscollapse2, !backend) {
    Xapian::Query q(Xapian::Query::OP_OR, Xapian::Query("this", 1, 1), Xapian::Query("this", 1, 1));
    TEST_EQUAL(q.get_length(), 2);
    return true;
}

// regression test of querying an uninitialised database: should report an
// error; used to segfault with 1.0.0.
DEFINE_TESTCASE(uninitdb1, !backend) {
    Xapian::Database db;
    TEST_EXCEPTION(Xapian::InvalidArgumentError,
		   Xapian::Enquire enq(db));
    return true;
}

// Test a scaleweight query applied to a match nothing query
DEFINE_TESTCASE(scaleweight3, !backend) {
    Xapian::Query matchnothing(Xapian::Query::MatchNothing);
    Xapian::Query query(Xapian::Query::OP_SCALE_WEIGHT, matchnothing, 3.0);
    TEST_EQUAL(query.get_description(), "Xapian::Query()");
    return true;
}

// Test that scaling by a weight close to 1 is optimised away.
DEFINE_TESTCASE(scaleweight4, !backend) {
    // Factor is a double which, when multiplied by its reciprocal, doesn't
    // give exactly 1.0
    double factor = 179.76931348623157e306;
    double nearly1 = factor * (1.0 / factor);

    TEST_NOT_EQUAL(nearly1, 1.0);
    Xapian::Query foo("foo");
    Xapian::Query foo_nearly1(Xapian::Query::OP_SCALE_WEIGHT, foo, nearly1);
    TEST_EQUAL(foo_nearly1.get_description(), "Xapian::Query(foo)");

    return true;
}

// Regression test - RSet::get_description() gave a malformed answer in 1.0.7.
DEFINE_TESTCASE(rset4, !backend) {
    Xapian::RSet rset;
    rset.add_document(1);
    // In 1.0.7 this gave: RSet(RSet(RSet::Internal(, 1))
    TEST_STRINGS_EQUAL(rset.get_description(), "RSet(RSet::Internal(1))");
    return true;
}

// Check that Query(OP_VALUE_GE, 0, "") -> Query::MatchAll.
DEFINE_TESTCASE(opvaluege1, !backend) {
    Xapian::Query query(Xapian::Query::OP_VALUE_GE, 0, "");
    TEST_STRINGS_EQUAL(query.get_description(), Xapian::Query::MatchAll.get_description());
    return true;
}

/// Regression test - this threw AssertionError in 1.0.9 and earlier (bug#201).
DEFINE_TESTCASE(nearsubqueries1, !backend) {
    Xapian::Query a_or_b(Xapian::Query::OP_OR,
			 Xapian::Query("a"),
			 Xapian::Query("b"));
    TEST_EXCEPTION(Xapian::UnimplementedError,
		   Xapian::Query near(Xapian::Query::OP_NEAR, a_or_b, a_or_b));
    return true;
}
