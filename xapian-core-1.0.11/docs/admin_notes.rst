.. This document was originally written by Richard Boulton, with funding
.. provided by Enfold Systems.

.. Copyright (C) 2006 Lemur Consulting Ltd
.. Copyright (C) 2007 Olly Betts

============================
Xapian Administrator's Guide
============================

.. contents:: Table of contents

Introduction
============

This document is intended to provide general hints, tips and advice to
administrators of Xapian systems.  It assumes that you have installed Xapian
on your system, and are familiar with the basics of creating and searching
Xapian databases.

The intended audience is system administrators who need to be able to perform
general management of a Xapian database, including tasks such as taking
backups and optimising performance.  It may also be useful introductory
reading for Xapian application developers.

The document is up-to-date for Xapian version 1.0.5.

Databases
=========

Xapian databases hold all the information needed to perform searches in a set
of tables.  The following tables always exist:

 - A posting list table, which holds a list of all the documents indexed by
   each term in the database.
 - A record table, which holds the document data associated with each document
   in the database.
 - A termlist table, which holds a list of all the terms which index each
   document.

And the following optional tables exist only when there is data to store in
them (in 1.0.1 and earlier, the position and value tables were always created
even if empty; spelling and synonym tables are new in 1.0.2):

 - A position list table, which holds a list of all the word positions in each
   document which each term occurs at.
 - A value table, which holds the "values" (used for sorting, collapsing, and
   other match-time calculations) associated with each document in the
   database.
 - A spelling table, which holds data for suggesting spelling corrections.
 - A synonym table, which holds a synonym dictionary.

Each of the tables is held in a separate file, allowing an administrator to
see how much data is being used for each of the above purposes.  It is not
always necessary to fully populate these tables: for example, if phrase
searches are never going to be performed on the database, it is not necessary
to store any positionlist information.

If you look at a Xapian database, you will see that each of these tables
actually uses 2 or 3 files.  For example, for a "flint" format database the
termlist table is stored in the files "termlist.baseA", "termlist.baseB"
and "termlist.DB".

Of these files, only the ".DB" file actually stores the data.  The ".baseA"
and ".baseB" files are used to keep track of where to start looking for that
data in the ".DB" file.  Often, only one of the ".baseA" and ".baseB" files
will be present; each of these files refers to a revision of the database, and
there may be more than one valid revision of the database stored in the ".DB"
file at once.

The ".DB" file is structured as a set of blocks, which have a default size of
8KB (though this can be set, either through the Xapian API, or with some of
the tools detailed later in this document).  The first block is used for
header information, so a ".DB" file with only a single entry will be 16KB in
size.

Changing the blocksize may have performance implications, but it is hard to
tell whether these will be positive or negative for a particular combination
of hardware and software without doing some profiling.

Atomic modifications
--------------------

Xapian ensures that all modifications to its database are performed
atomically.  This means that:

 - From the point of view of a separate process (or a separate database object
   in the same process) reading the database, all modifications made to a
   database are invisible until the modifications is committed.
 - The database on disk is always in a consistent state.
 - If the system is interrupted during a modification, the database should
   always be left in a valid state.  This applies even if the power is cut
   unexpectedly, as long as the disk does not become corrupted due to hardware
   failure.

Committing a modification requires several calls to the operating system to
make it flush any cached modifications to the database to disk.  This is to
ensure that if the system fails at any point, the database is left in a
consistent state.  Of course, this is a fairly slow process (since the system
has to wait for the disk to physically write the data), so grouping many
changes together will speed up the throughput considerably.

Many modifications can be explicitly grouped into a single transaction, so
that lots of changes are applied at once.  Even if an application doesn't
explicitly protect modifications to the database using transactions, Xapian
will group modifications into transactions, applying the modifications in
batches.

Note that it is not currently possible to extend Xapian's transactions to
cover multiple databases, or to link them with transactions in external
systems, such as an RDBMS.

Finally, note that it is possible to compile Xapian such that it doesn't make
modifications in an atomic manner, in order to build very large databases more
quickly (search the Xapian mailing list archives for "DANGEROUS" mode for more
details).  This isn't yet integrated into standard builds of Xapian, but may
be in future, if appropriate protections can be incorporated.

Single writer, multiple reader
------------------------------

Xapian implements a "single writer, multiple reader" model.  This means that,
at any given instant, there is only permitted to be a single object modifying
a database, but there may (simultaneously) be many objects reading the
database at once.

