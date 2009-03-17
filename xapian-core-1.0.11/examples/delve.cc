/* delve.cc: Allow inspection of the contents of a Xapian database
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2002,2003,2004,2006,2007,2008 Olly Betts
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

#include <xapian.h>

#include <algorithm>
#include <iostream>
#include <vector>

#include "gnu_getopt.h"

#include <string.h>

using namespace Xapian;
using namespace std;

static char separator = ' ';

static bool verbose = false;
static bool showvalues = false;
static bool showdocdata = false;

#define PROG_NAME "delve"
#define PROG_DESC "Inspect the contents of a Xapian database"

static void show_usage() {
    cout << "Usage: "PROG_NAME" [OPTIONS] DATABASE...\n\n"
"Options:\n"
"  -r <recno>            for term list(s)\n"
"  -t <term>             for posting list(s)\n"
"  -t <term> -r <recno>  for position list(s)\n"
"  -s, --stemmer=LANG    set the stemming language, the default is 'none'\n"
"  -1                    output one list entry per line\n"
"  -V                    output values for each document referred to\n"
"  -V<valueno>           output value valueno for each document in the database\n"
"  -d                    output document data for each document referred to\n"
"  -v                    extra info (wdf and len for postlist;\n"
"                        wdf and termfreq for termlist; number of terms for db)\n"
"      --help            display this help and exit\n"
"      --version         output version information and exit" << endl;
}

static void
show_db_stats(Database &db)
{
    // Display a few database stats.
    cout << "number of documents = " << db.get_doccount() << endl;
    cout << "average document length = " << db.get_avlength() << endl;
    if (verbose) {
	// To find the number of terms, we have to count them!
	// This will take a few seconds or minutes, so only do it if -v
	// was specified.
	termcount terms = 0;
	TermIterator t = db.allterms_begin();
	const TermIterator end = db.allterms_end();
	while (t != end) {
	    ++terms;
	    ++t;
	}
	cout << "number of unique terms = " << terms << endl;
    }
}

static void
show_values(Database &db, docid docid, char sep)
{
    Document doc = db.get_document(docid);
    ValueIterator v = doc.values_begin();
    ValueIterator vend = doc.values_end();
    while (v != vend) {
	cout << sep << v.get_valueno() << ':' << *v;
	++v;
    }
}

static void
show_values(Database &db,
	    vector<docid>::const_iterator i,
	    vector<docid>::const_iterator end)
{
    while (i != end) {
	cout << "Values for record #" << *i << ':';
	show_values(db, *i, separator);
	cout << endl;
	++i;
    }
}

static void
show_docdata(Database &db, docid docid, char sep)
{
    cout << sep << "[" << db.get_document(docid).get_data() << ']';
}

static void
show_docdata(Database &db,
	     vector<docid>::const_iterator i,
	     vector<docid>::const_iterator end)
{
    while (i != end) {
	cout << "Data for record #" << *i << ':' << endl;
	cout << db.get_document(*i).get_data() << endl;
	++i;
    }
}

static void
show_termlists(Database &db,
	       vector<docid>::const_iterator i,
	       vector<docid>::const_iterator end)
{
    // Display termlists
    while (i != end) {
	TermIterator t = db.termlist_begin(*i);
	TermIterator tend = db.termlist_end(*i);
	cout << "Term List for record #" << *i << ':';
	while (t != tend) {
	    cout << separator << *t;
	    if (verbose) {
		cout << ' ' << t.get_wdf() << ' ' << t.get_termfreq();
	    }
	    ++t;
	}
	cout << endl;
	++i;
    }
}

static Stem stemmer;

int
main(int argc, char **argv)
{
    if (argc > 1 && argv[1][0] == '-') {
	if (strcmp(argv[1], "--help") == 0) {
	    cout << PROG_NAME" - "PROG_DESC"\n\n";
	    show_usage();
	    exit(0);
	}
	if (strcmp(argv[1], "--version") == 0) {
	    cout << PROG_NAME" - "PACKAGE_STRING << endl;
	    exit(0);
	}
    }

    vector<docid> recnos;
    vector<string> terms;
    vector<string> dbs;

    valueno valno = 0; // Avoid "may be used uninitialised" warnings.
    bool valno_set = false;

    int c;
    while ((c = gnu_getopt(argc, argv, "r:t:s:1vkV::d")) != -1) {
	switch (c) {
	    case 'r':
		recnos.push_back(atoi(optarg));
		break;
	    case 't':
		terms.push_back(optarg);
		break;
	    case 's':
		stemmer = Stem(optarg);
		break;
	    case '1':
		separator = '\n';
		break;
	    case 'V': case 'k': /* -k for backward compatibility */
		showvalues = true;
		if (optarg) {
		    valno = atoi(optarg);
		    valno_set = true;
		}
		break;
	    case 'd':
		showdocdata = true;
		break;
	    case 'v':
		verbose = true;
		break;
	    default:
		show_usage();
		exit(1);
	}
    }

    while (argv[optind]) dbs.push_back(argv[optind++]);

    if (dbs.empty()) {
	show_usage();
	exit(1);
    }

    std::sort(recnos.begin(), recnos.end());

    Database db;
    {
	vector<string>::const_iterator i;
	for (i = dbs.begin(); i != dbs.end(); i++) {
	    try {
		db.add_database(Database(*i));
	    } catch (const Error &e) {
		cout << "Error opening database `" << *i << "': ";
		cout << e.get_description() << endl;
		return 1;
	    }
	}
    }

    try {
	if (terms.empty() && recnos.empty() && !valno_set) {
	    show_db_stats(db);
	    return 0;
	}

	if (!recnos.empty()) {
	    if (showvalues) {
		show_values(db, recnos.begin(), recnos.end());
	    }

	    if (showdocdata) {
		show_docdata(db, recnos.begin(), recnos.end());
	    }
	}

	if (valno_set) {
	    doccount n = db.get_doccount();
	    docid did = 0;
	    docid hwm = db.get_lastdocid();
	    cout << "Value " << valno << " for each document:";
	    while (n && did != hwm) {
		try {
		    Document doc = db.get_document(++did);
		    string val = doc.get_value(valno);
		    if (!val.empty())
			cout << separator << did << ':' << doc.get_value(valno);
		    --n;
		} catch (DocNotFoundError &) {
		}
	    }
	    cout << endl;
	}

	if (terms.empty()) {
	    show_termlists(db, recnos.begin(), recnos.end());
	    return 0;
	}

	vector<string>::const_iterator i;
	for (i = terms.begin(); i != terms.end(); i++) {
	    string term = stemmer(*i);
	    PostingIterator p = db.postlist_begin(term);
	    PostingIterator pend = db.postlist_end(term);
	    if (p == pend) {
		cout << "term `" << term << "' not in database\n";
		continue;
	    }
	    if (recnos.empty()) {
		// Display posting list
		cout << "Posting List for term `" << term << "' (termfreq "
		     << db.get_termfreq(term) << ", collfreq "
		     << db.get_collection_freq(term) << "):";
		while (p != pend) {
		    cout << separator << *p;
		    if (verbose) {
			cout << ' ' << p.get_wdf()
			    << ' ' << p.get_doclength();
		    }
		    if (showvalues) show_values(db, *p, ' ');
		    if (showdocdata) show_docdata(db, *p, ' ');
		    p++;
		}
		cout << endl;
	    } else {
		// Display position lists
		vector<docid>::const_iterator j;
		for (j = recnos.begin(); j != recnos.end(); j++) {
		    p.skip_to(*j);
		    if (p == pend || *p != *j) {
			cout << "term `" << term <<
			    "' doesn't index document #" << *j << endl;
		    } else {
			cout << "Position List for term `" << term
			    << "', record #" << *j << ':';
			try {
			    PositionIterator pos = p.positionlist_begin();
			    PositionIterator posend = p.positionlist_end();
			    while (pos != posend) {
				cout << separator << *pos;
				++pos;
			    }
			    cout << endl;
			} catch (const Error &e) {
			    cout << "Error: " << e.get_description() << endl;
			}
		    }
		}
	    }
	}
    } catch (const Error &e) {
	cout << "\nError: " << e.get_description() << endl;
	return 1;
    }
}
