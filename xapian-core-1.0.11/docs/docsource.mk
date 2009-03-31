APIDOC_SRC=\
	$T/include/xapian.h\
	$T/include/xapian/base.h\
	$T/include/xapian/database.h\
	$T/include/xapian/dbfactory.h\
	$T/include/xapian/deprecated.h\
	$T/include/xapian/document.h\
	$T/include/xapian/enquire.h\
	$(top_builddir)/include/xapian/error.h\
	$T/include/xapian/errorhandler.h\
	$T/include/xapian/expanddecider.h\
	$T/include/xapian/positioniterator.h\
	$T/include/xapian/postingiterator.h\
	$T/include/xapian/query.h\
	$T/include/xapian/queryparser.h\
	$T/include/xapian/sorter.h\
	$T/include/xapian/stem.h\
	$T/include/xapian/termgenerator.h\
	$T/include/xapian/termiterator.h\
	$T/include/xapian/types.h\
	$T/include/xapian/unicode.h\
	$T/include/xapian/valueiterator.h\
	$(top_builddir)/include/xapian/version.h\
	$T/include/xapian/visibility.h
SOURCEDOC_SRC=\
	$T/api/editdistance.cc\
	$T/api/editdistance.h\
	$T/api/error.cc\
	$T/api/errorhandler.cc\
	$T/api/expanddecider.cc\
	$T/api/leafpostlist.cc\
	$T/api/maptermlist.h\
	$T/api/omdatabase.cc\
	$T/api/omdocument.cc\
	$T/api/omenquire.cc\
	$T/api/ompositionlistiterator.cc\
	$T/api/ompostlistiterator.cc\
	$T/api/omquery.cc\
	$T/api/omqueryinternal.cc\
	$T/api/omtermlistiterator.cc\
	$T/api/omvalueiterator.cc\
	$T/api/postlist.cc\
	$T/api/sortable-serialise.cc\
	$T/api/sorter.cc\
	$T/api/termlist.cc\
	$T/api/valuerangeproc.cc\
	$T/api/valuerangeproccompat.cc\
	$T/api/version.cc\
	$T/api/weightinternal.cc\
	$T/backends/alltermslist.cc\
	$T/backends/database.cc\
	$T/backends/dbfactory_remote.cc\
	$T/backends/flint/contiguousalldocspostlist.cc\
	$T/backends/flint/contiguousalldocspostlist.h\
	$T/backends/flint/flint_alldocspostlist.cc\
	$T/backends/flint/flint_alldocspostlist.h\
	$T/backends/flint/flint_alltermslist.cc\
	$T/backends/flint/flint_alltermslist.h\
	$T/backends/flint/flint_btreebase.cc\
	$T/backends/flint/flint_btreebase.h\
	$T/backends/flint/flint_btreeutil.h\
	$T/backends/flint/flint_check.cc\
	$T/backends/flint/flint_check.h\
	$T/backends/flint/flint_cursor.cc\
	$T/backends/flint/flint_cursor.h\
	$T/backends/flint/flint_database.cc\
	$T/backends/flint/flint_database.h\
	$T/backends/flint/flint_document.cc\
	$T/backends/flint/flint_document.h\
	$T/backends/flint/flint_io.cc\
	$T/backends/flint/flint_io.h\
	$T/backends/flint/flint_lock.cc\
	$T/backends/flint/flint_lock.h\
	$T/backends/flint/flint_metadata.cc\
	$T/backends/flint/flint_metadata.h\
	$T/backends/flint/flint_modifiedpostlist.cc\
	$T/backends/flint/flint_modifiedpostlist.h\
	$T/backends/flint/flint_positionlist.cc\
	$T/backends/flint/flint_positionlist.h\
	$T/backends/flint/flint_postlist.cc\
	$T/backends/flint/flint_postlist.h\
	$T/backends/flint/flint_record.cc\
	$T/backends/flint/flint_record.h\
	$T/backends/flint/flint_spelling.cc\
	$T/backends/flint/flint_spelling.h\
	$T/backends/flint/flint_spellingwordslist.cc\
	$T/backends/flint/flint_spellingwordslist.h\
	$T/backends/flint/flint_synonym.cc\
	$T/backends/flint/flint_synonym.h\
	$T/backends/flint/flint_table.cc\
	$T/backends/flint/flint_table.h\
	$T/backends/flint/flint_termlist.cc\
	$T/backends/flint/flint_termlist.h\
	$T/backends/flint/flint_termlisttable.cc\
	$T/backends/flint/flint_termlisttable.h\
	$T/backends/flint/flint_types.h\
	$T/backends/flint/flint_utils.h\
	$T/backends/flint/flint_values.cc\
	$T/backends/flint/flint_values.h\
	$T/backends/flint/flint_version.cc\
	$T/backends/flint/flint_version.h\
	$T/backends/inmemory/inmemory_alltermslist.cc\
	$T/backends/inmemory/inmemory_alltermslist.h\
	$T/backends/inmemory/inmemory_database.cc\
	$T/backends/inmemory/inmemory_database.h\
	$T/backends/inmemory/inmemory_document.cc\
	$T/backends/inmemory/inmemory_document.h\
	$T/backends/inmemory/inmemory_positionlist.cc\
	$T/backends/multi/multi_alltermslist.cc\
	$T/backends/multi/multi_postlist.cc\
	$T/backends/multi/multi_postlist.h\
	$T/backends/multi/multi_termlist.cc\
	$T/backends/multi/multi_termlist.h\
	$T/backends/quartz/bcursor.cc\
	$T/backends/quartz/bcursor.h\
	$T/backends/quartz/btree.cc\
	$T/backends/quartz/btree.h\
	$T/backends/quartz/btree_base.cc\
	$T/backends/quartz/btree_base.h\
	$T/backends/quartz/btree_util.h\
	$T/backends/quartz/btreecheck.cc\
	$T/backends/quartz/btreecheck.h\
	$T/backends/quartz/quartz_alldocspostlist.cc\
	$T/backends/quartz/quartz_alldocspostlist.h\
	$T/backends/quartz/quartz_alltermslist.cc\
	$T/backends/quartz/quartz_alltermslist.h\
	$T/backends/quartz/quartz_database.cc\
	$T/backends/quartz/quartz_database.h\
	$T/backends/quartz/quartz_document.cc\
	$T/backends/quartz/quartz_document.h\
	$T/backends/quartz/quartz_log.cc\
	$T/backends/quartz/quartz_log.h\
	$T/backends/quartz/quartz_metafile.cc\
	$T/backends/quartz/quartz_metafile.h\
	$T/backends/quartz/quartz_positionlist.cc\
	$T/backends/quartz/quartz_positionlist.h\
	$T/backends/quartz/quartz_postlist.cc\
	$T/backends/quartz/quartz_postlist.h\
	$T/backends/quartz/quartz_record.cc\
	$T/backends/quartz/quartz_record.h\
	$T/backends/quartz/quartz_termlist.cc\
	$T/backends/quartz/quartz_termlist.h\
	$T/backends/quartz/quartz_types.h\
	$T/backends/quartz/quartz_utils.h\
	$T/backends/quartz/quartz_values.cc\
	$T/backends/quartz/quartz_values.h\
	$T/backends/remote/net_document.cc\
	$T/backends/remote/net_document.h\
	$T/backends/remote/net_postlist.cc\
	$T/backends/remote/net_postlist.h\
	$T/backends/remote/net_termlist.cc\
	$T/backends/remote/net_termlist.h\
	$T/backends/remote/remote-database.cc\
	$T/bin/quartzcheck.cc\
	$T/bin/quartzcompact.cc\
	$T/bin/quartzdump.cc\
	$T/bin/xapian-check.cc\
	$T/bin/xapian-compact.cc\
	$T/bin/xapian-inspect.cc\
	$T/bin/xapian-progsrv.cc\
	$T/bin/xapian-tcpsrv.cc\
	$T/common/alltermslist.h\
	$T/common/autoptr.h\
	$T/common/database.h\
	$T/common/document.h\
	$T/common/documentterm.h\
	$T/common/emptypostlist.h\
	$T/common/expand.h\
	$T/common/expandweight.h\
	$T/common/getopt.cc\
	$T/common/gnu_getopt.h\
	$T/common/inmemory_positionlist.h\
	$T/common/leafpostlist.h\
	$T/common/msvc_dirent.cc\
	$T/common/msvc_dirent.h\
	$T/common/msvc_posix_wrapper.cc\
	$T/common/msvc_posix_wrapper.h\
	$T/common/multialltermslist.h\
	$T/common/multimatch.h\
	$T/common/noreturn.h\
	$T/common/omassert.h\
	$T/common/omdebug.cc\
	$T/common/omdebug.h\
	$T/common/omenquireinternal.h\
	$T/common/omqueryinternal.h\
	$T/common/omtime.h\
	$T/common/ortermlist.h\
	$T/common/output.h\
	$T/common/positionlist.h\
	$T/common/postlist.h\
	$T/common/progclient.h\
	$T/common/remote-database.h\
	$T/common/remoteconnection.h\
	$T/common/remoteprotocol.h\
	$T/common/remoteserver.h\
	$T/common/rset.h\
	$T/common/safe.cc\
	$T/common/safeerrno.h\
	$T/common/safefcntl.h\
	$T/common/safesysselect.h\
	$T/common/safesysstat.h\
	$T/common/safeunistd.h\
	$T/common/safewindows.h\
	$T/common/safewinsock2.h\
	$T/common/serialise-double.cc\
	$T/common/serialise-double.h\
	$T/common/serialise.h\
	$T/common/stats.h\
	$T/common/stringutils.cc\
	$T/common/stringutils.h\
	$T/common/submatch.h\
	$T/common/tcpclient.h\
	$T/common/tcpserver.h\
	$T/common/termlist.h\
	$T/common/utils.cc\
	$T/common/utils.h\
	$T/common/vectortermlist.h\
	$T/common/weightinternal.h\
	$T/examples/copydatabase.cc\
	$T/examples/delve.cc\
	$T/examples/quest.cc\
	$T/examples/simpleexpand.cc\
	$T/examples/simpleindex.cc\
	$T/examples/simplesearch.cc\
	$T/expand/expand.cc\
	$T/expand/expandweight.cc\
	$T/expand/ortermlist.cc\
	$(top_builddir)/languages/allsnowballheaders.h\
	$(top_builddir)/languages/danish.cc\
	$(top_builddir)/languages/danish.h\
	$(top_builddir)/languages/dutch.cc\
	$(top_builddir)/languages/dutch.h\
	$(top_builddir)/languages/english.cc\
	$(top_builddir)/languages/english.h\
	$(top_builddir)/languages/finnish.cc\
	$(top_builddir)/languages/finnish.h\
	$(top_builddir)/languages/french.cc\
	$(top_builddir)/languages/french.h\
	$(top_builddir)/languages/german.cc\
	$(top_builddir)/languages/german.h\
	$(top_builddir)/languages/german2.cc\
	$(top_builddir)/languages/german2.h\
	$(top_builddir)/languages/hungarian.cc\
	$(top_builddir)/languages/hungarian.h\
	$(top_builddir)/languages/italian.cc\
	$(top_builddir)/languages/italian.h\
	$(top_builddir)/languages/kraaij_pohlmann.cc\
	$(top_builddir)/languages/kraaij_pohlmann.h\
	$(top_builddir)/languages/lovins.cc\
	$(top_builddir)/languages/lovins.h\
	$(top_builddir)/languages/norwegian.cc\
	$(top_builddir)/languages/norwegian.h\
	$(top_builddir)/languages/porter.cc\
	$(top_builddir)/languages/porter.h\
	$(top_builddir)/languages/portuguese.cc\
	$(top_builddir)/languages/portuguese.h\
	$(top_builddir)/languages/romanian.cc\
	$(top_builddir)/languages/romanian.h\
	$(top_builddir)/languages/russian.cc\
	$(top_builddir)/languages/russian.h\
	$(top_builddir)/languages/spanish.cc\
	$(top_builddir)/languages/spanish.h\
	$T/languages/stem.cc\
	$T/languages/steminternal.cc\
	$T/languages/steminternal.h\
	$(top_builddir)/languages/swedish.cc\
	$(top_builddir)/languages/swedish.h\
	$(top_builddir)/languages/turkish.cc\
	$(top_builddir)/languages/turkish.h\
	$T/matcher/andmaybepostlist.cc\
	$T/matcher/andmaybepostlist.h\
	$T/matcher/andnotpostlist.cc\
	$T/matcher/andnotpostlist.h\
	$T/matcher/andpostlist.cc\
	$T/matcher/andpostlist.h\
	$T/matcher/bm25weight.cc\
	$T/matcher/branchpostlist.cc\
	$T/matcher/branchpostlist.h\
	$T/matcher/emptysubmatch.cc\
	$T/matcher/emptysubmatch.h\
	$T/matcher/exactphrasepostlist.cc\
	$T/matcher/exactphrasepostlist.h\
	$T/matcher/extraweightpostlist.h\
	$T/matcher/localmatch.cc\
	$T/matcher/localmatch.h\
	$T/matcher/mergepostlist.cc\
	$T/matcher/mergepostlist.h\
	$T/matcher/msetcmp.cc\
	$T/matcher/msetcmp.h\
	$T/matcher/msetpostlist.cc\
	$T/matcher/msetpostlist.h\
	$T/matcher/multiandpostlist.cc\
	$T/matcher/multiandpostlist.h\
	$T/matcher/multimatch.cc\
	$T/matcher/orpostlist.cc\
	$T/matcher/orpostlist.h\
	$T/matcher/phrasepostlist.cc\
	$T/matcher/phrasepostlist.h\
	$T/matcher/queryoptimiser.cc\
	$T/matcher/queryoptimiser.h\
	$T/matcher/remotesubmatch.cc\
	$T/matcher/remotesubmatch.h\
	$T/matcher/rset.cc\
	$T/matcher/scaleweight.cc\
	$T/matcher/scaleweight.h\
	$T/matcher/selectpostlist.cc\
	$T/matcher/selectpostlist.h\
	$T/matcher/stats.cc\
	$T/matcher/tradweight.cc\
	$T/matcher/valuegepostlist.cc\
	$T/matcher/valuegepostlist.h\
	$T/matcher/valuerangepostlist.cc\
	$T/matcher/valuerangepostlist.h\
	$T/matcher/weight.cc\
	$T/matcher/xorpostlist.cc\
	$T/matcher/xorpostlist.h\
	$T/net/progclient.cc\
	$T/net/remoteconnection.cc\
	$T/net/remoteserver.cc\
	$T/net/serialise.cc\
	$T/net/tcpclient.cc\
	$T/net/tcpserver.cc\
	$T/queryparser/queryparser.cc\
	$(top_builddir)/queryparser/queryparser_internal.cc\
	$T/queryparser/queryparser_internal.h\
	$(top_builddir)/queryparser/queryparser_token.h\
	$T/queryparser/termgenerator.cc\
	$T/queryparser/termgenerator_internal.cc\
	$T/queryparser/termgenerator_internal.h\
	$T/unicode/tclUniData.cc\
	$T/unicode/utf8itor.cc
DIR_CONTENTS_FILES=\
	$T/dir_contents\
	$T/api/dir_contents\
	$T/bin/dir_contents\
	$T/backends/dir_contents\
	$T/backends/flint/dir_contents\
	$T/backends/inmemory/dir_contents\
	$T/backends/multi/dir_contents\
	$T/backends/quartz/dir_contents\
	$T/backends/remote/dir_contents\
	$T/common/dir_contents\
	$T/examples/dir_contents\
	$T/expand/dir_contents\
	$T/include/dir_contents\
	$T/languages/dir_contents\
	$T/matcher/dir_contents\
	$T/net/dir_contents\
	$T/queryparser/dir_contents\
	$T/unicode/dir_contents\
	$T/docs/dir_contents\
	$T/tests/dir_contents\
	$T/tests/harness/dir_contents
