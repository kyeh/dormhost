%{
/* python/util.i: custom Python typemaps for xapian-bindings
 *
 * Copyright (C) 1999,2000,2001 BrightStation PLC
 * Copyright (C) 2002 Ananova Ltd
 * Copyright (C) 2002,2003 James Aylett
 * Copyright (C) 2002,2003,2004,2005,2006,2007 Olly Betts
 * Copyright (C) 2007 Lemur Consulting Ltd
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
%}

/* Include the documentation comments extracted from doxygen output. */
#ifdef DOCCOMMENTS_I_SOURCES
%include "doccomments.i"
#endif

/* Include overrides for the documentation comments. */
%include "extracomments.i"


// Use SWIG directors for Python wrappers.
#define XAPIAN_SWIG_DIRECTORS

%include typemaps.i
%include stl.i

/* For the 1.0 series, we support get_description() as a deprecated method.
 * This will be removed in release 1.1.0
 */
namespace Xapian {
    %extend Database { std::string get_description() const { return self->get_description(); } }
    %extend Document { std::string get_description() const { return self->get_description(); } }
    %extend ESet { std::string get_description() const { return self->get_description(); } }
    %extend ESetIterator { std::string get_description() const { return self->get_description(); } }
    %extend Enquire { std::string get_description() const { return self->get_description(); } }
    %extend MSet { std::string get_description() const { return self->get_description(); } }
    %extend MSetIterator { std::string get_description() const { return self->get_description(); } }
    %extend PositionIterator { std::string get_description() const { return self->get_description(); } }
    %extend PostingIterator { std::string get_description() const { return self->get_description(); } }
    %extend Query { std::string get_description() const { return self->get_description(); } }
    %extend QueryParser { std::string get_description() const { return self->get_description(); } }
    %extend RSet { std::string get_description() const { return self->get_description(); } }
    %extend Stem { std::string get_description() const { return self->get_description(); } }
    %extend Stopper { std::string get_description() const { return self->get_description(); } }
    %extend TermIterator { std::string get_description() const { return self->get_description(); } }
    %extend ValueIterator { std::string get_description() const { return self->get_description(); } }
    %extend WritableDatabase { std::string get_description() const { return self->get_description(); } }
}
/* Use get_description() methods for str(). */
%rename(__str__) get_description;


%{
namespace Xapian {
    class PythonProblem {};
    Query *get_py_query(PyObject *obj) {
#if PY_VERSION_HEX < 0x02050000
	// In Python 2.4 (and presumably earlier), PyObject_GetAttrString()
	// takes a char* parameter which causes a warning with GCC >= 4.2.
	// This is fixed in Python 2.5.
	PyObject * mythis = PyObject_GetAttrString(obj, (char *)"this");
#else
	PyObject * mythis = PyObject_GetAttrString(obj, "this");
#endif
	if (!mythis)
	    return 0;

	Query * retval = 0;
	int res = SWIG_ConvertPtr(mythis, (void **)&retval,
				  SWIGTYPE_p_Xapian__Query, 0);
	if (!SWIG_IsOK(res)) {
	    retval = 0;
	}
	Py_DECREF(mythis);
	return retval;
    }
}
%}

#define XAPIAN_MIXED_VECTOR_QUERY_INPUT_TYPEMAP
%typemap(typecheck, precedence=500) const vector<Xapian::Query> & {
    if (!PySequence_Check($input)) {
	$1 = 0;
    } else {
	$1 = 1;
	PyObject * fastseq = PySequence_Fast($input, "expected sequence of strings or queries");
	if (!fastseq) {
	    // We've already checked that we have a sequence, so the failure of
	    // PySequence_Fast() is a serious error, not just a failure of
	    // typecheck.
	    SWIG_fail;
	}
	int numitems = PySequence_Fast_GET_SIZE(fastseq);
	for (int i = 0; i < numitems; ++i) {
	    PyObject *obj = PySequence_Fast_GET_ITEM(fastseq, i);
	    if (!PyUnicode_Check(obj) && !PyString_Check(obj) && !Xapian::get_py_query(obj)) {
		$1 = 0;
		break;
	    }
	}
	Py_DECREF(fastseq);
    }
}

%typemap(in) const vector<Xapian::Query> & (vector<Xapian::Query> v) {
    PyObject * fastseq = PySequence_Fast($input, "expected sequence of strings or queries");
    if (!fastseq) {
	SWIG_fail;
    }

    int numitems = PySequence_Fast_GET_SIZE(fastseq);
    v.reserve(numitems);
    for (int i = 0; i < numitems; ++i) {
	PyObject *obj = PySequence_Fast_GET_ITEM(fastseq, i);
	PyObject *decrefme = NULL;
	if (PyUnicode_Check(obj)) {
	    PyObject *strobj = PyUnicode_EncodeUTF8(PyUnicode_AS_UNICODE(obj), PyUnicode_GET_SIZE(obj), "ignore");
	    if (!strobj) SWIG_fail;
	    obj = strobj;
	    decrefme = strobj;
	}
	if (PyString_Check(obj)) {
	    char * p;
	    Py_ssize_t len;
	    /* We know this must be a string, so this call can't fail. */
	    (void)PyString_AsStringAndSize(obj, &p, &len);
	    v.push_back(Xapian::Query(string(p, len)));
	} else {
	    Xapian::Query *subqp = Xapian::get_py_query(obj);
	    if (!subqp) {
		PyErr_SetString(PyExc_TypeError, "expected string or query");
		Py_XDECREF(decrefme);
		SWIG_fail;
	    }
	    v.push_back(*subqp);
	}
	Py_XDECREF(decrefme);
    }
    $1 = &v;
    Py_DECREF(fastseq);
}

#define XAPIAN_TERMITERATOR_PAIR_OUTPUT_TYPEMAP
%typemap(out) std::pair<Xapian::TermIterator, Xapian::TermIterator> {
    $result = PyList_New(0);
    if ($result == 0) {
	return NULL;
    }

    for (Xapian::TermIterator i = $1.first; i != $1.second; ++i) {
	PyObject * str = PyString_FromStringAndSize((*i).data(), (*i).size());
	if (str == 0) return NULL;
	if (PyList_Append($result, str) == -1) return NULL;
    }
}

%typedef PyObject *LangSpecificListType;

%inline %{
#define MSET_DID 0
#define MSET_WT 1
#define MSET_RANK 2
#define MSET_PERCENT 3
#define MSET_DOCUMENT 4

#define ESET_TNAME 0
#define ESET_WT 1
%}

%feature("nothread") Xapian::MSet::items;
%{
/* The GIL must be held when this is called. */
PyObject *Xapian_MSet_items_get(Xapian::MSet *mset)
{
    PyObject *retval = PyList_New(0);
    if (retval == 0) {
	return NULL;
    }

    for (Xapian::MSetIterator i = mset->begin(); i != mset->end(); ++i) {
	PyObject *t = PyTuple_New(4);
	if (!t) return NULL;

	PyTuple_SetItem(t, MSET_DID, PyInt_FromLong(*i));
	PyTuple_SetItem(t, MSET_WT, PyFloat_FromDouble(i.get_weight()));
	PyTuple_SetItem(t, MSET_RANK, PyInt_FromLong(i.get_rank()));
	PyTuple_SetItem(t, MSET_PERCENT, PyInt_FromLong(i.get_percent()));

	if (PyList_Append(retval, t) == -1) return NULL;
    }
    return retval;
}
%}

%feature("nothread") Xapian::ESet::items;
%{
/* The GIL must be held when this is called. */
PyObject *Xapian_ESet_items_get(Xapian::ESet *eset)
{
    PyObject *retval = PyList_New(0);
    if (retval == 0) {
	return NULL;
    }

    for (Xapian::ESetIterator i = eset->begin(); i != eset->end(); ++i) {
	PyObject *t = PyTuple_New(2);
	if (!t) return NULL;

	PyObject * str = PyString_FromStringAndSize((*i).data(), (*i).size());
	if (str == 0) return NULL;
	PyTuple_SetItem(t, ESET_TNAME, str);
	PyTuple_SetItem(t, ESET_WT, PyFloat_FromDouble(i.get_weight()));

	if (PyList_Append(retval, t) == -1) return NULL;
    }
    return retval;
}
%}

%typemap(memberout) PyObject *items {
    $result = PyList_New(0);
    if ($result == 0) {
	return NULL;
    }

    for (Xapian::MSetIterator i = $1.begin(); i != $1.end(); ++i) {
	PyObject *t = PyTuple_New(2);
	if (!t) return NULL;

	PyTuple_SetItem(t, MSET_DID, PyInt_FromLong(*i));
	PyTuple_SetItem(t, MSET_WT, PyFloat_FromDouble(i->get_weight()));

	if (PyList_Append($result, t) == -1) return NULL;
    }
%}

namespace Xapian {
    %extend TermIterator {
	bool __eq__(const TermIterator &other) {
	    return (*self)==other;
	}
	bool __ne__(const TermIterator &other) {
	    return (*self)!=other;
	}
    }

    %extend PositionIterator {
	bool __eq__(const PositionIterator &other) {
	    return (*self)==other;
	}
	bool __ne__(const PositionIterator &other) {
	    return (*self)!=other;
	}
    }

    %extend PostingIterator {
	bool __eq__(const PostingIterator &other) {
	    return (*self)==other;
	}
	bool __ne__(const PostingIterator &other) {
	    return (*self)!=other;
	}
    }

    %extend ValueIterator {
	bool __eq__(const ValueIterator &other) {
	    return (*self)==other;
	}
	bool __ne__(const ValueIterator &other) {
	    return (*self)!=other;
	}
    }

    %extend MSetIterator {
	bool __eq__(const MSetIterator &other) {
	    return (*self)==other;
	}
	bool __ne__(const MSetIterator &other) {
	    return (*self)!=other;
	}
    }

    %extend ESetIterator {
	bool __eq__(const ESetIterator &other) {
	    return (*self)==other;
	}
	bool __ne__(const ESetIterator &other) {
	    return (*self)!=other;
	}
    }

    %extend MSet {
	%immutable;
	// access to the items array
	PyObject *items;
	%mutable;

	// for comparison
	int __cmp__(const MSet &other) {
	    if (self->get_max_possible() != other.get_max_possible()) {
		return (self->get_max_possible() < other.get_max_possible())? -1 : 1;
	    }
	    if (self->size() != other.size()) {
		return (self->size() < other.size())? -1 : 1;
	    }

	    for (size_t i=0; i<self->size(); ++i) {
		if (*(*self)[i] != *other[i]) {
		    return (*(*self)[i] < *other[i])? -1 : 1;
		}
		if ((*self)[i].get_weight() != other[i].get_weight()) {
		    return ((*self)[i].get_weight() < other[i].get_weight())? -1 : 1;
		}
	    }
	    return 0;
	}
    }

    //%apply LangSpecificListType items { PyObject *items }

    %extend ESet {
	%immutable;
	PyObject *items;
	%mutable;
    }
}

%{
/* Forward declaration. */
SWIGINTERN int
SWIG_AsPtr_std_string (PyObject * obj, std::string **val);

/* Utility function which works like SWIG_AsPtr_std_string, but
 * converts unicode strings to UTF-8 simple strings first. */
SWIGINTERN int
SWIG_anystring_as_ptr(PyObject ** obj, std::string **val)
{
    if (PyUnicode_Check(*obj)) {
	PyObject * strobj = PyUnicode_EncodeUTF8(PyUnicode_AS_UNICODE(*obj), PyUnicode_GET_SIZE(*obj), "ignore");
	if (strobj == NULL) return SWIG_ERROR;
	int res = SWIG_AsPtr_std_string(strobj, val);
	Py_DECREF(strobj);
	return res;
    } else {
	return SWIG_AsPtr_std_string(*obj, val);
    }
}
%}

/* These typemaps depends somewhat heavily on the internals of SWIG, so
 * might break with future versions of SWIG.
 */
%typemap(in) const std::string &(int res = SWIG_OLDOBJ) {
    std::string *ptr = (std::string *)0;
    res = SWIG_anystring_as_ptr(&($input), &ptr);
    if (!SWIG_IsOK(res)) {
	%argument_fail(res,"$type",$symname, $argnum); 
    }
    if (!ptr) {
	%argument_nullref("$type",$symname, $argnum); 
    }
    $1 = ptr;
}
%typemap(in) std::string {
    std::string *ptr = (std::string *)0;
    int res = SWIG_anystring_as_ptr(&($input), &ptr);
    if (!SWIG_IsOK(res) || !ptr) {
	%argument_fail((ptr ? res : SWIG_TypeError),"$type",$symname, $argnum); 
    }
    $1 = *ptr;
    if (SWIG_IsNewObj(res)) delete ptr;
}
%typemap(freearg,noblock=1,match="in") const std::string & {
    if (SWIG_IsNewObj(res$argnum)) %delete($1);
}
%typemap(typecheck,noblock=1,precedence=900) const std::string & {
    int res = SWIG_anystring_as_ptr(&($input), (std::string**)(0));
    $1 = SWIG_CheckState(res);

}

/* This typemap is only currently needed for returning a value from the
 * get_description() method of a Stopper subclass to a C++ caller, but might be
 * more generally useful in future.
 */
%typemap(directorout,noblock=1) std::string {
    std::string *swig_optr = 0;
    int swig_ores;
    {
	PyObject * tmp = $input;
	Py_INCREF(tmp);
	swig_ores = SWIG_anystring_as_ptr(&tmp, &swig_optr);
	Py_DECREF(tmp);
    }
    if (!SWIG_IsOK(swig_ores) || !swig_optr) {
	%dirout_fail((swig_optr ? swig_ores : SWIG_TypeError),"$type");
    }
    $result = *swig_optr;
    if (SWIG_IsNewObj(swig_ores)) %delete(swig_optr);
}

/** This pair of typemaps implements conversion of the return value of
 *  ValueRangeProcessor subclasses implemented in Python from a tuple of
 *  (valueno, begin, end) to a return value of valueno, and assigning the new
 *  values of begin and end to the parameters.
 */
%typemap(directorin,noblock=1) std::string & {
    $input = SWIG_From_std_string(static_cast< std::string >($1_name));
}
%typemap(directorout,noblock=1) Xapian::valueno {
    if (!PyTuple_Check($input)) {
        %dirout_fail(SWIG_TypeError, "($type, std::string, std::string)");
    }
    if (PyTuple_Size($input) != 3) {
        %dirout_fail(SWIG_IndexError, "($type, std::string, std::string)");
    }

    // Set the return value from the first item of the tuple.
    unsigned int swig_val;
    int swig_res = SWIG_AsVal_unsigned_SS_int(PyTuple_GET_ITEM((PyObject *)$input, 0), &swig_val);
    if (!SWIG_IsOK(swig_res)) {
        %dirout_fail(swig_res, "($type, std::string, std::string)");
    }
    c_result = static_cast< Xapian::valueno >(swig_val);

    // Set "begin" from the second item of the tuple.
    std::string *ptr = (std::string *)0;
    swig_res = SWIG_AsPtr_std_string(PyTuple_GET_ITEM((PyObject *)$input, 1), &ptr);
    if (!SWIG_IsOK(swig_res) || !ptr) {
        delete ptr;
        ptr = (std::string *)0;
	%dirout_fail((ptr ? swig_res : SWIG_TypeError),"($type, std::string, std::string)"); 
    }
    begin = *ptr;
    delete ptr;
    ptr = (std::string *)0;

    // Set "end" from the third item of the tuple.
    swig_res = SWIG_AsPtr_std_string(PyTuple_GET_ITEM((PyObject *)$input, 2), &ptr);
    if (!SWIG_IsOK(swig_res) || !ptr) {
        delete ptr;
        ptr = (std::string *)0;
	%dirout_fail((ptr ? swig_res : SWIG_TypeError),"($type, std::string, std::string)"); 
    }
    end = *ptr;
    delete ptr;
    ptr = (std::string *)0;
}

/* Extend ValueRangeProcessor to have a method with named parameters vrpbegin
 * and vrpend.  We only have to do this so that we have parameter names which
 * aren't used anywhere else, so that we can then write specific typemaps for
 * them.  If SWIG allowed us to apply a typemap only to a specific method, we
 * wouldn't need to do this. */
namespace Xapian {
    %extend ValueRangeProcessor {
        Xapian::valueno __call(std::string &vrpbegin, std::string &vrpend) {
            return (*self)(vrpbegin, vrpend);
        }
    }
}

/* These typemaps handle ValueRangeProcessors, which take non-const references
 * to std::string and modify the strings.  They rely on no other methods
 * existing which use the parameter names "vrpbegin" and "vrpend". */
%typemap(in) std::string &vrpbegin (std::string temp),
             std::string &vrpend (std::string temp) {
    std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string($input, &ptr);
    if (!SWIG_IsOK(res) || !ptr) {
	%argument_fail((ptr ? res : SWIG_TypeError),"$type",$symname, $argnum); 
    }
    temp = *ptr;
    $1 = &temp;
    if (SWIG_IsNewObj(res)) delete ptr;
}
%typemap(argout) std::string &vrpbegin {
    PyObject * str;
    PyObject * newresult;

    // Put the existing result into the first item of a new 3-tuple.
    newresult = PyTuple_New(3);
    if (newresult == 0) {
        Py_DECREF($result);
        $result = NULL;
        SWIG_fail;
    }
    PyTuple_SetItem(newresult, 0, $result);
    $result = newresult;

    str = PyString_FromStringAndSize($1->data(), $1->size());
    if (str == 0) {
        Py_DECREF($result);
        $result = NULL;
        SWIG_fail;
    }
    PyTuple_SetItem($result, 1, str);
}
%typemap(argout) std::string &vrpend {
    PyObject * str;

    str = PyString_FromStringAndSize($1->data(), $1->size());
    if (str == 0) {
        Py_DECREF($result);
        $result = NULL;
        SWIG_fail;
    }

    PyTuple_SetItem($result, 2, str);
}

/* vim:set syntax=cpp:set noexpandtab: */