Xapian enforces this restriction using lock-files.  For a flint database, each
Xapian database directory contains a lock file named ``flintlock``.  The
lock-file will always exist, but will be locked using ``fcntl()`` when the
database is open for writing.  If a writer exits without being given a
chance to clean up (for example, if the application holding the writer
is killed), the ``fcntl()`` lock will be automatically released by the operating
system.  Under Microsoft Windows, we use a different locking technique, but
with the same features.

Revision numbers
----------------

Xapian databases contain a revision number.  This is essentially a count of
the number of modifications since the database was created, and is needed to
implement the atomic modification functionality.  It is stored as a 32 bit
integer, so there is a chance that a very frequently updated database could
cause this to overflow.  The consequence of such an overflow would be to throw
database errors.

This isn't likely to be a practical problem, since it would take nearly a year
for a database to reach this limit if 100 modifications were committed every
second, and no normal Xapian system will commit more than once every few
seconds.  However, if you are concerned, you can use the ``xapian-compact``
tool to make a fresh copy of the database with the revision number set to 1.

For a "flint" database, the revision number of each table can be displayed by
the ``xapian-check`` tool.

Network file systems
--------------------

Xapian should work correctly over a network file system.  However, there are a
large number of potential issues with such file systems, so we recommend
extensive testing of your particular network file system before deployment.

Be warned that Xapian is heavily I/O dependent, and therefore performance over
a network file system is likely to be slow unless you've got a very well tuned
setup.

Xapian needs to be able to create a lock file in a database directory when
modifications are being performed.  On some network files systems (e.g., NFS)
this requires a lock daemon to be running.

Which database format to use?
-----------------------------

As of release 1.0.0, you should use the flint format (which is now the
default).  The quartz format is now deprecated and support is scheduled
for removal in 1.1.0.

Can I put other files in the database directory?
------------------------------------------------

If you wish to store meta-data or other information relating to the Xapian
database, it is reasonable to wish to put this in files inside the Xapian
database directory, for neatness.  For example, you might wish to store a list
of the prefixes you've applied to terms for specific fields in the database.

Xapian's "flint" backend doesn't perform any operations
which will break this technique, so as long as you don't choose a filename
that Xapian uses itself, there should be no problems.  However, be aware that
new versions of Xapian may use new files in the database directory, and it is
also possible that new backend formats may not be compatible with the
technique (e.g., it is possible that a future backend could store its entire
database in a single file, not in a directory).


Backup Strategies
=================

Summary
-------

 - The simplest way to perform a backup is to temporarily halt modifications,
   take a copy of all files in the database directory, and then allow
   modifications to resume.  Read access can continue while a backup is being
   taken.

 - If you have a filesystem which allows atomic snapshots to be taken of
   directories (such as an LVM filesystem), an alternative strategy is to take
   a snapshot and simply copy all the files in the database directory to the
   backup medium.  Such a copy will always be a valid database.

 - Progressive backups are not easily possible; modifications are typically
   spread throughout the database files.

Detail
------

Even though Xapian databases are often automatically generated from source
data which is stored in a reliable manner, it is usually desirable to keep
backups of Xapian databases being run in production environments.  This is
particularly important in systems with high-availability requirements, since
re-building a Xapian database from scratch can take many hours.  It is also
important in the case where the data stored in the database cannot easily be
recovered from external sources.

Xapian databases are managed such that at any instant in time, there is at
least one valid revision of the database written to disk (and if there are
multiple valid revisions, Xapian will always open the most recent).
Therefore, if it is possible to take an instantaneous snapshot of all the
database files (for example, on an LVM filesystem), this snapshot is suitable
for copying to a backup medium.  Note that it is not sufficient to take a
snapshot of each database file in turn - the snapshot must be across all
database files simultaneously.  Otherwise, there is a risk that the snapshot
could contain database files from different revisions.

If it is not possible to take an instantaneous snapshot, the best backup
strategy is simply to ensure that no modifications are committed during the
backup procedure.  While the simplest way to implement this may be to stop
whatever processes are used to modify the database, and ensure that they close
the database, it is not actually necessary to ensure that no writers are open
on the database; it is enough to ensure that no writer makes any modification
to the database.

Because a Xapian database can contain more than one valid revision of the
database, it is actually possible to allow a limited number of modifications
to be performed while a backup copy is being made, but this is tricky and we
do not recommend relying on it.  Future versions of Xapian are likely to
support this better, by allowing the current revision of a database to be
preserved while modifications continue.

Progressive backups are not recommended for Xapian databases: Xapian database
files are block-structured, and modifications are spread throughout the
database file.  Therefore, a progressive backup tool will not be able to take
a backup by storing only the new parts of the database.  Modifications will
normally be so extensive that most parts of the database have been modified,
however, if only a small number of modifications have been made, a binary diff
algorithm might make a usable progressive backup tool.


