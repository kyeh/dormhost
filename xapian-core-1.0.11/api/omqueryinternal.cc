/* omqueryinternal.cc: Internals of query interface
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2002,2003,2004,2005,2006,2007,2008,2009 Olly Betts
 * Copyright 2006,2007,2008 Lemur Consulting Ltd
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

#include "autoptr.h"
#include "omdebug.h"
#include "omqueryinternal.h"
#include "utils.h"
#include "serialise.h"
#include "serialise-double.h"

#include <xapian/error.h>
#include <xapian/termiterator.h>
#include <xapian/version.h>
#include "vectortermlist.h"

#include <vector>
#include <set>
#include <algorithm>
#include <math.h>
#include <limits.h>
#include <cfloat>

using namespace std;

// Properties for query operations.

static unsigned int
get_min_subqs(Xapian::Query::Internal::op_t op)
{
    switch (op) {
	case Xapian::Query::Internal::OP_LEAF:
	case Xapian::Query::OP_AND:
	case Xapian::Query::OP_OR:
	case Xapian::Query::OP_XOR:
	case Xapian::Query::OP_NEAR:
	case Xapian::Query::OP_PHRASE:
	case Xapian::Query::OP_ELITE_SET:
	case Xapian::Query::OP_VALUE_RANGE:
	case Xapian::Query::OP_VALUE_GE:
	case Xapian::Query::OP_VALUE_LE:
	    return 0;
	case Xapian::Query::OP_SCALE_WEIGHT:
	    return 1;
	case Xapian::Query::OP_FILTER:
	case Xapian::Query::OP_AND_MAYBE:
	case Xapian::Query::OP_AND_NOT:
	    return 2;
	default:
	    Assert(false);
	    throw Xapian::InvalidOperationError("get_min_subqs called with invalid operator type");
    }
}

static unsigned int
get_max_subqs(Xapian::Query::Internal::op_t op)
{
    switch (op) {
	case Xapian::Query::Internal::OP_LEAF:
	case Xapian::Query::OP_VALUE_RANGE:
	case Xapian::Query::OP_VALUE_GE:
	case Xapian::Query::OP_VALUE_LE:
	    return 0;
	case Xapian::Query::OP_SCALE_WEIGHT:
	    return 1;
	case Xapian::Query::OP_FILTER:
	case Xapian::Query::OP_AND_MAYBE:
	case Xapian::Query::OP_AND_NOT:
	    return 2;
	case Xapian::Query::OP_AND:
	case Xapian::Query::OP_OR:
	case Xapian::Query::OP_XOR:
	case Xapian::Query::OP_NEAR:
	case Xapian::Query::OP_PHRASE:
	case Xapian::Query::OP_ELITE_SET:
	    return UINT_MAX;
	default:
	    Assert(false);
	    throw Xapian::InvalidOperationError("get_max_subqs called with invalid operator type");
    }
}

static inline bool
is_leaf(Xapian::Query::Internal::op_t op)
{
    return (op == Xapian::Query::Internal::OP_LEAF);
}

// Methods for Xapian::Query::Internal

/** serialising method, for network matches.
 *
 *  The format is designed to be relatively easy to parse.
 *
 *  A single-term query becomes `[<encodedtname>@<termpos>#<wqf>'
 *  where:
 *	<wqf> is the decimal within query frequency (1 if omitted),
 *	<termpos> is the decimal term position (index of term if omitted).
 *
 *  A compound query becomes `(<subqueries><op>', where:
 *	<subqueries> is the list of subqueries
 *	<op> is one of: &|%+-^
 *  also ~N "N >F *N (N unsigned int; F floating point)
 * 
 *  If querylen != sum(wqf) we append `=len' (at present we always do this
 *  for compound queries as it's simpler than working out what sum(wqf) would
 *  be - FIXME).
 */
