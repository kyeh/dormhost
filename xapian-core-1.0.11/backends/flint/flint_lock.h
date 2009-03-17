/* flint_lock.h: database locking for flint backend.
 *
 * Copyright (C) 2005,2006,2007 Olly Betts
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

#ifndef XAPIAN_INCLUDED_FLINT_LOCK_H
#define XAPIAN_INCLUDED_FLINT_LOCK_H

#include <string>

#if defined __CYGWIN__ || defined __WIN32__
# include "safewindows.h"
#elif defined __EMX__
# define INCL_DOS
# define INCL_DOSERRORS
# include <os2.h>
#else
# include <sys/types.h>
#endif

class FlintLock {
    std::string filename;
#if defined __CYGWIN__ || defined __WIN32__
    HANDLE hFile;
#elif defined __EMX__
    HFILE hFile;
#else
    int fd;
    pid_t pid;
#endif

  public:
    typedef enum {
	SUCCESS, // We got the lock!
	INUSE, // Already locked by someone else.
	UNSUPPORTED, // Locking probably not supported (e.g. NFS without lockd).
	UNKNOWN // The attempt failed for some unspecified reason.
    } reason;
#if defined __CYGWIN__ || defined __WIN32__
    FlintLock(const std::string &filename_)
	: filename(filename_), hFile(INVALID_HANDLE_VALUE) { }
    operator bool() { return hFile != INVALID_HANDLE_VALUE; }
#elif defined __EMX__
    FlintLock(const std::string &filename_)
	: filename(filename_), hFile(NULLHANDLE) { }
    operator bool() { return hFile != NULLHANDLE; }
#else
    FlintLock(const std::string &filename_) : filename(filename_), fd(-1) { }
    operator bool() { return fd != -1; }
#endif
    // Release any lock held when we're destroyed.
    ~FlintLock() { release(); }

    reason lock(bool exclusive);
    void release();
};

#endif // XAPIAN_INCLUDED_FLINT_LOCK_H
