/* quartz_log.cc: A logfile for quartz.
 *
 * Copyright 1999,2000,2001 BrightStation PLC
 * Copyright 2002 Ananova Ltd
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
#include "safefcntl.h"

#include "quartz_log.h"
#include "omdebug.h"
#include "utils.h"

#include <string.h>
#include <time.h>

#include <sys/types.h>

#include "safeunistd.h"

using std::string;

QuartzLog::QuartzLog(string filename)
{
    DEBUGCALL(DB, void, "QuartzLog", filename);
    fd = open(filename.c_str(), O_APPEND|O_WRONLY);
}

QuartzLog::~QuartzLog()
{
    DEBUGCALL(DB, void, "~QuartzLog", "");
    if (fd != -1) {
	// Must ignore errors - we can't throw an exception here.
	(void) close(fd);
    }
}

void
QuartzLog::make_entry(const string &entry) const
{
    DEBUGCALL(DB, void, "QuartzLog::make_entry", entry);
    if (fd != -1) {
	string line(om_tostring(getpid()));
	line += ':';
	line += om_tostring(time(NULL));
	line += ':';
	line += entry;
	line += '\n';
	const char *p = line.data();
	ssize_t c = line.size();
	while (c) {
	    ssize_t n = write(fd, p, c);
	    if (n == -1) {
		if (errno == EINTR) continue;
		// FIXME: Xapian::Database*Opening*Error ?!
		throw Xapian::DatabaseOpeningError("Error writing log file: " +
			string(strerror(errno)));
	    }
	    c -= n;
	    p += n;
	}
    }
}