string
Xapian::Query::Internal::serialise(Xapian::termpos & curpos) const
{
#ifdef XAPIAN_HAS_REMOTE_BACKEND
    string result;

    if (op == Xapian::Query::Internal::OP_LEAF) {
	result += '[';
	result += encode_length(tname.length());
	result += tname;
	if (term_pos != curpos) result += '@' + om_tostring(term_pos);
	if (wqf != 1) result += '#' + om_tostring(wqf);
	++curpos;
    } else {
	result += "(";
	for (subquery_list::const_iterator i = subqs.begin();
	     i != subqs.end();
	     ++i) {
	    result += (*i)->serialise(curpos);
	}
	switch (op) {
	    case Xapian::Query::Internal::OP_LEAF:
		Assert(false);
		break;
	    case Xapian::Query::OP_AND:
		result += "&";
		break;
	    case Xapian::Query::OP_OR:
		result += "|";
		break;
	    case Xapian::Query::OP_FILTER:
		result += "%";
		break;
	    case Xapian::Query::OP_AND_MAYBE:
		result += "+";
		break;
	    case Xapian::Query::OP_AND_NOT:
		result += "-";
		break;
	    case Xapian::Query::OP_XOR:
		result += "^";
		break;
	    case Xapian::Query::OP_NEAR:
		result += "~" + om_tostring(parameter);
		break;
	    case Xapian::Query::OP_PHRASE:
		result += "\"" + om_tostring(parameter);
		break;
	    case Xapian::Query::OP_ELITE_SET:
		result += "*" + om_tostring(parameter);
		break;
	    case Xapian::Query::OP_VALUE_RANGE:
		result += "]";
		result += encode_length(tname.length());
		result += tname;
		result += encode_length(str_parameter.length());
		result += str_parameter;
		result += om_tostring(parameter);
		break;
	    case Xapian::Query::OP_VALUE_GE:
		result += "}";
		result += encode_length(tname.length());
		result += tname;
		result += om_tostring(parameter);
		break;
	    case Xapian::Query::OP_VALUE_LE:
		result += "{";
		result += encode_length(tname.length());
		result += tname;
		result += om_tostring(parameter);
		break;
	    case Xapian::Query::OP_SCALE_WEIGHT:
		result += ".";
		result += str_parameter; // serialise_double(get_dbl_parameter());
		break;
	}
    }
    return result;
#else
    (void)curpos;
    throw Xapian::InternalError("query serialisation not compiled in");
#endif
}

string
Xapian::Query::Internal::get_op_name(Xapian::Query::Internal::op_t op)
{
    string name;
    switch (op) {
	case Xapian::Query::Internal::OP_LEAF:  name = "LEAF"; break;
	case Xapian::Query::OP_AND:             name = "AND"; break;
	case Xapian::Query::OP_OR:              name = "OR"; break;
	case Xapian::Query::OP_FILTER:          name = "FILTER"; break;
	case Xapian::Query::OP_AND_MAYBE:       name = "AND_MAYBE"; break;
	case Xapian::Query::OP_AND_NOT:         name = "AND_NOT"; break;
	case Xapian::Query::OP_XOR:             name = "XOR"; break;
	case Xapian::Query::OP_NEAR:            name = "NEAR"; break;
	case Xapian::Query::OP_PHRASE:          name = "PHRASE"; break;
	case Xapian::Query::OP_ELITE_SET:       name = "ELITE_SET"; break;
	case Xapian::Query::OP_VALUE_RANGE:     name = "VALUE_RANGE"; break;
	case Xapian::Query::OP_VALUE_GE:        name = "VALUE_GE"; break;
	case Xapian::Query::OP_VALUE_LE:        name = "VALUE_LE"; break;
	case Xapian::Query::OP_SCALE_WEIGHT:    name = "SCALE_WEIGHT"; break;
    }
    return name;
}

