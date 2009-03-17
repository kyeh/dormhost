/** @file flint_io.h
 * @brief Wrappers for low-level POSIX I/O routines.
 */
/* Copyright (C) 2006,2007 Olly Betts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#ifndef XAPIAN_INCLUDED_FLINT_IO_H
#define XAPIAN_INCLUDED_FLINT_IO_H

#include <sys/types.h>
#include "safefcntl.h"
#include "safeunistd.h"

/** Ensure all data previously written to file descriptor fd has been written to
 *  disk.
 *
 *  Returns false if this could not be done.
 */
inline bool flint_io_sync(int fd)
{
    // If we have it, prefer fdatasync() as it avoids updating the access time
    // so is probably a little more efficient.
#if defined HAVE_FDATASYNC
    return fdatasync(fd) == 0;
#elif defined HAVE_FSYNC
    return fsync(fd) == 0;
#elif defined __WIN32__
    return _commit(fd) == 0;
#else
# error Cannot implement flint_io_sync() without fdatasync(), fsync(), or _commit()
#endif
}

/** Read n bytes (or until EOF) into block pointed to by p from file descriptor
 *  fd.
 *
 *  If less than min bytes are read, throw an exception.
 *
 *  Returns the number of bytes actually read.
 */
size_t flint_io_read(int fd, char * p, size_t n, size_t min);

/** Write n bytes from block pointed to by p to file descriptor fd. */
void flint_io_write(int fd, const char * p, size_t n);

#endif