Inspecting a database
=====================

When designing an indexing strategy, it is often useful to be able to check
the contents of the database.  Xapian includes a simple command-line program,
"delve", to allow this.

For example, to display the list of terms in document "1" of the database
"foo", use::

  delve foo -r 1

It is also possible to perform simple searches of a database.  Xapian includes
another simple command-line program, "quest", to support this.  "quest" is
only able to search for un-prefixed terms, the query string must be quoted to
protect it from the shell.  To search the database "foo" for the phrase "hello
world", use::

  quest -d foo '"hello world"'

If you have installed the "Omega" CGI application built on Xapian, this can
also be used with the built-in "godmode" template to provide a web-based
interface for browsing a database.  See Omega's documentation for more details
on this.

Database maintenance
====================

Compacting a database
---------------------

Xapian databases normally have some spare space in each block to allow
new information to be efficiently slotted into the database.  However, the
smaller a database is, the faster it can be searched, so if there aren't
expected to be many further modifications, it can be desirable to compact the
database.

Xapian includes a tool, "xapian-compact" for compacting "flint" format
databases.
This tool makes a copy of a database, and takes advantage of the sorted nature
of the source Xapian database to write the database out without leaving so
much space for future modifications.  This can result in a large space saving.

The downside of these tools is that future modifications may take a little
longer, due to needing to reorganise the database to make space for them.
However, modifications are still possible, and if many modifications are made,
the database will eventually adjust itself.

The tools have an option ("-F") to perform a "fuller" compaction.  This option
compacts the database as much as possible, but it violates the design of the
Btree format slightly to achieve this, so it is not recommended if further
modifications are at all likely in future.  If you do need to modify a "fuller"
compacted database, we recommend you run xapian-compact on it without "-F"
first.

While taking a copy of the database, it is also possible to change the
blocksize.  If you wish to profile search speed with different blocksizes,
this is the recommended way to generate the different databases (but remember
to compact the original database as well, for a fair comparison).


Merging databases
-----------------

When building an index for a very large amount of data, it can be desirable to
index the data in smaller chunks (perhaps on separate machines), and then
merge the chunks together into a single database.  This can also be performed
using the "xapian-compact" tool, simply by supplying
several source database paths.

Normally, merging works by reading the source databases in parallel, and
writing the contents in sorted order to the destination database.  This will
work most efficiently if excessive disk seeking can be avoided; if you have
several disks, it may be worth placing the source databases and the
destination database on separate disks to obtain maximum speed.

The ``xapian-compact`` tool supports an additional option, ``--multipass``,
which is useful when merging more than three databases.  This will cause the
postlist tables to be grouped and merged into temporary tables, which are then
grouped and merged, and so on until a single postlist table is created, which
is usually faster, but requires more disk space for the temporary files.


Checking database integrity
---------------------------

Xapian includes a command-line tool to check that a flint database is
self-consistent.  This tool, "xapian-check", runs through the entire database,
checking that all the internal nodes are correctly connected.  It can also be
used on a single table in a flint database, by specifying the prefix of the
table: for example, this command will check the termlist table of database "foo"::

  xapian-check foo/termlist


Converting a quartz database to a flint database
------------------------------------------------

It is possible to convert a quartz database to a flint database using the
"copydatabase" example program included with Xapian.  This is a lot slower to
run than "quartzcompact" or "xapian-compact", since it has to perform the
sorting of the term occurrence data from scratch, but should be faster than a
re-index from source data since it doesn't need to perform the tokenisation
step.  It is also useful if you no longer have the source data available.

The following command will copy a database from "SOURCE" to "DESTINATION",
creating the new database at "DESTINATION" as a flint database::

  copydatabase SOURCE DESTINATION


Converting a 0.9.x flint database to work with 1.0.y
----------------------------------------------------

Due to a bug in the flint position list encoding in 0.9.x which made flint
databases non-portable between platforms, we had to make an incompatible
change in the flint format.  It's not easy to write an upgrader, but you
can convert a database using the following procedure (although it might
be better to rebuild from scratch if you want to use the new UTF-8 support
in Xapian::QueryParser, Xapian::Stem, and Xapian::TermGenerator).

Run the following command in your Xapian 0.9.x installation to copy your
0.9.x flint database "SOURCE" to a new quartz database "INTERMEDIATE"::

  copydatabase SOURCE INTERMEDIATE

Then run the following command in your Xapian 1.0.y installation to copy
your quartz database to a 1.0.y flint database "DESTINATION"::

  copydatabase INTERMEDIATE DESTINATION