string
Xapian::Query::Internal::get_description() const
{
    string opstr;

    if (is_leaf(op)) {
	if (term_pos != 0) {
	    opstr += "pos=" + om_tostring(term_pos);
	}
	if (wqf != 1) {
	    if (!opstr.empty()) opstr += ",";
	    opstr += "wqf=" + om_tostring(wqf);
	}
	if (!opstr.empty()) opstr = ":(" + opstr + ")";
	if (tname.empty()) return "<alldocuments>" + opstr;
	return tname + opstr;
    }

    switch (op) {
	case Xapian::Query::OP_VALUE_RANGE: {
	    opstr = get_op_name(op);
	    opstr += ' ';
	    opstr += om_tostring(parameter);
	    opstr += ' ';
	    opstr += tname;
	    opstr += ' ';
	    opstr += str_parameter;
	    return opstr;
	}
	case Xapian::Query::OP_VALUE_GE:
	case Xapian::Query::OP_VALUE_LE: {
	    opstr = get_op_name(op);
	    opstr += ' ';
	    opstr += om_tostring(parameter);
	    opstr += ' ';
	    opstr += tname;
	    return opstr;
	}
	case Xapian::Query::OP_SCALE_WEIGHT: {
	    opstr += om_tostring(get_dbl_parameter());
	    opstr += " * ";
	    opstr += subqs[0]->get_description();
	    return opstr;
	}
    }

    opstr = " " + get_op_name(op) + " ";
    if (op == Xapian::Query::OP_NEAR ||
	op == Xapian::Query::OP_PHRASE ||
	op == Xapian::Query::OP_ELITE_SET)
	opstr += om_tostring(parameter) + " ";

    string description;
    subquery_list::const_iterator i;
    for (i = subqs.begin(); i != subqs.end(); i++) {
	if (!description.empty()) description += opstr;
	description += (**i).get_description();
    }

    return "(" + description + ")";
}

Xapian::termcount
Xapian::Query::Internal::get_length() const
{
    if (is_leaf(op)) return wqf;
    Xapian::termcount len = 0;
    subquery_list::const_iterator i;
    for (i = subqs.begin(); i != subqs.end(); ++i) {
	len += (**i).get_length();
    }
    return len;
}

/** Private function used to implement get_terms() */
void
Xapian::Query::Internal::accumulate_terms(
			vector<pair<string, Xapian::termpos> > &terms) const
{
    if (is_leaf(op)) {
	// We're a leaf, so just return our term, but skip Query::MatchAllTerms
	// (which is Query("")).
	if (!tname.empty())
	    terms.push_back(make_pair(tname, term_pos));
    } else {
	subquery_list::const_iterator end = subqs.end();
	// Not a leaf, concatenate results from all subqueries.
	for (subquery_list::const_iterator i = subqs.begin(); i != end; ++i) {
	    (*i)->accumulate_terms(terms);
	}
    }
}

struct LessByTermpos {
    typedef const pair<string, Xapian::termpos> argtype;
    bool operator()(argtype &left, argtype &right) {
	if (left.second != right.second) {
	    return left.second < right.second;
	} else {
	    return left.first < right.first;
	}
    }
};

Xapian::TermIterator
Xapian::Query::Internal::get_terms() const
{
    vector<pair<string, Xapian::termpos> > terms;
    accumulate_terms(terms);

    sort(terms.begin(), terms.end(), LessByTermpos());

    // remove adjacent duplicates, and return an iterator pointing
    // to just after the last unique element
    vector<pair<string, Xapian::termpos> >::iterator newlast =
		unique(terms.begin(), terms.end());
    // and remove the rest...  (See Stroustrup 18.6.3)
    terms.erase(newlast, terms.end());

    vector<string> result;
    vector<pair<string, Xapian::termpos> >::const_iterator i;
    for (i = terms.begin(); i != terms.end(); ++i) {
	result.push_back(i->first);
    }

    return Xapian::TermIterator(new VectorTermList(result.begin(),
						   result.end()));
}

#ifdef XAPIAN_HAS_REMOTE_BACKEND
// Methods.

class QUnserial {
  private:
    const char *p;
    const char *end;
    Xapian::termpos curpos;

    Xapian::Query::Internal * readquery();
    Xapian::Query::Internal * readcompound();

  public:
    QUnserial(const string & s) : p(s.c_str()), end(p + s.size()), curpos(1) { }
    Xapian::Query::Internal * decode();
};

Xapian::Query::Internal *
QUnserial::decode() {
    DEBUGLINE(UNKNOWN, "QUnserial::decode(" << p << ")");
    AutoPtr<Xapian::Query::Internal> qint(readquery());
    if (p != end)
        throw Xapian::InvalidArgumentError("Bad serialised query");
    return qint.release();
}

