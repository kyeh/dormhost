/* dbfactory_remote.cc: Database factories for remote databases.
 *
 * Copyright (C) 2006,2007 Olly Betts
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <config.h>

#include <xapian/dbfactory.h>

#include "progclient.h"
#include "tcpclient.h"

#include <string>

using namespace std;

namespace Xapian {

Database
Remote::open(const string &host, unsigned int port, Xapian::timeout timeout,
	     Xapian::timeout connect_timeout)
{
    DEBUGAPICALL_STATIC(Database, "Remote::open",
	host << ", " << port << ", " << timeout << ", " << connect_timeout);
    return Database(new TcpClient(host, port, timeout, connect_timeout, false));
}

WritableDatabase
Remote::open_writable(const string &host, unsigned int port,
		      Xapian::timeout timeout, Xapian::timeout connect_timeout)
{
    DEBUGAPICALL_STATIC(WritableDatabase, "Remote::open_writable",
	host << ", " << port << ", " << timeout << ", " << connect_timeout);
    return WritableDatabase(new TcpClient(host, port, timeout, connect_timeout, true));
}

Database
Remote::open(const string &program, const string &args, Xapian::timeout timeout)
{
    DEBUGAPICALL_STATIC(Database, "Remote::open",
	program << ", " << args << ", " << timeout);
    return Database(new ProgClient(program, args, timeout, false));
}

WritableDatabase
Remote::open_writable(const string &program, const string &args,
		      Xapian::timeout timeout)
{
    DEBUGAPICALL_STATIC(WritableDatabase, "Remote::open_writable",
	program << ", " << args << ", " << timeout);
    return WritableDatabase(new ProgClient(program, args, timeout, true));
}

}
