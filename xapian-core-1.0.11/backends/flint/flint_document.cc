/* flint_document.cc: Implementation of document for Flint database
 *
 * ----START-LICENCE----
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2003,2004 Olly Betts
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 * -----END-LICENCE-----
 */

#include <config.h>
#include "omdebug.h"
#include "flint_database.h"
#include "flint_document.h"
#include "flint_values.h"
#include "flint_record.h"

/** Create a FlintDocument: this is only called by
 *  FlintDatabase::open_document().
 */
FlintDocument::FlintDocument(Xapian::Internal::RefCntPtr<const Xapian::Database::Internal> database_,
			       const FlintValueTable *value_table_,
			       const FlintRecordTable *record_table_,
			       Xapian::docid did_, bool lazy)
	: Xapian::Document::Internal(database_.get(), did_),
	  database(database_),
	  value_table(value_table_),
	  record_table(record_table_)
{
    DEBUGCALL(DB, void, "FlintDocument", "[database_], " << value_table_ << ", " << record_table_ << ", " << did_ << ", " << lazy);
    // FIXME: this should work but isn't great - in fact I wonder if
    // we should cache the results anyway...
    if (!lazy) (void)record_table->get_record(did);
}

/** Destroy a FlintDocument.
 */
FlintDocument::~FlintDocument()
{
    DEBUGCALL(DB, void, "~FlintDocument", "");
}

/** Retrieve a value from the database
 *
 *  @param valueid	The value number to retrieve.
 */
string
FlintDocument::do_get_value(Xapian::valueno valueid) const
{
    DEBUGCALL(DB, string, "FlintDocument::do_get_value", valueid);
    string retval;
    value_table->get_value(retval, did, valueid);
    RETURN(retval);
}

/** Retrieve all value values from the database
 */
map<Xapian::valueno, string>
FlintDocument::do_get_all_values() const
{
    DEBUGCALL(DB, void, "FlintDocument::do_get_all_values", "");
    map<Xapian::valueno, string> value_map;
    value_table->get_all_values(value_map, did);
    return value_map;
}

/** Retrieve the document data from the database
 */
string
FlintDocument::do_get_data() const
{
    DEBUGCALL(DB, string, "FlintDocument::do_get_data", "");
    RETURN(record_table->get_record(did));
}