Xapian::Query::Internal *
QUnserial::readquery() {
    if (p == end)
	throw Xapian::InvalidArgumentError("Bad serialised query");
    switch (*p++) {
	case '[': {
	    size_t length = decode_length(&p, end, true);
	    string tname(p, length);
	    p += length;
	    Xapian::termpos term_pos = curpos;
	    Xapian::termcount wqf = 1;
	    if (p != end) {
		if (*p == '@') {
		    char *tmp; // avoid compiler warning
		    term_pos = strtol(p + 1, &tmp, 10);
		    p = tmp;
		}
		if (*p == '#') {
		    char *tmp; // avoid compiler warning
		    wqf = strtol(p + 1, &tmp, 10);
		    p = tmp;
		}
	    }
	    ++curpos;
	    return new Xapian::Query::Internal(tname, wqf, term_pos);
	}
	case '(':
	    return readcompound();
	default:
	    DEBUGLINE(UNKNOWN, "Can't parse remainder `" << p - 1 << "'");
	    throw Xapian::InvalidArgumentError("Invalid query string");
    }
}

static Xapian::Query::Internal *
qint_from_vector(Xapian::Query::op op,
		 const vector<Xapian::Query::Internal *> & vec,
		 Xapian::termcount parameter = 0)
{
    Xapian::Query::Internal * qint = new Xapian::Query::Internal(op, parameter);
    vector<Xapian::Query::Internal *>::const_iterator i;
    for (i = vec.begin(); i != vec.end(); i++) {
	qint->add_subquery_nocopy(*i);
    }
    qint->end_construction();
    return qint;
}

static Xapian::Query::Internal *
qint_from_vector(Xapian::Query::op op,
		 const vector<Xapian::Query::Internal *> & vec,
		 Xapian::termcount parameter,
		 double dbl_parameter)
{
    Xapian::Query::Internal * qint = new Xapian::Query::Internal(op, parameter);
    qint->set_dbl_parameter(dbl_parameter);
    vector<Xapian::Query::Internal *>::const_iterator i;
    for (i = vec.begin(); i != vec.end(); i++) {
	qint->add_subquery_nocopy(*i);
    }
    qint->end_construction();
    return qint;
}

Xapian::Query::Internal *
QUnserial::readcompound() {
    vector<Xapian::Query::Internal *> subqs;
    try {
        while (true) {
	    if (p == end)
	        throw Xapian::InvalidArgumentError("Bad serialised query");
	    switch (*p++) {
	        case '[':
		    --p;
		    subqs.push_back(readquery());
		    break;
	        case '(': {
		    subqs.push_back(readcompound());
		    break;
	        }
	        case '&':
		    return qint_from_vector(Xapian::Query::OP_AND, subqs);
	        case '|':
		    return qint_from_vector(Xapian::Query::OP_OR, subqs);
	        case '%':
		    return qint_from_vector(Xapian::Query::OP_FILTER, subqs);
	        case '^':
		    return qint_from_vector(Xapian::Query::OP_XOR, subqs);
	        case '+':
		    return qint_from_vector(Xapian::Query::OP_AND_MAYBE, subqs);
	        case '-':
		    return qint_from_vector(Xapian::Query::OP_AND_NOT, subqs);
	        case '~': {
		    char *tmp; // avoid compiler warning
		    Xapian::termcount window(strtol(p, &tmp, 10));
		    p = tmp;
		    return qint_from_vector(Xapian::Query::OP_NEAR, subqs, window);
	        }
	        case '"': {
		    char *tmp; // avoid compiler warning
		    Xapian::termcount window(strtol(p, &tmp, 10));
		    p = tmp;
		    return qint_from_vector(Xapian::Query::OP_PHRASE, subqs, window);
	        }
	        case '*': {
		    char *tmp; // avoid compiler warning
		    Xapian::termcount elite_set_size(strtol(p, &tmp, 10));
		    p = tmp;
		    return qint_from_vector(Xapian::Query::OP_ELITE_SET, subqs,
					    elite_set_size);
		}
		case ']': {
		    size_t len = decode_length(&p, end, true);
		    string start(p, len);
		    p += len;
		    len = decode_length(&p, end, true);
		    string stop(p, len);
		    p += len;
		    char *tmp; // avoid compiler warning
		    Xapian::valueno valno = strtoul(p, &tmp, 10);
		    p = tmp;
		    return new Xapian::Query::Internal(Xapian::Query::OP_VALUE_RANGE, valno,
						       start, stop);
	        }
		case '}': {
		    size_t len = decode_length(&p, end, true);
		    string start(p, len);
		    p += len;
		    char *tmp; // avoid compiler warning
		    Xapian::valueno valno = strtoul(p, &tmp, 10);
		    p = tmp;
		    return new Xapian::Query::Internal(Xapian::Query::OP_VALUE_GE, valno,
						       start);
	        }
		case '{': {
		    size_t len = decode_length(&p, end, true);
		    string start(p, len);
		    p += len;
		    char *tmp; // avoid compiler warning
		    Xapian::valueno valno = strtoul(p, &tmp, 10);
		    p = tmp;
		    return new Xapian::Query::Internal(Xapian::Query::OP_VALUE_LE, valno,
						       start);
	        }
	        case '.': {
		    double param = unserialise_double(&p, end);
		    return qint_from_vector(Xapian::Query::OP_SCALE_WEIGHT,
					    subqs, 0, param);
		}
	        default:
		    DEBUGLINE(UNKNOWN, "Can't parse remainder `" << p - 1 << "'");
		    throw Xapian::InvalidArgumentError("Invalid query string");
	    }
        }
    } catch (...) {
        vector<Xapian::Query::Internal *>::iterator i;
        for (i = subqs.begin(); i != subqs.end(); i++)
            delete *i;
        throw;
    }
}

