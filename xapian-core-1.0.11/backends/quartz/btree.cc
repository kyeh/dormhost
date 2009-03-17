/* btree.cc: Btree implementation
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
 * Copyright 2002,2003,2004,2005,2006,2007,2008 Olly Betts
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

#include <xapian/error.h>

#include "safeerrno.h"
#include "safefcntl.h"
#ifdef __WIN32__
# include "msvc_posix_wrapper.h"
#endif

#include "stringutils.h" // For STRINGIZE().

// Define to use "dangerous" mode - in this mode we write modified btree
// blocks back in place.  This is somewhat faster (especially once we're
// I/O bound) but the database can't be safely searched during indexing
// and if indexing is terminated uncleanly, the database may be corrupt.
//
// Despite the risks, it's useful for speeding up a full rebuild.
//
// FIXME: make this mode run-time selectable, and record that it is currently
// in use somewhere on disk, so readers can check and refuse to open the
// database.
//
// #define DANGEROUS

#include <sys/types.h>

// Trying to include the correct headers with the correct defines set to
// get pread() and pwrite() prototyped on every platform without breaking any
// other platform is a real can of worms.  So instead we probe for what
// prototypes (if any) are required in configure and put them into
// PREAD_PROTOTYPE and PWRITE_PROTOTYPE.
#if defined HAVE_PREAD && defined PREAD_PROTOTYPE
PREAD_PROTOTYPE
#endif
#if defined HAVE_PWRITE && defined PWRITE_PROTOTYPE
PWRITE_PROTOTYPE
#endif

#include "safeunistd.h"

#include <stdio.h>
#include <string.h>   /* for memmove */
#include <limits.h>   /* for CHAR_BIT */

#include "btree.h"
#include "btree_util.h"
#include "btree_base.h"
#include "bcursor.h"
#include "quartz_utils.h"

#include "omassert.h"
#include "omdebug.h"
#include <xapian/error.h>
#include "utils.h"

#include <algorithm>  // for std::min()
#include <string>
#include <vector>

using std::min;
using std::string;
using std::vector;

//#define BTREE_DEBUG_FULL 1
#undef BTREE_DEBUG_FULL

#ifdef BTREE_DEBUG_FULL
/*------debugging aids from here--------*/

static void print_key(const byte * p, int c, int j);
static void print_tag(const byte * p, int c, int j);

/*
static void report_cursor(int N, Btree * B, Cursor * C)
{
    int i;
    printf("%d)\n", N);
    for (i = 0; i <= B->level; i++)
	printf("p=%d, c=%d, n=[%d], rewrite=%d\n",
		C[i].p, C[i].c, C[i].n, C[i].rewrite);
}
*/

/*------to here--------*/
#endif /* BTREE_DEBUG_FULL */

/* Input/output is defined with calls to the basic Unix system interface: */

int sys_open_to_read_no_except(const string & name)
{
#ifdef __WIN32__
    int fd = msvc_posix_open(name.c_str(), O_RDONLY | O_BINARY);
#else
    int fd = open(name.c_str(), O_RDONLY | O_BINARY);
#endif
    return fd;
}

int sys_open_to_read(const string & name)
{
    int fd = sys_open_to_read_no_except(name);
    if (fd < 0) {
	string message = string("Couldn't open ")
		+ name + " to read: " + strerror(errno);
	throw Xapian::DatabaseOpeningError(message);
    }
    return fd;
}

static int sys_open_to_write_no_except(const string & name)
{
#ifdef __WIN32__
    int fd = msvc_posix_open(name.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_BINARY);
#else
    int fd = open(name.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666);
#endif
    return fd;
}

int sys_open_to_write(const string & name)
{
    int fd = sys_open_to_write_no_except(name);
    if (fd < 0) {
	string message = string("Couldn't open ")
		+ name + " to write: " + strerror(errno);
	throw Xapian::DatabaseOpeningError(message);
    }
    return fd;
}

static int sys_open_for_readwrite(const string & name)
{
    int fd = open(name.c_str(), O_RDWR | O_BINARY);
    if (fd < 0) {
	string message = string("Couldn't open ")
		+ name + " read/write: " + strerror(errno);
	throw Xapian::DatabaseOpeningError(message);
    }
    return fd;
}

static void sys_write_bytes(int h, int n, const char * p)
{
    while (true) {
	ssize_t bytes_written = write(h, p, n);
	if (bytes_written == n) {
	    // normal case - write succeeded, so return.
	    return;
	} else if (bytes_written == -1) {
	    if (errno == EINTR) continue;
	    string message = "Error writing block: ";
	    message += strerror(errno);
	    throw Xapian::DatabaseError(message);
	} else if (bytes_written == 0) {
	    string message = "Error writing block: wrote no data";
	    throw Xapian::DatabaseError(message);
	} else if (bytes_written < n) {
	    /* Wrote part of the block, which is not an error.  We should
	     * continue writing the rest of the block.
	     */
	    n -= bytes_written;
	    p += bytes_written;
	}
    }
}

string sys_read_all_bytes(int h, size_t bytes_to_read)
{
    ssize_t bytes_read;
    string retval;
    while (true) {
	char buf[1024];
	bytes_read = read(h, buf, min(sizeof(buf), bytes_to_read));
	if (bytes_read > 0) {
	    // add byte to string, continue unless we reached max
	    retval.append(buf, bytes_read);
	    bytes_to_read -= bytes_read;
	    if (bytes_to_read == 0) {
		break;
	    }
	} else if (bytes_read == 0) {
	    // end of file, we're finished
	    break;
	} else if (bytes_read == -1) {
	    if (errno == EINTR) continue;
	    string message = "Error reading all bytes: ";
	    message += strerror(errno);
	    throw Xapian::DatabaseError(message);
	}
    }
    return retval;
}

void
sys_write_string(int h, const string &s)
{
    sys_write_bytes(h, s.length(), s.data());
}

int sys_flush(int h) {
#if defined HAVE_FDATASYNC
    return (fdatasync(h) != -1);
#elif defined HAVE_FSYNC
    return (fsync(h) != -1);
#elif defined __WIN32__
    return (_commit(h) != -1);
#else
#error "Have neither fsync() nor fdatasync() nor _commit() - cannot sync."
#endif
}

