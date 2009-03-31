/* omvalueiterator.cc
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2003,2007,2008 Olly Betts
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
#include <xapian/valueiterator.h>
#include "document.h"
#include "omassert.h"
#include "omdebug.h"
#include "utils.h"

namespace Xapian {

const string &
ValueIterator::operator *() const
{
    DEBUGAPICALL(const string &, "ValueIterator::operator*", "");
    Xapian::Internal::RefCntPtr<Xapian::Document::Internal> d(doc.internal);
    if (d->value_nos.empty()) {
	d->value_nos.reserve(d->values.size());
	map<Xapian::valueno, string>::const_iterator i;
	for (i = d->values.begin(); i != d->values.end(); ++i) {
	    d->value_nos.push_back(i->first);
	}
    }
    Assert(index < d->value_nos.size());
    RETURN(d->values[d->value_nos[index]]);
}

const string *
ValueIterator::operator->() const
{
    DEBUGAPICALL(const string *, "ValueIterator::operator->", "");
    Xapian::Internal::RefCntPtr<Xapian::Document::Internal> d(doc.internal);
    if (d->value_nos.empty()) {
	d->value_nos.reserve(d->values.size());
	map<Xapian::valueno, string>::const_iterator i;
	for (i = d->values.begin(); i != d->values.end(); ++i) {
	    d->value_nos.push_back(i->first);
	}
    }
    Assert(index < d->value_nos.size());
    RETURN(&(d->values[d->value_nos[index]]));
}

Xapian::valueno
ValueIterator::get_valueno() const
{
    DEBUGAPICALL(Xapian::valueno, "ValueIterator::get_valueno", "");
    Xapian::Internal::RefCntPtr<Xapian::Document::Internal> d(doc.internal);
    if (d->value_nos.empty()) {
	d->value_nos.reserve(d->values.size());
	map<Xapian::valueno, string>::const_iterator i;
	for (i = d->values.begin(); i != d->values.end(); ++i) {
	    d->value_nos.push_back(i->first);
	}
    }
    Assert(index < d->value_nos.size());
    RETURN(d->value_nos[index]);
}

std::string
ValueIterator::get_description() const
{
    return "ValueIterator(" + om_tostring(index) + ")";
}

}