Xapian::Query::Internal *
Xapian::Query::Internal::unserialise(const string &s)
{
    Assert(s.length() > 1);
    QUnserial u(s);
    Xapian::Query::Internal * qint = u.decode();
    AssertEq(s, qint->serialise());
    return qint;
}
#else
Xapian::Query::Internal *
Xapian::Query::Internal::unserialise(const string &)
{
    throw Xapian::InternalError("query serialisation not compiled in");
}
#endif

/** Swap the contents of this with another Xapian::Query::Internal,
 *  in a way which is guaranteed not to throw.  This is
 *  used with the assignment operator to make it exception
 *  safe.
 *  It's important to adjust swap with any addition of
 *  member variables!
 */
void
Xapian::Query::Internal::swap(Xapian::Query::Internal &other)
{
    std::swap(op, other.op);
    subqs.swap(other.subqs);
    std::swap(parameter, other.parameter);
    std::swap(tname, other.tname);
    std::swap(str_parameter, other.str_parameter);
    std::swap(term_pos, other.term_pos);
    std::swap(wqf, other.wqf);
}

Xapian::Query::Internal::Internal(const Xapian::Query::Internal &copyme)
	: Xapian::Internal::RefCntBase(),
	  op(copyme.op),
	  subqs(),
	  parameter(copyme.parameter),
	  tname(copyme.tname),
	  str_parameter(copyme.str_parameter),
	  term_pos(copyme.term_pos),
	  wqf(copyme.wqf)
{
    for (subquery_list::const_iterator i = copyme.subqs.begin();
	 i != copyme.subqs.end();
	 ++i) {
	subqs.push_back(new Xapian::Query::Internal(**i));
    }
}

//////////////////////////////////////////
// Methods for making new query objects

Xapian::Query::Internal::Internal(const string & tname_, Xapian::termcount wqf_,
		 Xapian::termpos term_pos_)
	: op(Xapian::Query::Internal::OP_LEAF),
	  subqs(),
	  parameter(0),
	  tname(tname_),
	  term_pos(term_pos_),
	  wqf(wqf_)
{
    validate_query();
}

