/* python/except.i: Custom Python exception handling.
 * Warning: This file is generated by ./generate-python-exceptions
 * - do not modify directly!
 *
 * Copyright 2007 Lemur Consulting Ltd
 * Copyright 2007 Olly Betts
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
namespace Xapian {

%exceptionclass Error;
%ignore Error::get_description;
%extend Error {
    std::string __str__() const {
	std::string desc($self->get_msg());
	if (!$self->get_context().empty()) {
	    desc += " (context: ";
	    desc += $self->get_context();
	    desc += ')';
	}
	if ($self->get_error_string()) {
	    desc += " (";
	    desc += $self->get_error_string();
	    desc += ')';
	}
	return desc;
    }
}
%exceptionclass LogicError;
%exceptionclass RuntimeError;
%exceptionclass AssertionError;
%exceptionclass InvalidArgumentError;
%exceptionclass InvalidOperationError;
%exceptionclass UnimplementedError;
%exceptionclass DatabaseError;
%exceptionclass DatabaseCorruptError;
%exceptionclass DatabaseCreateError;
%exceptionclass DatabaseLockError;
%exceptionclass DatabaseModifiedError;
%exceptionclass DatabaseOpeningError;
%exceptionclass DatabaseVersionError;
%exceptionclass DocNotFoundError;
%exceptionclass FeatureUnavailableError;
%exceptionclass InternalError;
%exceptionclass NetworkError;
%exceptionclass NetworkTimeoutError;
%exceptionclass QueryParserError;
%exceptionclass RangeError;
}
%include "xapian/error.h"

%{
// Use our own DirectorException class to signal python exceptions in director
// methods, to avoid the unwanted side effects of the default SWIG mechanism.
// This can be removed once we're using a SWIG version with the patch linked to
// from ticket #289 merged into it.
class XapianDirectorException {
};

static void XapianSetPythonException() {
    try {
	throw;
    } catch (XapianDirectorException &e) {
	/* This happens if a python error occurred in a director method.  The
	 * original Python error state should still be set, so we don't need to
	 * do anything. */
    } catch (Swig::DirectorException &e) {
	/* This happens if a director raised an exception.  The standard SWIG
	 * director exception handling code sets the Python error state, so we
	 * don't need to do anything. */
    } catch (const Xapian::RangeError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::RangeError(e)),
					     SWIGTYPE_p_Xapian__RangeError,
					     SWIG_POINTER_OWN),
			  "Xapian::RangeError",
			  SWIGTYPE_p_Xapian__RangeError);
    } catch (const Xapian::QueryParserError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::QueryParserError(e)),
					     SWIGTYPE_p_Xapian__QueryParserError,
					     SWIG_POINTER_OWN),
			  "Xapian::QueryParserError",
			  SWIGTYPE_p_Xapian__QueryParserError);
    } catch (const Xapian::NetworkTimeoutError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::NetworkTimeoutError(e)),
					     SWIGTYPE_p_Xapian__NetworkTimeoutError,
					     SWIG_POINTER_OWN),
			  "Xapian::NetworkTimeoutError",
			  SWIGTYPE_p_Xapian__NetworkTimeoutError);
    } catch (const Xapian::NetworkError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::NetworkError(e)),
					     SWIGTYPE_p_Xapian__NetworkError,
					     SWIG_POINTER_OWN),
			  "Xapian::NetworkError",
			  SWIGTYPE_p_Xapian__NetworkError);
    } catch (const Xapian::InternalError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::InternalError(e)),
					     SWIGTYPE_p_Xapian__InternalError,
					     SWIG_POINTER_OWN),
			  "Xapian::InternalError",
			  SWIGTYPE_p_Xapian__InternalError);
    } catch (const Xapian::FeatureUnavailableError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::FeatureUnavailableError(e)),
					     SWIGTYPE_p_Xapian__FeatureUnavailableError,
					     SWIG_POINTER_OWN),
			  "Xapian::FeatureUnavailableError",
			  SWIGTYPE_p_Xapian__FeatureUnavailableError);
    } catch (const Xapian::DocNotFoundError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DocNotFoundError(e)),
					     SWIGTYPE_p_Xapian__DocNotFoundError,
					     SWIG_POINTER_OWN),
			  "Xapian::DocNotFoundError",
			  SWIGTYPE_p_Xapian__DocNotFoundError);
    } catch (const Xapian::DatabaseVersionError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DatabaseVersionError(e)),
					     SWIGTYPE_p_Xapian__DatabaseVersionError,
					     SWIG_POINTER_OWN),
			  "Xapian::DatabaseVersionError",
			  SWIGTYPE_p_Xapian__DatabaseVersionError);
    } catch (const Xapian::DatabaseOpeningError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DatabaseOpeningError(e)),
					     SWIGTYPE_p_Xapian__DatabaseOpeningError,
					     SWIG_POINTER_OWN),
			  "Xapian::DatabaseOpeningError",
			  SWIGTYPE_p_Xapian__DatabaseOpeningError);
    } catch (const Xapian::DatabaseModifiedError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DatabaseModifiedError(e)),
					     SWIGTYPE_p_Xapian__DatabaseModifiedError,
					     SWIG_POINTER_OWN),
			  "Xapian::DatabaseModifiedError",
			  SWIGTYPE_p_Xapian__DatabaseModifiedError);
    } catch (const Xapian::DatabaseLockError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DatabaseLockError(e)),
					     SWIGTYPE_p_Xapian__DatabaseLockError,
					     SWIG_POINTER_OWN),
			  "Xapian::DatabaseLockError",
			  SWIGTYPE_p_Xapian__DatabaseLockError);
    } catch (const Xapian::DatabaseCreateError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DatabaseCreateError(e)),
					     SWIGTYPE_p_Xapian__DatabaseCreateError,
					     SWIG_POINTER_OWN),
			  "Xapian::DatabaseCreateError",
			  SWIGTYPE_p_Xapian__DatabaseCreateError);
    } catch (const Xapian::DatabaseCorruptError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DatabaseCorruptError(e)),
					     SWIGTYPE_p_Xapian__DatabaseCorruptError,
					     SWIG_POINTER_OWN),
			  "Xapian::DatabaseCorruptError",
			  SWIGTYPE_p_Xapian__DatabaseCorruptError);
    } catch (const Xapian::DatabaseError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::DatabaseError(e)),
					     SWIGTYPE_p_Xapian__DatabaseError,
					     SWIG_POINTER_OWN),
			  "Xapian::DatabaseError",
			  SWIGTYPE_p_Xapian__DatabaseError);
    } catch (const Xapian::UnimplementedError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::UnimplementedError(e)),
					     SWIGTYPE_p_Xapian__UnimplementedError,
					     SWIG_POINTER_OWN),
			  "Xapian::UnimplementedError",
			  SWIGTYPE_p_Xapian__UnimplementedError);
    } catch (const Xapian::InvalidOperationError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::InvalidOperationError(e)),
					     SWIGTYPE_p_Xapian__InvalidOperationError,
					     SWIG_POINTER_OWN),
			  "Xapian::InvalidOperationError",
			  SWIGTYPE_p_Xapian__InvalidOperationError);
    } catch (const Xapian::InvalidArgumentError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::InvalidArgumentError(e)),
					     SWIGTYPE_p_Xapian__InvalidArgumentError,
					     SWIG_POINTER_OWN),
			  "Xapian::InvalidArgumentError",
			  SWIGTYPE_p_Xapian__InvalidArgumentError);
    } catch (const Xapian::AssertionError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::AssertionError(e)),
					     SWIGTYPE_p_Xapian__AssertionError,
					     SWIG_POINTER_OWN),
			  "Xapian::AssertionError",
			  SWIGTYPE_p_Xapian__AssertionError);
    } catch (const Xapian::RuntimeError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::RuntimeError(e)),
					     SWIGTYPE_p_Xapian__RuntimeError,
					     SWIG_POINTER_OWN),
			  "Xapian::RuntimeError",
			  SWIGTYPE_p_Xapian__RuntimeError);
    } catch (const Xapian::LogicError &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::LogicError(e)),
					     SWIGTYPE_p_Xapian__LogicError,
					     SWIG_POINTER_OWN),
			  "Xapian::LogicError",
			  SWIGTYPE_p_Xapian__LogicError);
    } catch (const Xapian::Error &e) {
	SWIG_Python_Raise(SWIG_NewPointerObj((new Xapian::Error(e)),
					     SWIGTYPE_p_Xapian__Error,
					     SWIG_POINTER_OWN),
			  "Xapian::Error",
			  SWIGTYPE_p_Xapian__Error);
    } catch (const std::exception& e) {
        SWIG_Error(SWIG_RuntimeError, e.what());
    } catch (...) {
	SWIG_Error(SWIG_UnknownError, "unknown error in Xapian");
    }
}
%}

