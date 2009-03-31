/* flint_cursor.cc: Btree cursor implementation
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002,2003,2004,2005,2006,2007 Olly Betts
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

#include "safeerrno.h"

#include <xapian/error.h>

#include "flint_cursor.h"
#include "flint_table.h"
#include "flint_btreeutil.h"
#include "omassert.h"
#include "omdebug.h"

#ifdef XAPIAN_DEBUG_VERBOSE
static string
hex_encode(const string & input)
{
    const char * table = "0123456789abcdef";
    string result;
    for (string::const_iterator i = input.begin(); i != input.end(); ++i) {
	unsigned char val = *i;
	result += "\\x";
	result += table[val/16];
	result += table[val%16];
    }

    return result;
}
#endif

#define DIR_START        11

FlintCursor::FlintCursor(FlintTable *B_)
	: is_positioned(false),
	  is_after_end(false),
	  tag_status(UNREAD),
	  B(B_),
	  level(B_->level)
{
    C = new Cursor_[level + 1];

    for (int j = 0; j < level; j++) {
        C[j].n = BLK_UNUSED;
	C[j].p = new byte[B->block_size];
    }
    C[level].n = B->C[level].n;
    C[level].p = B->C[level].p;
}

FlintCursor::~FlintCursor()
{
    // Use the value of level stored in the cursor rather than the
    // Btree, since the Btree might have been deleted already.
    for (int j = 0; j < level; j++) {
	delete [] C[j].p;
    }
    delete [] C;
}

bool
FlintCursor::prev()
{
    DEBUGCALL(DB, bool, "FlintCursor::prev", "");
    Assert(B->level <= level);
    Assert(!is_after_end);

    if (!is_positioned) {
	// We've read the last key and tag, and we're now not positioned.
	// Simple fix - seek to the current key, and then it's as if we
	// read the key but not the tag.
	(void)find_entry(current_key);
	tag_status = UNREAD;
    }

    if (tag_status != UNREAD) {
	while (true) {
	    if (! B->prev(C, 0)) {
		is_positioned = false;
		return false;
	    }
	    if (Item_(C[0].p, C[0].c).component_of() == 1) {
		break;
	    }
	}
    }

    while (true) {
	if (! B->prev(C, 0)) {
	    is_positioned = false;
	    return false;
	}
	if (Item_(C[0].p, C[0].c).component_of() == 1) {
	    break;
	}
    }
    get_key(&current_key);
    tag_status = UNREAD;

    DEBUGLINE(DB, "Moved to entry: key=`" << hex_encode(current_key) << "'");
    return true;
}

bool
FlintCursor::next()
{
    DEBUGCALL(DB, bool, "FlintCursor::next", "");
    Assert(B->level <= level);
    Assert(!is_after_end);
    if (tag_status == UNREAD) {
	while (true) {
	    if (! B->next(C, 0)) {
		is_positioned = false;
		break;
	    }
	    if (Item_(C[0].p, C[0].c).component_of() == 1) {
		is_positioned = true;
		break;
	    }
	}
    }

    if (!is_positioned) {
	is_after_end = true;
	return false;
    }

    get_key(&current_key);
    tag_status = UNREAD;

    DEBUGLINE(DB, "Moved to entry: key=`" << hex_encode(current_key) << "'");
    return true;
}

bool
FlintCursor::find_entry(const string &key)
{
    DEBUGCALL(DB, bool, "FlintCursor::find_entry", key);
    Assert(B->level <= level);

    is_after_end = false;

    bool found;

    is_positioned = true;
    if (key.size() > FLINT_BTREE_MAX_KEY_LEN) {
	// Can't find key - too long to possibly be present, so find the
	// truncated form but ignore "found".
	B->form_key(key.substr(0, FLINT_BTREE_MAX_KEY_LEN));
	(void)(B->find(C));
	found = false;
    } else {
	B->form_key(key);
	found = B->find(C);
    }

    if (!found) {
	if (C[0].c < DIR_START) {
	    C[0].c = DIR_START;
	    if (! B->prev(C, 0)) goto done;
	}
	while (Item_(C[0].p, C[0].c).component_of() != 1) {
	    if (! B->prev(C, 0)) {
		is_positioned = false;
		throw Xapian::DatabaseCorruptError("find_entry failed to find any entry at all!");
	    }
	}
    }
done:

    if (found)
	current_key = key;
    else
	get_key(&current_key);
    tag_status = UNREAD;

    DEBUGLINE(DB, "Found entry: key=`" << hex_encode(current_key) << "'");
    RETURN(found);
}

bool
FlintCursor::find_entry_ge(const string &key)
{
    DEBUGCALL(DB, bool, "FlintCursor::find_entry_ge", key);
    Assert(B->level <= level);

    is_after_end = false;

    bool found;

    is_positioned = true;
    if (key.size() > FLINT_BTREE_MAX_KEY_LEN) {
	// Can't find key - too long to possibly be present, so find the
	// truncated form but ignore "found".
	B->form_key(key.substr(0, FLINT_BTREE_MAX_KEY_LEN));
	(void)(B->find(C));
	found = false;
    } else {
	B->form_key(key);
	found = B->find(C);
    }

    if (found) {
	current_key = key;
    } else {
	if (! B->next(C, 0)) {
	    is_after_end = true;
	    is_positioned = false;
	    RETURN(false);
	}
	Assert(Item_(C[0].p, C[0].c).component_of() == 1);
	get_key(&current_key);
    }
    tag_status = UNREAD;

    DEBUGLINE(DB, "Found entry: key=`" << hex_encode(current_key) << "'");
    RETURN(found);
}

void
FlintCursor::get_key(string * key) const
{
    Assert(B->level <= level);
    Assert(is_positioned);

    (void)Item_(C[0].p, C[0].c).key().read(key);
}

bool
FlintCursor::read_tag(bool keep_compressed)
{
    DEBUGCALL(DB, bool, "FlintCursor::read_tag", keep_compressed);
    if (tag_status == UNREAD) {
	Assert(B->level <= level);
	Assert(is_positioned);

	if (B->read_tag(C, &current_tag, keep_compressed)) {
	    tag_status = COMPRESSED;
	} else {
	    tag_status = UNCOMPRESSED;
	}

	// We need to call B->next(...) after B->read_tag(...) so that the
	// cursor ends up on the next key.
	is_positioned = B->next(C, 0);

	DEBUGLINE(DB, "tag=`" << hex_encode(current_tag) << "'");
    }
    return (tag_status == COMPRESSED);
}

bool
FlintCursor::del()
{
    Assert(!is_after_end);

    B->del(current_key);

    // If we're iterating an older revision of the tree, then the deletion
    // happens in a new (uncommitted) revision and the cursor still sees
    // the deleted key.  But if we're iterating the new uncommitted revision
    // then the deleted key is no longer visible.  We need to handle both
    // cases - either find_entry_ge() finds the deleted key or not.
    if (!find_entry_ge(current_key)) return is_positioned;
    return next();
}