Xapian::Query::Internal::Internal(op_t op_, Xapian::termcount parameter_)
	: op(op_),
	  subqs(),
	  parameter(parameter_),
	  tname(),
	  term_pos(0),
	  wqf(0)
{
    if (parameter != 0 && op != OP_PHRASE && op != OP_NEAR && op != OP_ELITE_SET)
	throw Xapian::InvalidArgumentError("parameter is only meaningful for OP_NEAR, OP_PHRASE, or OP_ELITE_SET");
}

Xapian::Query::Internal::Internal(op_t op_, Xapian::valueno valno,
				  const string &begin, const string &end)
	: op(op_),
	  parameter(Xapian::termcount(valno)),
	  tname(begin),
	  str_parameter(end)
{
    if (op != OP_VALUE_RANGE)
	throw Xapian::InvalidArgumentError("This constructor is only meaningful for OP_VALUE_RANGE");
    validate_query();
}

Xapian::Query::Internal::Internal(op_t op_, Xapian::valueno valno,
				  const std::string &value)
	: op(op_),
	  parameter(Xapian::termcount(valno)),
	  tname(value)
{
    if (op != OP_VALUE_GE && op != OP_VALUE_LE)
	throw Xapian::InvalidArgumentError("This constructor is only meaningful for OP_VALUE_GE or OP_VALUE_LE");
    if (op == OP_VALUE_GE && value.empty()) {
	// Map '<value> >= ""' to MatchAll.
	op = OP_LEAF;
	parameter = 0;
	term_pos = 0;
	wqf = 1;
    }
    validate_query();
}

Xapian::Query::Internal::~Internal()
{
    subquery_list::iterator i;
    for (i = subqs.begin(); i != subqs.end(); i++) {
        delete *i;
    }
}

Xapian::Query::Internal *
Xapian::Query::Internal::end_construction()
{
    DEBUGCALL(API, void, "Xapian::Query::Internal::end_construction", "");
    validate_query();
    Xapian::Query::Internal * qint = simplify_query();
    if (qint) qint->validate_query();
    return qint;
}

void
Xapian::Query::Internal::validate_query() const
{
    DEBUGCALL(API, void, "Xapian::Query::Internal::validate_query", "");

    // Check that the number of subqueries is in acceptable limits for this op
    if (subqs.size() < get_min_subqs(op) ||
	subqs.size() > get_max_subqs(op)) {
	throw Xapian::InvalidArgumentError("Xapian::Query: " + get_op_name(op) +
		" requires a minimum of " + om_tostring(get_min_subqs(op)) +
		" and a maximum of " + om_tostring(get_max_subqs(op)) +
		" sub queries, had " +
		om_tostring(subqs.size()) + ".");
    }

    if (op == OP_SCALE_WEIGHT && get_dbl_parameter() < 0) {
	throw Xapian::InvalidArgumentError("Xapian::Query: " + get_op_name(op) + " requires a non-negative parameter.");
    }

    // Check that the termname is null in a branch query, unless the op
    // is OP_VALUE_RANGE or OP_VALUE_GE or OP_VALUE_LE.
    Assert(is_leaf(op) ||
	   op == OP_VALUE_RANGE ||
	   op == OP_VALUE_GE ||
	   op == OP_VALUE_LE ||
	   tname.empty());
}

bool
Xapian::Query::Internal::simplify_matchnothing()
{
    subquery_list::iterator sq;
    switch (op) {
        case OP_PHRASE:
        case OP_NEAR:
        case OP_AND:
        case OP_FILTER:
            // Doing an "AND" type operation - if we've got any MatchNothing
            // nodes, we match nothing.
            for (sq = subqs.begin(); sq != subqs.end(); sq++) {
                if (*sq == 0) {
                    for (sq = subqs.begin(); sq != subqs.end(); sq++)
                        delete *sq;
                    subqs.clear();
                    return true;
                }
            }
            break;
        case OP_ELITE_SET:
        case OP_OR:
        case OP_XOR:
            // Doing an "OR" type operation - if we've got any MatchNothing
            // subnodes, drop them; except that we mustn't become an empty
            // node due to this, so we never drop a MatchNothing subnode
            // if it's the only subnode.
            sq = subqs.begin();
            while (sq != subqs.end() && subqs.size() > 1) {
                if (*sq == 0) {
                    sq = subqs.erase(sq);
                } else {
                    ++sq;
                }
            }
            break;
        case OP_AND_MAYBE:
	case OP_AND_NOT:
            // If left hand side is MatchNothing, we match nothing.
            // If right hand side is MatchNothing, replace node with LHS.
            // So, if either node is MatchNothing, replace node with LHS.
            // Easiest way to do this is to remove the right hand node,
            // and let simplify_query() perform the replacement of
            // the unary operator with its one remaining child.
            Assert(subqs.size() == 2);
            if (subqs[0] == 0 || subqs[1] == 0) {
                sq = subqs.begin();
                ++sq;
                delete *sq;
                subqs.erase(sq);
            }
            break;
        case OP_SCALE_WEIGHT:
            Assert(subqs.size() == 1);
	    // We should have already handled OP_SCALE_WEIGHT applied to
	    // MatchNothing in the relevant constructor.
	    Assert(subqs[0]);
	    break;
        case OP_LEAF:
            // Do nothing.
            break;
    }
    return false;
}

