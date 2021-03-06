// deprecated.h: Define XAPIAN_DEPRECATED() macro.
//
// Copyright (C) 2006,2007 Olly Betts
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

#ifndef XAPIAN_INCLUDED_DEPRECATED_H
#define XAPIAN_INCLUDED_DEPRECATED_H

// How to use of the XAPIAN_DEPRECATED is document in HACKING - see the
// section "Marking Features as Deprecated".  Don't forget to update the
// documentation of deprecated methods for end users in docs/deprecation.rst
// too!

// xapian-bindings needs to wrap deprecated functions without warnings,
// so check if XAPIAN_DEPRECATED is defined so xapian-bindings can override
// it.
#ifndef XAPIAN_DEPRECATED
# if defined __GNUC__ && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0))
// __attribute__((__deprecated__)) is supported by GCC 3.1 and later.
#  define XAPIAN_DEPRECATED(D) D __attribute__((__deprecated__))
# elif defined _MSC_VER && _MSC_VER >= 1300
// __declspec(deprecated) is supported by MSVC 7.0 and later.
#  define XAPIAN_DEPRECATED(D) __declspec(deprecated) D
# else
#  define XAPIAN_DEPRECATED(D) D
# endif
#endif

#endif