static void sys_unlink(const string &filename)
{
#ifdef __WIN32__
    if (msvc_posix_unlink(filename.c_str()) == -1) {
#else
    if (unlink(filename) == -1) {
#endif
	string message = "Failed to unlink ";
	message += filename;
	message += ": ";
	message += strerror(errno);
	throw Xapian::DatabaseCorruptError(message);
    }
}

/* There are two bit maps in bit_map0 and bit_map. The nth bit of bitmap is 0
   if the nth block is free, otherwise 1. bit_map0 is the initial state of
   bitmap at the start of the current transaction.

   Note use of the limits.h values:
   UCHAR_MAX = 255, an unsigned with all bits set, and
   CHAR_BIT = 8, the number of bits per byte

   BYTE_PAIR_RANGE below is the smallest +ve number that can't be held in two
   bytes -- 64K effectively.
*/

#define BYTE_PAIR_RANGE (1 << 2 * CHAR_BIT)

/// read_block(n, p) reads block n of the DB file to address p.
void
Btree::read_block(uint4 n, byte * p) const
{
    // Log the value of p, not the contents of the block it points to...
    DEBUGCALL(DB, void, "Btree::read_block", n << ", " << (void*)p);
    /* Use the base bit_map_size not the bitmap's size, because
     * the latter is uninitialised in readonly mode.
     */
    Assert(n / CHAR_BIT < base.get_bit_map_size());

#ifdef HAVE_PREAD
    off_t offset = off_t(block_size) * n;
    int m = block_size;
    while (true) {
	ssize_t bytes_read = pread(handle, reinterpret_cast<char *>(p), m,
				   offset);
	// normal case - read succeeded, so return.
	if (bytes_read == m) return;
	if (bytes_read == -1) {
	    if (errno == EINTR) continue;
	    string message = "Error reading block " + om_tostring(n) + ": ";
	    message += strerror(errno);
	    throw Xapian::DatabaseError(message);
	} else if (bytes_read == 0) {
	    string message = "Error reading block " + om_tostring(n) + ": got end of file";
	    throw Xapian::DatabaseError(message);
	} else if (bytes_read < m) {
	    /* Read part of the block, which is not an error.  We should
	     * continue reading the rest of the block.
	     */
	    m -= bytes_read;
	    p += bytes_read;
	    offset += bytes_read;
	}
    }
#else
    if (lseek(handle, off_t(block_size) * n, SEEK_SET) == -1) {
	string message = "Error seeking to block: ";
	message += strerror(errno);
	throw Xapian::DatabaseError(message);
    }

    int m = block_size;
    while (true) {
	ssize_t bytes_read = read(handle, reinterpret_cast<char *>(p), m);
	// normal case - read succeeded, so return.
	if (bytes_read == m) return;
	if (bytes_read == -1) {
	    if (errno == EINTR) continue;
	    string message = "Error reading block " + om_tostring(n) + ": ";
	    message += strerror(errno);
	    throw Xapian::DatabaseError(message);
	} else if (bytes_read == 0) {
	    string message = "Error reading block " + om_tostring(n) + ": got end of file";
	    throw Xapian::DatabaseError(message);
	} else if (bytes_read < m) {
	    /* Read part of the block, which is not an error.  We should
	     * continue reading the rest of the block.
	     */
	    m -= bytes_read;
	    p += bytes_read;
	}
    }
#endif
}

/** write_block(n, p) writes block n in the DB file from address p.
 *  When writing we check to see if the DB file has already been
 *  modified. If not (so this is the first write) the old base is
 *  deleted. This prevents the possibility of it being opened
 *  subsequently as an invalid base.
 */
void
Btree::write_block(uint4 n, const byte * p) const
{
    DEBUGCALL(DB, void, "Btree::write_block", n << ", " << p);
    Assert(writable);
    /* Check that n is in range. */
    Assert(n / CHAR_BIT < base.get_bit_map_size());

    /* don't write to non-free */;
    AssertParanoid(base.block_free_at_start(n));

    /* write revision is okay */
    AssertEqParanoid(REVISION(p), latest_revision_number + 1);

    if (both_bases) {
	// Delete the old base before modifying the database.
	sys_unlink(name + "base" + other_base_letter);
	both_bases = false;
	latest_revision_number = revision_number;
    }

#ifdef HAVE_PWRITE
    off_t offset = off_t(block_size) * n;
    int m = block_size;
    while (true) {
	ssize_t bytes_written = pwrite(handle, p, m, offset);
	if (bytes_written == m) {
	    // normal case - write succeeded, so return.
	    return;
	} else if (bytes_written == -1) {
	    if (errno == EINTR) continue;
	    string message = "Error writing block: ";
	    message += strerror(errno);
	    throw Xapian::DatabaseError(message);
	} else if (bytes_written == 0) {
	    string message = "Error writing block: wrote no data";
	    throw Xapian::DatabaseError(message);
	} else if (bytes_written < m) {
	    /* Wrote part of the block, which is not an error.  We should
	     * continue writing the rest of the block.
	     */
	    m -= bytes_written;
	    p += bytes_written;
	    offset += bytes_written;
	}
    }
#else
    if (lseek(handle, (off_t)block_size * n, SEEK_SET) == -1) {
	string message = "Error seeking to block: ";
	message += strerror(errno);
	throw Xapian::DatabaseError(message);
    }

    sys_write_bytes(handle, block_size, (const char *)p);
#endif
}


/* A note on cursors:

   Each B-tree level has a corresponding array element C[j] in a
   cursor, C. C[0] is the leaf (or data) level, and C[B->level] is the
   root block level. Within a level j,

       C[j].p  addresses the block
       C[j].c  is the offset into the directory entry in the block
       C[j].n  is the number of the block at C[j].p

   A look up in the B-tree causes navigation of the blocks starting
   from the root. In each block, p, we find an offset, c, to an item
   which gives the number, n, of the block for the next level. This
   leads to an array of values p,c,n which are held inside the cursor.

   Any Btree object B has a built-in cursor, at B->C. But other cursors may
   be created.  If BC is a created cursor, BC->C is the cursor in the
   sense given above, and BC->B is the handle for the B-tree again.
*/


void
Btree::set_overwritten() const
{
    DEBUGCALL(DB, void, "Btree::set_overwritten", "");
    // If we're writable, there shouldn't be another writer who could cause
    // overwritten to be flagged, so that's a DatabaseCorruptError.
    if (writable)
	throw Xapian::DatabaseCorruptError("Db block overwritten - are there multiple writers?");
    throw Xapian::DatabaseModifiedError("The revision being read has been discarded - you should call Xapian::Database::reopen() and retry the operation");
}

/* block_to_cursor(C, j, n) puts block n into position C[j] of cursor
   C, writing the block currently at C[j] back to disk if necessary.
   Note that

       C[j].rewrite

   is true iff C[j].n is different from block n in file DB. If it is
   false no rewriting is necessary.
*/

void
Btree::block_to_cursor(Cursor * C_, int j, uint4 n) const
{
    DEBUGCALL(DB, void, "Btree::block_to_cursor", (void*)C_ << ", " << j << ", " << n);
    if (n == C_[j].n) return;
    byte * p = C_[j].p;
    Assert(p);

    // FIXME: only needs to be done in write mode
    if (C_[j].rewrite) {
	Assert(writable);
	Assert(C == C_);
	write_block(C_[j].n, p);
	C_[j].rewrite = false;
    }
    // Check if the block is in the built-in cursor (potentially in
    // modified form).
    if (writable && n == C[j].n) {
	memcpy(p, C[j].p, block_size);
    } else {
	read_block(n, p);
    }

    C_[j].n = n;
    if (j < level) {
	/* unsigned comparison */
	if (REVISION(p) > REVISION(C_[j + 1].p)) {
	    set_overwritten();
	    return;
	}
    }
    AssertEq(j, GET_LEVEL(p));
}

/** Btree::alter(); is called when the B-tree is to be altered.

   It causes new blocks to be forced for the current set of blocks in
   the cursor.

   The point is that if a block at level 0 is to be altered it may get
   a new number. Then the pointer to this block from level 1 will need
   changing. So the block at level 1 needs altering and may get a new
   block number. Then the pointer to this block from level 2 will need
   changing ... and so on back to the root.

   The clever bit here is spotting the cases when we can make an early
   exit from this process. If C[j].rewrite is true, C[j+k].rewrite
   will be true for k = 1,2 ... We have been through all this before,
   and there is no need to do it again. If C[j].n was free at the
   start of the transaction, we can copy it back to the same place
   without violating the integrity of the B-tree. We don't then need a
   new n and can return. The corresponding C[j].rewrite may be true or
   false in that case.
*/

void
Btree::alter()
{
    DEBUGCALL(DB, void, "Btree::alter", "");
    Assert(writable);
#ifdef DANGEROUS
    C[0].rewrite = true;
#else
    int j = 0;
    byte * p = C[j].p;
    while (true) {
	if (C[j].rewrite) return; /* all new, so return */
	C[j].rewrite = true;

	uint4 n = C[j].n;
	if (base.block_free_at_start(n)) {
	    Assert(REVISION(p) == latest_revision_number + 1);
	    return;
	}
	Assert(REVISION(p) < latest_revision_number + 1);
	base.free_block(n);
	n = base.next_free_block();
	C[j].n = n;
	SET_REVISION(p, latest_revision_number + 1);

	if (j == level) return;
	j++;
	p = C[j].p;
	Item_wr(p, C[j].c).set_block_given_by(n);
    }
#endif
}

/** find_in_block(p, key, leaf, c) searches for the key in the block at p.

   leaf is true for a data block, and false for an index block (when the
   first key is dummy and never needs to be tested). What we get is the
   directory entry to the last key <= the key being searched for.

   The lookup is by binary chop, with i and j set to the left and
   right ends of the search area. In sequential addition, c will often
   be the answer, so we test the keys round c and move i and j towards
   c if possible.
*/

int Btree::find_in_block(const byte * p, Key key, bool leaf, int c)
{
    DEBUGCALL_STATIC(DB, int, "Btree::find_in_block", reinterpret_cast<const void*>(p) << ", " << reinterpret_cast<const void*>(key.get_address()) << ", " << leaf << ", " << c);
    int i = DIR_START;
    if (leaf) i -= D2;
    int j = DIR_END(p);

    if (c != -1) {
	if (c < j && i < c && Item(p, c).key() <= key)
	    i = c;
	c += D2;
	if (c < j && i < c && key < Item(p, c).key())
	    j = c;
    }

    while (j - i > D2) {
	int k = i + ((j - i)/(D2 * 2))*D2; /* mid way */
	if (key < Item(p, k).key()) j = k; else i = k;
    }
    RETURN(i);
}

/** find(C_) searches for the key of B->kt in the B-tree.

   Result is true if found, false otherwise.  When false, the B_tree
   cursor is positioned at the last key in the B-tree <= the search
   key.  Goes to first (null) item in B-tree when key length == 0.
*/

bool
Btree::find(Cursor * C_) const
{
    DEBUGCALL(DB, bool, "Btree::find", (void*)C_);
    // Note: the parameter is needed when we're called by BCursor
    const byte * p;
    int c;
    Key key = kt.key();
    for (int j = level; j > 0; --j) {
	p = C_[j].p;
	c = find_in_block(p, key, false, C_[j].c);
#ifdef BTREE_DEBUG_FULL
	printf("Block in Btree:find - code position 1");
	report_block_full(j, C_[j].n, p);
#endif /* BTREE_DEBUG_FULL */
	C_[j].c = c;
	block_to_cursor(C_, j - 1, Item(p, c).block_given_by());
    }
    p = C_[0].p;
    c = find_in_block(p, key, true, C_[0].c);
#ifdef BTREE_DEBUG_FULL
    printf("Block in Btree:find - code position 2");
    report_block_full(0, C_[0].n, p);
#endif /* BTREE_DEBUG_FULL */
    C_[0].c = c;
    if (c < DIR_START) RETURN(false);
    RETURN(Item(p, c).key() == key);
}

/** compact(p) compact the block at p by shuffling all the items up to the end.

   MAX_FREE(p) is then maximized, and is equal to TOTAL_FREE(p).
*/

void
Btree::compact(byte * p)
{
    DEBUGCALL(DB, void, "Btree::compact", (void*)p);
    Assert(writable);
    Assert(p);
    Assert(buffer);
    int e = block_size;
    byte * b = buffer;
    int dir_end = DIR_END(p);
    for (int c = DIR_START; c < dir_end; c += D2) {
	Item item(p, c);
	int l = item.size();
	Assert(e >= dir_end);
	e -= l;
	memmove(b + e, item.get_address(), l);
	SETD(p, c, e);  /* reform in b */
    }
    memmove(p + e, b + e, block_size - e);  /* copy back */
    e -= dir_end;
    SET_TOTAL_FREE(p, e);
    SET_MAX_FREE(p, e);
}

/** Btree needs to gain a new level to insert more items: so split root block
 *  and construct a new one.
 */
void
Btree::split_root(uint4 split_n)
{
    DEBUGCALL(DB, void, "Btree::split_root", split_n);
    /* gain a level */
    ++level;

    /* check level overflow - this isn't something that should ever happen
     * but deserves more than an Assert()... */
    if (level == BTREE_CURSOR_LEVELS) {
	throw Xapian::DatabaseCorruptError("Btree has grown impossibly large ("STRINGIZE(BTREE_CURSOR_LEVELS)" levels)");
    }

    byte * q = zeroed_new(block_size);
    if (q == 0) {
	throw std::bad_alloc();
    }
    C[level].p = q;
    C[level].c = DIR_START;
    C[level].n = base.next_free_block();
    C[level].rewrite = true;
    SET_REVISION(q, latest_revision_number + 1);
    SET_LEVEL(q, level);
    SET_DIR_END(q, DIR_START);
    compact(q);   /* to reset TOTAL_FREE, MAX_FREE */

    /* form a null key in b with a pointer to the old root */
    byte b[10]; /* 7 is exact */
    Item_wr item(b);
    item.form_null_key(split_n);
    add_item(item, level);
}

/** enter_key(j, prevkey, newkey) is called after a block split.

   It enters in the block at level C[j] a separating key for the block
   at level C[j - 1]. The key itself is newkey. prevkey is the
   preceding key, and at level 1 newkey can be trimmed down to the
   first point of difference to prevkey for entry in C[j].

   This code looks longer than it really is. If j exceeds the number
   of B-tree levels the root block has split and we have to construct
   a new one, but this is a rare event.

   The key is constructed in b, with block number C[j - 1].n as tag,
   and this is added in with add_item. add_item may itself cause a
   block split, with a further call to enter_key. Hence the recursion.
*/
void
Btree::enter_key(int j, Key prevkey, Key newkey)
{
    Assert(writable);
    Assert(prevkey < newkey);
    Assert(j >= 1);

    uint4 blocknumber = C[j - 1].n;

    // FIXME update to use Key
    // Keys are truncated here: but don't truncate the count at the end away.
    const int newkey_len = newkey.length();
    int i;

    if (j == 1) {
	// Truncate the key to the minimal key which differs from prevkey,
	// the preceding key in the block.
	i = 0;
	const int min_len = min(newkey_len, prevkey.length());
	while (i < min_len && prevkey[i] == newkey[i]) {
	    i++;
	}

	// Want one byte of difference.
	if (i < newkey_len) i++;
    } else {
	/* Can't truncate between branch levels, since the separated keys
	 * are in at the leaf level, and truncating again will change the
	 * branch point.
	 */
	i = newkey_len;
    }

    byte b[UCHAR_MAX + 6];
    Item_wr item(b);
    Assert(i <= 256 - I2 - C2);
    Assert(i <= (int)sizeof(b) - I2 - C2 - 4);
    item.set_key_and_block(newkey, i, blocknumber);

    // When j > 1 we can make the first key of block p null.  This is probably
    // worthwhile as it trades a small amount of CPU and RAM use for a small
    // saving in disk use.  Other redundant keys will still creep in though.
    if (j > 1) {
	byte * p = C[j - 1].p;
	uint4 n = get_int4(newkey.get_address(), newkey_len + K1 + C2);
	int new_total_free = TOTAL_FREE(p) + newkey_len + C2;
	// FIXME: incredibly icky going from key to item like this...
	Item_wr(const_cast<byte *>(newkey.get_address()) - I2).form_null_key(n);
	SET_TOTAL_FREE(p, new_total_free);
    }

    C[j].c = find_in_block(C[j].p, item.key(), false, 0) + D2;
    C[j].rewrite = true; /* a subtle point: this *is* required. */
    add_item(item, j);
}

/** mid_point(p) finds the directory entry in c that determines the
   approximate mid point of the data in the block at p.
 */

int
Btree::mid_point(byte * p)
{
    int n = 0;
    int dir_end = DIR_END(p);
    int size = block_size - TOTAL_FREE(p) - dir_end;
    for (int c = DIR_START; c < dir_end; c += D2) {
	int l = Item(p, c).size();
	n += 2 * l;
	if (n >= size) {
	    if (l < n - size) return c;
	    return c + D2;
	}
    }

    /* falling out of mid_point */
    Assert(false);
    return 0; /* Stop compiler complaining about end of method. */
}

/** add_item_to_block(p, kt_, c) adds item kt_ to the block at p.

   c is the offset in the directory that needs to be expanded to
   accommodate the new entry for the item. We know before this is
   called that there is enough room, so it's just a matter of byte
   shuffling.
*/

void
Btree::add_item_to_block(byte * p, Item_wr kt_, int c)
{
    Assert(writable);
    int dir_end = DIR_END(p);
    int kt_len = kt_.size();
    int needed = kt_len + D2;
    int new_total = TOTAL_FREE(p) - needed;
    int new_max = MAX_FREE(p) - needed;

    Assert(new_total >= 0);

    if (new_max < 0) {
	compact(p);
	new_max = MAX_FREE(p) - needed;
	Assert(new_max >= 0);
    }
    Assert(dir_end >= c);

    memmove(p + c + D2, p + c, dir_end - c);
    dir_end += D2;
    SET_DIR_END(p, dir_end);

    int o = dir_end + new_max;
    SETD(p, c, o);
    memmove(p + o, kt_.get_address(), kt_len);

    SET_MAX_FREE(p, new_max);

    SET_TOTAL_FREE(p, new_total);
}

/** Btree::add_item(kt_, j) adds item kt_ to the block at cursor level C[j].
 *
 *  If there is not enough room the block splits and the item is then
 *  added to the appropriate half.
 */
void
Btree::add_item(Item_wr kt_, int j)
{
    Assert(writable);
    byte * p = C[j].p;
    int c = C[j].c;
    uint4 n;

    int needed = kt_.size() + D2;
    if (TOTAL_FREE(p) < needed) {
	int m;
	// Prepare to split p. After splitting, the block is in two halves, the
	// lower half is split_p, the upper half p again. add_to_upper_half
	// becomes true when the item gets added to p, false when it gets added
	// to split_p.

	if (seq_count < 0) {
	    // If we're not in sequential mode, we split at the mid point
	    // of the node.
	    m = mid_point(p);
	} else {
	    // During sequential addition, split at the insert point
	    m = c;
	}

	uint4 split_n = C[j].n;
	C[j].n = base.next_free_block();

	memcpy(split_p, p, block_size);  // replicate the whole block in split_p
	SET_DIR_END(split_p, m);
	compact(split_p);      /* to reset TOTAL_FREE, MAX_FREE */

	{
	    int residue = DIR_END(p) - m;
	    int new_dir_end = DIR_START + residue;
	    memmove(p + DIR_START, p + m, residue);
	    SET_DIR_END(p, new_dir_end);
	}

	compact(p);      /* to reset TOTAL_FREE, MAX_FREE */

	bool add_to_upper_half;
	if (seq_count < 0) {
	    add_to_upper_half = (c >= m);
	} else {
	    // And add item to lower half if split_p has room, otherwise upper
	    // half
	    add_to_upper_half = (TOTAL_FREE(split_p) < needed);
	}

	if (add_to_upper_half) {
	    c -= (m - DIR_START);
	    Assert(seq_count < 0 || c <= DIR_START + D2);
	    Assert(c >= DIR_START);
	    Assert(c <= DIR_END(p));
	    add_item_to_block(p, kt_, c);
	    n = C[j].n;
	} else {
	    Assert(c >= DIR_START);
	    Assert(c <= DIR_END(split_p));
	    add_item_to_block(split_p, kt_, c);
	    n = split_n;
	}
	write_block(split_n, split_p);

	// Check if we're splitting the root block.
	if (j == level) split_root(split_n);

	/* Enter a separating key at level j + 1 between */
	/* the last key of block split_p, and the first key of block p */
	enter_key(j + 1,
		  Item(split_p, DIR_END(split_p) - D2).key(),
		  Item(p, DIR_START).key());
    } else {
	add_item_to_block(p, kt_, c);
	n = C[j].n;
    }
    if (j == 0) {
	changed_n = n;
	changed_c = c;
    }
}

/** Btree::delete_item(j, repeatedly) is (almost) the converse of add_item.
 *
 * If repeatedly is true, the process repeats at the next level when a
 * block has been completely emptied, freeing the block and taking out
 * the pointer to it.  Emptied root blocks are also removed, which
 * reduces the number of levels in the B-tree.
 */
void
Btree::delete_item(int j, bool repeatedly)
{
    Assert(writable);
    byte * p = C[j].p;
    int c = C[j].c;
    int kt_len = Item(p, c).size(); /* size of the item to be deleted */
    int dir_end = DIR_END(p) - D2;   /* directory length will go down by 2 bytes */

    memmove(p + c, p + c + D2, dir_end - c);
    SET_DIR_END(p, dir_end);
    SET_MAX_FREE(p, MAX_FREE(p) + D2);
    SET_TOTAL_FREE(p, TOTAL_FREE(p) + kt_len + D2);

    if (!repeatedly) return;
    if (j < level) {
	if (dir_end == DIR_START) {
	    base.free_block(C[j].n);
	    C[j].rewrite = false;
	    C[j].n = BLK_UNUSED;
	    C[j + 1].rewrite = true;  /* *is* necessary */
	    delete_item(j + 1, true);
	}
    } else {
	Assert(j == level);
	while (dir_end == DIR_START + D2 && level > 0) {
	    /* single item in the root block, so lose a level */
	    uint4 new_root = Item(p, DIR_START).block_given_by();
	    delete [] p;
	    C[level].p = 0;
	    base.free_block(C[level].n);
	    C[level].rewrite = false;
	    C[level].n = BLK_UNUSED;
	    level--;

	    block_to_cursor(C, level, new_root);

	    p = C[level].p;
	    dir_end = DIR_END(p); /* prepare for the loop */
	}
    }
}

/* debugging aid:
static addcount = 0;
*/

/** add_kt(found) adds the item (key-tag pair) at B->kt into the
   B-tree, using cursor C.

   found == find() is handed over as a parameter from Btree::add.
   Btree::alter() prepares for the alteration to the B-tree. Then
   there are a number of cases to consider:

     If an item with the same key is in the B-tree (found is true),
     the new kt replaces it.

     If then kt is smaller, or the same size as, the item it replaces,
     kt is put in the same place as the item it replaces, and the
     TOTAL_FREE measure is reduced.

     If kt is larger than the item it replaces it is put in the
     MAX_FREE space if there is room, and the directory entry and
     space counts are adjusted accordingly.

     - But if there is not room we do it the long way: the old item is
     deleted with delete_item and kt is added in with add_item.

     If the key of kt is not in the B-tree (found is false), the new
     kt is added in with add_item.
*/

int
Btree::add_kt(bool found)
{
    Assert(writable);
    int components = 0;

    /*
    {
	printf("%d) %s ", addcount++, (found ? "replacing" : "adding"));
	print_bytes(kt[I2] - K1 - C2, kt + I2 + K1); putchar('\n');
    }
    */
    alter();

    if (found) { /* replacement */
	seq_count = SEQ_START_POINT;
	sequential = false;

	byte * p = C[0].p;
	int c = C[0].c;
	Item item(p, c);
	int kt_size = kt.size();
	int needed = kt_size - item.size();

	components = Item(p, c).components_of();

	if (needed <= 0) {
	    /* simple replacement */
	    memmove(const_cast<byte *>(item.get_address()),
		    kt.get_address(), kt_size);
	    SET_TOTAL_FREE(p, TOTAL_FREE(p) - needed);
	} else {
	    /* new item into the block's freespace */
	    int new_max = MAX_FREE(p) - kt_size;
	    if (new_max >= 0) {
		int o = DIR_END(p) + new_max;
		memmove(p + o, kt.get_address(), kt_size);
		SETD(p, c, o);
		SET_MAX_FREE(p, new_max);
		SET_TOTAL_FREE(p, TOTAL_FREE(p) - needed);
	    } else {
		/* do it the long way */
		delete_item(0, false);
		add_item(kt, 0);
	    }
	}
    } else {
	/* addition */
	if (changed_n == C[0].n && changed_c == C[0].c) {
	    if (seq_count < 0) seq_count++;
	} else {
	    seq_count = SEQ_START_POINT;
	    sequential = false;
	}
	C[0].c += D2;
	add_item(kt, 0);
    }
    return components;
}

/* delete_kt() corresponds to add_kt(found), but there are only
   two cases: if the key is not found nothing is done, and if it is
   found the corresponding item is deleted with delete_item.
*/

int
Btree::delete_kt()
{
    Assert(writable);

    bool found = find(C);

    int components = 0;
    seq_count = SEQ_START_POINT;
    sequential = false;

    /*
    {
	printf("%d) %s ", addcount++, (found ? "deleting " : "ignoring "));
	print_bytes(B->kt[I2] - K1 - C2, B->kt + I2 + K1); putchar('\n');
    }
    */
    if (found) {
	components = Item(C[0].p, C[0].c).components_of();
	alter();
	delete_item(0, true);
    }
    return components;
}

/* Btree::form_key(key) treats address kt as an item holder and fills in
the key part:

	   (I) K key c (C tag)

The bracketed parts are left blank. The key is filled in with key_len bytes and
K set accordingly. c is set to 1.
*/

void Btree::form_key(const string & key) const
{
    kt.form_key(key);
}

/* Btree::add(key, tag) adds the key/tag item to the
   B-tree, replacing any existing item with the same key.

   For a long tag, we end up having to add m components, of the form

       key 1 m tag1
       key 2 m tag2
       ...
       key m m tagm

   and tag1+tag2+...+tagm are equal to tag. These in their turn may be replacing
   n components of the form

       key 1 n TAG1
       key 2 n TAG2
       ...
       key n n TAGn

   and n may be greater than, equal to, or less than m. These cases are dealt
   with in the code below. If m < n for example, we end up with a series of
   deletions.
*/

bool
Btree::add(const string &key, string tag)
{
    DEBUGCALL(DB, bool, "Btree::add", key << ", " << tag);
    Assert(writable);

    if (key.size() > BTREE_MAX_KEY_LEN) {
	throw Xapian::InvalidArgumentError(
		"Key too long: length was " +
		om_tostring(key.size()) +
		" bytes, maximum length of a key is " + 
		STRINGIZE(BTREE_MAX_KEY_LEN) + " bytes");
    }

    form_key(key);

    // sort of matching kt.append_chunk(), but setting the chunk
    const size_t cd = kt.key().length() + K1 + I2 + C2 + C2;  // offset to the tag data
    const size_t L = max_item_size - cd; // largest amount of tag data for any chunk
    size_t first_L = L;                  // - amount for tag1
    bool found = find(C);
    if (!found) {
	byte * p = C[0].p;
	size_t n = TOTAL_FREE(p) % (max_item_size + D2);
	if (n > D2 + cd) {
	    n -= (D2 + cd);
	    // if n >= last then fully filling this block won't produce
	    // an extra item, so we might as well do this even if
	    // full_compaction isn't active.
	    //
	    // In the full_compaction case, it turns out we shouldn't always
	    // try to fill every last byte.  Doing so can actually increase the
	    // total space required (I believe this effect is due to longer
	    // dividing keys being required in the index blocks).  Empirically,
	    // n >= key.size() + K appears a good criterion for K ~= 34.  This
	    // seems to save about 0.2% in total database size over always
	    // splitting the tag.  It'll also give be slightly faster retrieval
	    // as we can avoid reading an extra block occasionally.
	    size_t last = tag.length() % L;
	    if (n >= last || (full_compaction && n >= key.size() + 34))
		first_L = n;
	}
    }

    // a null tag must be added in of course
    int m = tag.empty() ? 1 : (tag.length() - first_L + L - 1) / L + 1;
				      // there are m items to add
    /* FIXME: sort out this error higher up and turn this into
     * an assert.
     */
    if (m >= BYTE_PAIR_RANGE) RETURN(false);

    int n = 0; // initialise to shut off warning
				      // - and there will be n to delete
    int o = 0;                        // Offset into the tag
    size_t residue = tag.length();    // Bytes of the tag remaining to add in
    int replacement = false;          // Has there been a replacement ?
    int i;
    kt.set_components_of(m);
    for (i = 1; i <= m; i++) {
	size_t l = (i == m ? residue : (i == 1 ? first_L : L));
	Assert(cd + l <= block_size);
	Assert(string::size_type(o + l) <= tag.length());
	kt.set_tag(cd, tag.data() + o, l);
	kt.set_component_of(i);
	
	o += l;
	residue -= l;

	if (i > 1) found = find(C);
	n = add_kt(found);
	if (n > 0) replacement = true;
    }
    /* o == tag.length() here, and n may be zero */
    for (i = m + 1; i <= n; i++) {
	kt.set_component_of(i);
	delete_kt();
    }
    if (!replacement) ++item_count;
    Btree_modified = true;
    RETURN(true);
}

/* Btree::del(key) returns false if the key is not in the B-tree,
   otherwise deletes it and returns true.

   Again, this is parallel to Btree::add, but simpler in form.
*/

bool
Btree::del(const string &key)
{
    DEBUGCALL(DB, bool, "Btree::del", key);
    Assert(writable);

    // We can't delete a key which we is too long for us to store.
    if (key.size() > BTREE_MAX_KEY_LEN) RETURN(false);

    if (key.empty()) RETURN(false);
    form_key(key);

    int n = delete_kt();  /* there are n items to delete */
    if (n <= 0) RETURN(false);

    for (int i = 2; i <= n; i++) {
	kt.set_component_of(i);
	delete_kt();
    }

    item_count--;
    Btree_modified = true;
    RETURN(true);
}

bool
Btree::get_exact_entry(const string &key, string & tag) const
{
    DEBUGCALL(DB, bool, "Btree::get_exact_entry", key << ", " << tag);
    Assert(!key.empty());

    // An oversized key can't exist, so attempting to search for it should fail.
    if (key.size() > BTREE_MAX_KEY_LEN) RETURN(false);

    RETURN(find_tag(key, &tag));
}

bool
Btree::find_tag(const string &key, string * tag) const
{
    DEBUGCALL(DB, bool, "Btree::find_tag", key << ", &tag");
    form_key(key);
    if (!find(C)) RETURN(false);

    read_tag(C, tag);
    RETURN(true);
}

void
Btree::read_tag(Cursor * C_, string *tag) const
{
    Item item(C_[0].p, C_[0].c);

    /* n components to join */
    int n = item.components_of();

    tag->resize(0);
    // max_item_size also includes K1 + I2 + C2 + C2 bytes overhead and the key
    // (which is at least 1 byte long).
    if (n > 1) tag->reserve((max_item_size - (1 + K1 + I2 + C2 + C2)) * n);

    item.append_chunk(tag);

    for (int i = 2; i <= n; i++) {
	if (!next(C_, 0)) {
	    throw Xapian::DatabaseCorruptError("Unexpected end of table when reading continuation of tag");
	}
	(void)Item(C_[0].p, C_[0].c).append_chunk(tag);
    }
    // At this point the cursor is on the last item - calling next will move
    // it to the next key (Bcursor::get_tag() relies on this).
}

void
Btree::set_full_compaction(bool parity)
{
    Assert(writable);

    if (parity) seq_count = 0;
    full_compaction = parity;
}

Bcursor * Btree::cursor_get() const {
    // FIXME Ick - casting away const is nasty
    return new Bcursor(const_cast<Btree *>(this));
}

/************ B-tree opening and closing ************/

bool
Btree::basic_open(bool revision_supplied, quartz_revision_number_t revision_)
{
    int ch = 'X'; /* will be 'A' or 'B' */

    {
	const size_t BTREE_BASES = 2;
	string err_msg;
	static const char basenames[BTREE_BASES] = { 'A', 'B' };

	Btree_base bases[BTREE_BASES];
	bool base_ok[BTREE_BASES];

	both_bases = true;
	bool valid_base = false;
	{
	    for (size_t i = 0; i < BTREE_BASES; ++i) {
		bool ok = bases[i].read(name, basenames[i], err_msg);
		base_ok[i] = ok;
		if (ok) {
		    valid_base = true;
		} else {
		    both_bases = false;
		}
	    }
	}

	if (!valid_base) {
	    string message = "Error opening table `";
	    message += name;
	    message += "':\n";
	    message += err_msg;
	    throw Xapian::DatabaseOpeningError(message);
	}

	if (revision_supplied) {
	    bool found_revision = false;
	    for (size_t i = 0; i < BTREE_BASES; ++i) {
		if (base_ok[i] && bases[i].get_revision() == revision_) {
		    ch = basenames[i];
		    found_revision = true;
		    break;
		}
	    }
	    if (!found_revision) {
		/* Couldn't open the revision that was asked for.
		 * This shouldn't throw an exception, but should just return
		 * false to upper levels.
		 */
		return false;
	    }
	} else {
	    quartz_revision_number_t highest_revision = 0;
	    for (size_t i = 0; i < BTREE_BASES; ++i) {
		if (base_ok[i] && bases[i].get_revision() >= highest_revision) {
		    ch = basenames[i];
		    highest_revision = bases[i].get_revision();
		}
	    }
	}

	Btree_base *basep = 0;
	Btree_base *other_base = 0;

	for (size_t i = 0; i < BTREE_BASES; ++i) {
	    DEBUGLINE(UNKNOWN, "Checking (ch == " << ch << ") against "
		      "basenames[" << i << "] == " << basenames[i]);
	    DEBUGLINE(UNKNOWN, "bases[" << i << "].get_revision() == " <<
		      bases[i].get_revision());
	    DEBUGLINE(UNKNOWN, "base_ok[" << i << "] == " << base_ok[i]);
	    if (ch == basenames[i]) {
		basep = &bases[i];

		// FIXME: assuming only two bases for other_base
		size_t otherbase_num = 1-i;
		if (base_ok[otherbase_num]) {
		    other_base = &bases[otherbase_num];
		}
		break;
	    }
	}
	Assert(basep);

	/* basep now points to the most recent base block */

	/* Avoid copying the bitmap etc. - swap contents with the base
	 * object in the vector, since it'll be destroyed anyway soon.
	 */
	base.swap(*basep);

	revision_number =  base.get_revision();
	block_size =       base.get_block_size();
	root =             base.get_root();
	level =            base.get_level();
	//bit_map_size =     basep->get_bit_map_size();
	item_count =       base.get_item_count();
	faked_root_block = base.get_have_fakeroot();
	sequential =       base.get_sequential();

	if (other_base != 0) {
	    latest_revision_number = other_base->get_revision();
	    if (revision_number > latest_revision_number)
		latest_revision_number = revision_number;
	} else {
	    latest_revision_number = revision_number;
	}
    }

    /* kt holds constructed items as well as keys */
    kt = Item_wr(zeroed_new(block_size));
    if (kt.get_address() == 0) {
	throw std::bad_alloc();
    }

    set_max_item_size(BLOCK_CAPACITY);

    base_letter = ch;

    /* ready to open the main file */

    return true;
}

void
Btree::read_root()
{
    if (faked_root_block) {
	/* root block for an unmodified database. */
	byte * p = C[0].p;
	Assert(p);

	/* clear block - shouldn't be neccessary, but is a bit nicer,
	 * and means that the same operations should always produce
	 * the same database. */
	memset(p, 0, block_size);

	int o = block_size - I2 - K1 - C2 - C2;
	Item_wr(p + o).fake_root_item();

	SETD(p, DIR_START, o);         // its directory entry
	SET_DIR_END(p, DIR_START + D2);// the directory size

	o -= (DIR_START + D2);
	SET_MAX_FREE(p, o);
	SET_TOTAL_FREE(p, o);
	SET_LEVEL(p, 0);

	if (!writable) {
	    /* reading - revision number doesn't matter as long as
	     * it's not greater than the current one. */
	    SET_REVISION(p, 0);
	    C[0].n = 0;
	} else {
	    /* writing - */
	    SET_REVISION(p, latest_revision_number + 1);
	    C[0].n = base.next_free_block();
	}
    } else {
	/* using a root block stored on disk */
	block_to_cursor(C, level, root);

	if (REVISION(C[level].p) > revision_number) set_overwritten();
	/* although this is unlikely */
    }
}

bool
Btree::do_open_to_write(bool revision_supplied, quartz_revision_number_t revision_)
{
    /* FIXME: do the exception safety the right way, by making all the
     * parts into sensible objects.
     */
    if (!basic_open(revision_supplied, revision_)) {
	if (!revision_supplied) {
	    throw Xapian::DatabaseOpeningError("Failed to open for writing");
	}
	/* When the revision is supplied, it's not an exceptional
	 * case when open failed, so we just return false here.
	 */
	return false;
    }

    writable = true;

    handle = sys_open_for_readwrite(name + "DB");

    prev_ptr = &Btree::prev_default;
    next_ptr = &Btree::next_default;

    for (int j = 0; j <= level; j++) {
	C[j].n = BLK_UNUSED;
	C[j].p = new byte[block_size];
	if (C[j].p == 0) {
	    throw std::bad_alloc();
	}
    }
    split_p = new byte[block_size];
    if (split_p == 0) {
	throw std::bad_alloc();
    }
    read_root();

    buffer = zeroed_new(block_size);
    if (buffer == 0) {
	throw std::bad_alloc();
    }

    // swap for writing
    other_base_letter = base_letter == 'A' ? 'B' : 'A';

    changed_n = 0;
    changed_c = DIR_START;
    seq_count = SEQ_START_POINT;

    return true;
}

Btree::Btree(string path_, bool readonly_)
	: revision_number(0),
	  item_count(0),
	  block_size(0),
	  latest_revision_number(0),
	  both_bases(false),
	  base_letter('A'),
	  faked_root_block(true),
	  sequential(true),
	  handle(-1),
	  level(0),
	  root(0),
	  kt(0),
	  buffer(0),
	  base(),
	  other_base_letter(0),
	  name(path_),
	  seq_count(0),
	  changed_n(0),
	  changed_c(0),
	  max_item_size(0),
	  Btree_modified(false),
	  full_compaction(false),
	  writable(!readonly_),
	  dont_close_handle(false),
	  split_p(0)
{
    DEBUGCALL(DB, void, "Btree::Btree", path_ << ", " << readonly_);
}

bool
Btree::exists() const {
    DEBUGCALL(DB, bool, "Btree::exists", "");
    return (file_exists(name + "DB") &&
	    (file_exists(name + "baseA") || file_exists(name + "baseB")));
}

/** Delete file, throwing an error if can't delete it (but not if it
 *  doesn't exist)
 */
void
sys_unlink_if_exists(const string & filename)
{
    if (unlink(filename) == -1) {
	if (errno == ENOENT) return;
	throw Xapian::DatabaseError("Can't delete file: `" + filename +
			      "': " + strerror(errno));
    }
}

void
Btree::create(unsigned int block_size_)
{
    DEBUGCALL(DB, void, "Btree::create", block_size_);
    close();

    // Block size must in the range 2048..BYTE_PAIR_RANGE, and a power of two.
    if (block_size_ < 2048 || block_size_ > BYTE_PAIR_RANGE ||
	(block_size_ & (block_size_ - 1)) != 0) {
	block_size_ = 8192;
    }

    // FIXME: it would be good to arrange that this works such that there's
    // always a valid table in place...

    /* write initial values to files */

    /* create the base file */
    Btree_base base_;
    base_.set_block_size(block_size_);
    base_.set_have_fakeroot(true);
    base_.set_sequential(true);
    base_.write_to_file(name + "baseA");

    /* remove the alternative base file, if any */
    sys_unlink_if_exists(name + "baseB");

    /* create the main file */
    int h = sys_open_to_write(name + "DB");
    if (h == -1 || !sys_close(h)) {
	string message = "Error creating DB file: ";
	message += strerror(errno);
	throw Xapian::DatabaseOpeningError(message);
    }
}

Btree::~Btree() {
    DEBUGCALL(DB, void, "Btree::~Btree", "");
    Btree::close();
}

void Btree::close() {
    DEBUGCALL(DB, void, "Btree::close", "");

    if (handle != -1) {
	// If an error occurs here, we just ignore it, since we're just
	// trying to free everything.
	if (!dont_close_handle) (void)sys_close(handle);
	handle = -1;
    }

    for (int j = level; j >= 0; j--) {
	delete [] C[j].p;
	C[j].p = 0;
    }
    delete [] split_p;
    split_p = 0;

    delete [] kt.get_address();
    kt = 0;
    delete [] buffer;
    buffer = 0;
}

void
Btree::commit(quartz_revision_number_t revision)
{
    DEBUGCALL(DB, void, "Btree::commit", revision);
    Assert(writable);

    if (revision <= revision_number) {
	throw Xapian::DatabaseError("New revision too low");
    }

    // FIXME: this doesn't work (probably because the table revisions get
    // out of step) but it's wasteful to keep applying changes to value
    // and position if they're never used...
    //
    // if (!Btree_modified) return;

    for (int j = level; j >= 0; j--) {
	if (C[j].rewrite) {
	    write_block(C[j].n, C[j].p);
	}
    }

    if (!sys_flush(handle)) {
	if (!dont_close_handle) (void)sys_close(handle);
	handle = -1;
	throw Xapian::DatabaseError("Can't commit new revision - failed to close DB");
    }

    if (Btree_modified) {
	faked_root_block = false;
    }

    if (faked_root_block) {
	/* We will use a dummy bitmap. */
	base.clear_bit_map();
    }

    base.set_revision(revision);
    base.set_root(C[level].n);
    base.set_level(level);
    base.set_item_count(item_count);
    base.set_have_fakeroot(faked_root_block);
    base.set_sequential(sequential);

    {
	int tmp = base_letter;
	base_letter = other_base_letter;
	other_base_letter = tmp;
    }
    both_bases = true;
    latest_revision_number = revision_number = revision;
    root = C[level].n;

    Btree_modified = false;

    for (int i = 0; i < BTREE_CURSOR_LEVELS; ++i) {
	C[i].n = BLK_UNUSED;
	C[i].c = -1;
	C[i].rewrite = false;
    }

    base.write_to_file(name + "base" + char(base_letter));
    // Save to "<table>.tmp" and then rename to "<table>.base<letter>" so that
    // a reader can't try to read a partially written base file.
    string tmp = name;
    tmp += "tmp";
    string basefile = name;
    basefile += "base";
    basefile += char(base_letter);
    base.write_to_file(tmp);
#if defined __WIN32__
    if (msvc_posix_rename(tmp.c_str(), basefile.c_str()) < 0) {
#else
    if (rename(tmp.c_str(), basefile.c_str()) < 0) {
#endif
	// With NFS, rename() failing may just mean that the server crashed
	// after successfully renaming, but before reporting this, and then
	// the retried operation fails.  So we need to check if the source
	// file still exists, which we do by calling unlink(), since we want
	// to remove the temporary file anyway.
	int saved_errno = errno;
	if (unlink(tmp) == 0 || errno != ENOENT) {
	    string msg("Couldn't update base file ");
	    msg += basefile;
	    msg += ": ";
	    msg += strerror(saved_errno);
	    throw Xapian::DatabaseError(msg);
	}
    }
    base.commit();

    read_root();

    changed_n = 0;
    changed_c = DIR_START;
    seq_count = SEQ_START_POINT;
}

void
Btree::cancel()
{
    DEBUGCALL(DB, void, "Btree::cancel", "");
    Assert(writable);

    // This causes problems: if (!Btree_modified) return;

    string err_msg;
    if (!base.read(name, base_letter, err_msg)) {
	throw Xapian::DatabaseCorruptError(string("Couldn't reread base ") + char(base_letter));
    }

    revision_number =  base.get_revision();
    block_size =       base.get_block_size();
    root =             base.get_root();
    level =            base.get_level();
    //bit_map_size =     basep->get_bit_map_size();
    item_count =       base.get_item_count();
    faked_root_block = base.get_have_fakeroot();
    sequential =       base.get_sequential();

    latest_revision_number = revision_number; // FIXME: we can end up reusing a revision if we opened a btree at an older revision, start to modify it, then cancel...

    prev_ptr = &Btree::prev_default;
    next_ptr = &Btree::next_default;

    for (int j = 0; j <= level; j++) {
	C[j].n = BLK_UNUSED;
	C[j].rewrite = false;
    }
    read_root();

    changed_n = 0;
    changed_c = DIR_START;
    seq_count = SEQ_START_POINT;
}

/************ B-tree reading ************/

bool
Btree::do_open_to_read(bool revision_supplied, quartz_revision_number_t revision_)
{
    if (!basic_open(revision_supplied, revision_)) {
	if (revision_supplied) {
	    // The requested revision was not available.
	    // This could be because the database was modified underneath us, or
	    // because a base file is missing.  Return false, and work out what
	    // the problem was at a higher level.
	    return false;
	}
	throw Xapian::DatabaseOpeningError("Failed to open table for reading");
    }

    handle = sys_open_to_read(name + "DB");

    if (sequential) {
	prev_ptr = &Btree::prev_for_sequential;
	next_ptr = &Btree::next_for_sequential;
    } else {
	prev_ptr = &Btree::prev_default;
	next_ptr = &Btree::next_default;
    }

    for (int j = 0; j <= level; j++) {
	C[j].n = BLK_UNUSED;
	C[j].p = new byte[block_size];
	if (C[j].p == 0) {
	    throw std::bad_alloc();
	}
    }

    read_root();
    return true;
}

void
Btree::open()
{
    DEBUGCALL(DB, void, "Btree::open", "");
    DEBUGLINE(DB, "opening at path " << name);
    close();

    if (!writable) {
	// Any errors are thrown if revision_supplied is false
	(void)do_open_to_read(false, 0);
	return;
    }

    // Any errors are thrown if revision_supplied is false
    (void)do_open_to_write(false, 0);
}

bool
Btree::open(quartz_revision_number_t revision)
{
    DEBUGCALL(DB, bool, "Btree::open", revision);
    DEBUGLINE(DB, "opening for particular revision at path " << name);
    close();

    if (!writable) {
	if (do_open_to_read(true, revision)) {
	    AssertEq(revision_number, revision);
	    RETURN(true);
	} else {
	    close();
	    RETURN(false);
	}
    }

    if (!do_open_to_write(true, revision)) {
	// Can't open at the requested revision.
	close();
	RETURN(false);
    }

    AssertEq(revision_number, revision);
    RETURN(true);
}

bool
Btree::prev_for_sequential(Cursor * C_, int /*dummy*/) const
{
    int c = C_[0].c;
    if (c == DIR_START) {
	byte * p = C_[0].p;
	Assert(p);
	uint4 n = C_[0].n;
	while (true) {
	    if (n == 0) return false;
	    n--;
	    if (writable) {
		if (n == C[0].n) {
		    // Block is a leaf block in the built-in cursor
		    // (potentially in modified form).
		    memcpy(p, C[0].p, block_size);
		} else {
		    // Blocks in the built-in cursor may not have been written
		    // to disk yet, so we have to check that the block number
		    // isn't in the built-in cursor or we'll read an
		    // uninitialised block (for which GET_LEVEL(p) will
		    // probably return 0).
		    int j;
		    for (j = 1; j <= level; ++j) {
			if (n == C[j].n) break;
		    }
		    if (j <= level) continue;

		    // Block isn't in the built-in cursor, so the form on disk
		    // is valid, so read it to check if it's the next level 0
		    // block.
		    read_block(n, p);
		}
	    } else {
		read_block(n, p);
	    }
	    if (REVISION(p) > 1) {
		set_overwritten();
		return false;
	    }
	    if (GET_LEVEL(p) == 0) break;
	}
	c = DIR_END(p);
	C_[0].n = n;
    }
    c -= D2;
    C_[0].c = c;
    return true;
}

bool
Btree::next_for_sequential(Cursor * C_, int /*dummy*/) const
{
    byte * p = C_[0].p;
    Assert(p);
    int c = C_[0].c;
    c += D2;
    if (c == DIR_END(p)) {
	uint4 n = C_[0].n;
	while (true) {
	    n++;
	    if (n > base.get_last_block()) return false;
	    if (writable) {
		if (n == C[0].n) {
		    // Block is a leaf block in the built-in cursor
		    // (potentially in modified form).
		    memcpy(p, C[0].p, block_size);
		} else {
		    // Blocks in the built-in cursor may not have been written
		    // to disk yet, so we have to check that the block number
		    // isn't in the built-in cursor or we'll read an
		    // uninitialised block (for which GET_LEVEL(p) will
		    // probably return 0).
		    int j;
		    for (j = 1; j <= level; ++j) {
			if (n == C[j].n) break;
		    }
		    if (j <= level) continue;

		    // Block isn't in the built-in cursor, so the form on disk
		    // is valid, so read it to check if it's the next level 0
		    // block.
		    read_block(n, p);
		}
	    } else {
		read_block(n, p);
	    }
	    if (REVISION(p) > 1) {
		set_overwritten();
		return false;
	    }
	    if (GET_LEVEL(p) == 0) break;
	}
	c = DIR_START;
	C_[0].n = n;
    }
    C_[0].c = c;
    return true;
}

bool
Btree::prev_default(Cursor * C_, int j) const
{
    byte * p = C_[j].p;
    int c = C_[j].c;
    Assert(c >= DIR_START);
    Assert((unsigned)c < block_size);
    Assert(c <= DIR_END(p));
    if (c == DIR_START) {
	if (j == level) return false;
	if (!prev_default(C_, j + 1)) return false;
	c = DIR_END(p);
    }
    c -= D2;
    C_[j].c = c;
    if (j > 0) {
	block_to_cursor(C_, j - 1, Item(p, c).block_given_by());
    }
    return true;
}

bool
Btree::next_default(Cursor * C_, int j) const
{
    byte * p = C_[j].p;
    int c = C_[j].c;
    Assert(c >= DIR_START);
    c += D2;
    Assert((unsigned)c < block_size);
    // Sometimes c can be DIR_END(p) + 2 here it appears...
    if (c > DIR_END(p)) c = DIR_END(p);
    Assert(c <= DIR_END(p));
    if (c == DIR_END(p)) {
	if (j == level) return false;
	if (!next_default(C_, j + 1)) return false;
	c = DIR_START;
    }
    C_[j].c = c;
    if (j > 0) {
	block_to_cursor(C_, j - 1, Item(p, c).block_given_by());
#ifdef BTREE_DEBUG_FULL
	printf("Block in Btree:next_default");
	report_block_full(j - 1, C_[j - 1].n, C_[j - 1].p);
#endif /* BTREE_DEBUG_FULL */
    }
    return true;
}

/** Compares this key with key2.

   The result is true if this key precedes key2. The comparison is for byte
   sequence collating order, taking lengths into account. So if the keys are
   made up of lower case ASCII letters we get alphabetical ordering.

   Now remember that items are added into the B-tree in fastest time
   when they are preordered by their keys. This is therefore the piece
   of code that needs to be followed to arrange for the preordering.

   This is complicated by the fact that keys have two parts - a value
   and then a count.  We first compare the values, and only if they
   are equal do we compare the counts.
*/

bool Key::operator<(Key key2) const
{
    DEBUGCALL(DB, bool, "Key::operator<", reinterpret_cast<const void*>(key2.p));
    int key1_len = length();
    int key2_len = key2.length();
    if (key1_len == key2_len) {
	// The keys are the same length, so we can compare the counts
	// in the same operation since they're stored as 2 byte
	// bigendian numbers.
	RETURN(memcmp(p + K1, key2.p + K1, key1_len + C2) < 0);
    }

    int k_smaller = (key2_len < key1_len ? key2_len : key1_len);

    // Compare the common part of the keys
    int diff = memcmp(p + K1, key2.p + K1, k_smaller);
    if (diff != 0) RETURN(diff < 0);

    // We dealt with the "same length" case above so we never need to check
    // the count here.
    RETURN(key1_len < key2_len);
}

bool Key::operator==(Key key2) const
{
    DEBUGCALL(DB, bool, "Key::operator==", reinterpret_cast<const void*>(key2.p));
    int key1_len = length();
    if (key1_len != key2.length()) return false;
    // The keys are the same length, so we can compare the counts
    // in the same operation since they're stored as 2 byte
    // bigendian numbers.
    RETURN(memcmp(p + K1, key2.p + K1, key1_len + C2) == 0);
}