Xapian::Query::Internal *
Xapian::Query::Internal::simplify_query()
{
    DEBUGCALL(API, bool, "Xapian::Query::Internal::simplify_query", "");

    // Simplify any MatchNothing nodes.
    if (simplify_matchnothing()) {
	return 0;
    }

    // General simplifications, dependent on operator.
    switch (op) {
	case OP_LEAF:
	    return this;
	case OP_VALUE_RANGE:
	    // If the start of the range is greater than the end then we won't
	    // match anything.
	    if (tname > str_parameter) return 0;
	    return this;
	case OP_VALUE_GE:
	case OP_VALUE_LE:
	    return this;
	case OP_SCALE_WEIGHT:
	    if (fabs(get_dbl_parameter() - 1.0) > DBL_EPSILON) return this;
	    // If the multiplier is 1, this node doesn't actually do anything,
	    // so we leave it to be removed.
	    break;
	case OP_PHRASE: case OP_NEAR:
	    // Default to the number of subqueries.
	    if (!parameter) parameter = subqs.size();
	    // Flatten out sub queries if this is a phrase (or near) operation.
	    flatten_subqs();
	    break;
	case OP_ELITE_SET:
	    if (!parameter) {
		// Default to sqrt(number of subqueries), or a minimum of 10.
		// Gives a reasonable default.
		if (subqs.size() <= 100) {
		    parameter = 10;
		} else {
		    parameter = Xapian::termcount(ceil(sqrt(double(subqs.size()))));
		    Assert(parameter > 10);
		}
	    }
	    break;
	case OP_OR: case OP_AND: case OP_XOR:
	    // Remove duplicates if we can.
	    if (subqs.size() > 1) collapse_subqs();
	    break;
	default:
	    break;
    }

    // If we have no subqueries, then we're an empty query.
    if (subqs.empty())
	return 0;

    // Any nodes which are valid with only one subquery can be replaced by
    // that solitary subquery.
    if (subqs.size() == 1) {
	Xapian::Query::Internal * qint = subqs[0];
	subqs[0] = 0;
	return qint;
    }

    return this;
}

struct SortPosName {
    bool operator()(const Xapian::Query::Internal * left,
		    const Xapian::Query::Internal * right) const {
	Assert(left != 0);
	Assert(right != 0);
	Assert(is_leaf(left->op));
	Assert(is_leaf(right->op));
	if (left->term_pos != right->term_pos) {
	    return left->term_pos < right->term_pos;
	} else {
	    return left->tname < right->tname;
	}
    }
};

/** Collapse occurrences of a term at the same position into a
 *  single occurrence with higher wqf.
 */
