EXTRA_DIST +=\
	include/dir_contents\
	include/Makefile\
	include/xapian/dir_contents\
	include/xapian/Makefile

xapianincludedir = $(includedir)/xapian

include_HEADERS =\
	include/xapian.h

xapianinclude_HEADERS =\
	include/xapian/base.h\
	include/xapian/database.h\
	include/xapian/dbfactory.h\
	include/xapian/deprecated.h\
	include/xapian/document.h\
	include/xapian/enquire.h\
	include/xapian/errorhandler.h\
	include/xapian/expanddecider.h\
	include/xapian/positioniterator.h\
	include/xapian/postingiterator.h\
	include/xapian/query.h\
	include/xapian/queryparser.h\
	include/xapian/sorter.h\
	include/xapian/stem.h\
	include/xapian/termgenerator.h\
	include/xapian/termiterator.h\
	include/xapian/types.h\
	include/xapian/unicode.h\
	include/xapian/valueiterator.h\
	include/xapian/visibility.h

nodist_xapianinclude_HEADERS =\
	include/xapian/version.h

# Regenerate include/xapian/version.h if its template has been changed.
all-local: include/xapian/version.h.timestamp

include/xapian/version.h.timestamp: include/xapian/version_h.cc
	$(SHELL) ./config.status --recheck

EXTRA_DIST +=\
	include/xapian/version_h.cc

DISTCLEANFILES +=\
	include/xapian/version.h\
	include/xapian/version.h.timestamp
