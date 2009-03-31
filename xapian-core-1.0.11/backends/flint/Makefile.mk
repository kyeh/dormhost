EXTRA_DIST +=\
	backends/flint/dir_contents\
	backends/flint/Makefile

if BUILD_BACKEND_FLINT
noinst_HEADERS +=\
	backends/flint/contiguousalldocspostlist.h\
	backends/flint/flint_alldocspostlist.h\
	backends/flint/flint_alltermslist.h\
	backends/flint/flint_btreebase.h\
	backends/flint/flint_btreeutil.h\
	backends/flint/flint_check.h\
	backends/flint/flint_cursor.h\
	backends/flint/flint_database.h\
	backends/flint/flint_document.h\
	backends/flint/flint_io.h\
	backends/flint/flint_lock.h\
	backends/flint/flint_metadata.h\
	backends/flint/flint_modifiedpostlist.h\
	backends/flint/flint_positionlist.h\
	backends/flint/flint_postlist.h\
	backends/flint/flint_record.h\
	backends/flint/flint_spelling.h\
	backends/flint/flint_spellingwordslist.h\
	backends/flint/flint_synonym.h\
	backends/flint/flint_table.h\
	backends/flint/flint_termlist.h\
	backends/flint/flint_termlisttable.h\
	backends/flint/flint_types.h\
	backends/flint/flint_utils.h\
	backends/flint/flint_values.h\
	backends/flint/flint_version.h

libxapian_la_SOURCES +=\
	backends/flint/contiguousalldocspostlist.cc\
	backends/flint/flint_alldocspostlist.cc\
	backends/flint/flint_alltermslist.cc\
	backends/flint/flint_btreebase.cc\
	backends/flint/flint_cursor.cc\
	backends/flint/flint_database.cc\
	backends/flint/flint_document.cc\
	backends/flint/flint_io.cc\
	backends/flint/flint_lock.cc\
	backends/flint/flint_metadata.cc\
	backends/flint/flint_modifiedpostlist.cc\
	backends/flint/flint_positionlist.cc\
	backends/flint/flint_postlist.cc\
	backends/flint/flint_record.cc\
	backends/flint/flint_spelling.cc\
	backends/flint/flint_spellingwordslist.cc\
	backends/flint/flint_synonym.cc\
	backends/flint/flint_table.cc\
	backends/flint/flint_termlist.cc\
	backends/flint/flint_termlisttable.cc\
	backends/flint/flint_values.cc\
	backends/flint/flint_version.cc

noinst_LTLIBRARIES += libflintcheck.la

libflintcheck_la_SOURCES =\
	backends/flint/flint_check.cc
endif