void
Xapian::Query::Internal::collapse_subqs()
{
    Assert(op == OP_OR || op == OP_AND || op == OP_XOR);
    typedef set<Xapian::Query::Internal *, SortPosName> subqtable;
    subqtable sqtab;

    subquery_list::iterator sq = subqs.begin();
    while (sq != subqs.end()) {
	Assert(*sq != 0);
	if (is_leaf((*sq)->op)) {
	    Assert((*sq)->subqs.empty());
	    subqtable::iterator s = sqtab.find(*sq);
	    if (s == sqtab.end()) {
		sqtab.insert(*sq);
		++sq;
	    } else {
		AssertEq((*s)->tname, (*sq)->tname);
		AssertEq((*s)->term_pos, (*sq)->term_pos);
		(*s)->wqf += (*sq)->wqf;
		// Rather than incrementing sq, delete the current
		// element, as it has been merged into the other
		// equivalent term.
		delete *sq;
		sq = subqs.erase(sq);
	    }
	} else {
	    ++sq;
	}
    }
}

/// Change, eg, A NEAR (B AND C) to (A NEAR B) AND (A NEAR C)
void
Xapian::Query::Internal::flatten_subqs()
{
    if (op != Xapian::Query::OP_NEAR && op != Xapian::Query::OP_PHRASE) {
	throw Xapian::UnimplementedError("NEAR or PHRASE with non-term subqueries isn't well supported currently");
    }

    subquery_list::iterator sq;
    for (sq = subqs.begin(); sq != subqs.end(); sq++) {
	if (!is_leaf((*sq)->op)) break;
    }

    if (sq != subqs.end()) {
	if ((*sq)->op == Xapian::Query::OP_NEAR ||
	    (*sq)->op == Xapian::Query::OP_PHRASE) {
	    // FIXME: A PHRASE (B PHRASE C) -> (A PHRASE B) AND (B PHRASE C)?
	    throw Xapian::UnimplementedError("Can't use NEAR/PHRASE with a subexpression containing NEAR or PHRASE");
	}

	AutoPtr<Xapian::Query::Internal> flattenme(*sq);
	*sq = 0;

	// New query to build up.
	Xapian::Query::Internal newq(flattenme->op, 0);

	subquery_list::iterator j;
	for (j = flattenme->subqs.begin(); j != flattenme->subqs.end(); ++j) {
	    *sq = *j;
	    *j = 0;
	    flatten_subqs();
	    newq.add_subquery(this);
	    delete *sq;
	    *sq = 0;
	}

	Xapian::Query::Internal * newq2 = newq.end_construction();
	Assert(newq2);
	this->swap(*newq2);
    }
}

void
Xapian::Query::Internal::add_subquery(const Xapian::Query::Internal * subq)
{
    Assert(!is_leaf(op));
    if (subq == 0) {
	subqs.push_back(0);
    } else if (op == subq->op && (op == OP_AND || op == OP_OR || op == OP_XOR)) {
	// Distribute the subquery.
	for (subquery_list::const_iterator i = subq->subqs.begin();
	     i != subq->subqs.end(); i++) {
	    add_subquery(*i);
	}
    } else {
	subqs.push_back(new Xapian::Query::Internal(*subq));
    }
}

void
Xapian::Query::Internal::add_subquery_nocopy(Xapian::Query::Internal * subq)
{
    Assert(!is_leaf(op));
    if (subq == 0) {
	subqs.push_back(0);
    } else if (op == subq->op && (op == OP_AND || op == OP_OR || op == OP_XOR)) {
	// Distribute the subquery.
	for (subquery_list::const_iterator i = subq->subqs.begin();
	     i != subq->subqs.end(); i++) {
	    add_subquery(*i);
	}
	delete subq;
    } else {
	subqs.push_back(subq);
    }
}

void
Xapian::Query::Internal::set_dbl_parameter(double dbl_parameter_)
{
    // We store the double parameter encoded as a string because
    // Xapian::Query::Internal is defined in an external API header and we want
    // to avoid any risk of ABI breakage (we suspect it would be OK, but it's
    // not risking).  FIXME: rework for 1.1.0
    str_parameter = serialise_double(dbl_parameter_);
}

double
Xapian::Query::Internal::get_dbl_parameter() const
{
    // We store the double parameter encoded as a string because
    // Xapian::Query::Internal is defined in an external API header and we want
    // to avoid any risk of ABI breakage (we suspect it would be OK, but it's
    // not risking).  FIXME: rework for 1.1.0
    const char * p = str_parameter.data();
    const char * end = p + str_parameter.size();
    return unserialise_double(&p, end);
}