/* Note that we have to re-acquire the GIL in the catch() here, because
 * XapianSetPythonException accesses Python data structures. */
%exception {
    try {
	$action
    } catch (...) {
	SWIG_PYTHON_THREAD_END_ALLOW;
	XapianSetPythonException();
	SWIG_fail;
    }
}

/* The following exception directives are for the methods defined by
 * director classes, which don't drop the GIL in the first place. */
%exception Xapian::MatchDecider::operator() {
    try {
	$action
    } catch (...) {
	XapianSetPythonException();
	SWIG_fail;
    }
}
%exception Xapian::ExpandDecider::operator() {
    try {
	$action
    } catch (...) {
	XapianSetPythonException();
	SWIG_fail;
    }
}
%exception Xapian::Stopper::operator() {
    try {
	$action
    } catch (...) {
	XapianSetPythonException();
	SWIG_fail;
    }
}
%exception Xapian::ValueRangeProcessor::operator() {
    try {
	$action
    } catch (...) {
	XapianSetPythonException();
	SWIG_fail;
    }
}
%exception Xapian::Sorter::operator() {
    try {
	$action
    } catch (...) {
	XapianSetPythonException();
	SWIG_fail;
    }
}
%exception Xapian::Stopper::get_description {
    try {
	$action
    } catch (...) {
	XapianSetPythonException();
	SWIG_fail;
    }
}

/* If a Python error is raised by a call to a director function, the following
 * code should cause a C++ exception to be thrown.
 */
%feature("director:except") {
    if ($error != NULL) {
	throw XapianDirectorException();
    }
}

/* vim:syntax=cpp:set noexpandtab: */
