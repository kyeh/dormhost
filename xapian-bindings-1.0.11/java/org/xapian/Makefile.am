SUBDIRS = errors . examples

EXTRA_DIST = Database.java Document.java ESet.java ESetIterator.java\
 Enquire.java ExpandDecider.java MSet.java MSetIterator.java MatchDecider.java\
 PositionIterator.java Query.java RSet.java Stem.java TermIterator.java\
 WritableDatabase.java Xapian.java XapianJNI.java

XAPIAN_CLASS_FILES = Database.class Document.class ESet.class\
 ESetIterator.class Enquire.class ExpandDecider.class MSet.class\
 MSetIterator.class MatchDecider.class PositionIterator.class Query.class\
 RSet.class Stem.class TermIterator.class WritableDatabase.class\
 Xapian.class XapianJNI.class

# These inner class files get generated as a side-effect of generating the
# files listed in XAPIAN_CLASS_FILES.  We need a list of them so that the
# clean target can remove them.
XAPIAN_INNER_CLASS_FILES = TermIterator\$$Info.class Xapian\$$Auto.class\
 Xapian\$$InMemory.class Xapian\$$Quartz.class Xapian\$$Remote.class

noinst_DATA = $(XAPIAN_CLASS_FILES)

.java.class:
	-rm -f ../../built/xapian_jni.jar
	$(JAVAC) -classpath $(srcdir)/../..$(JAVA_PATHSEP)../.. -d ../.. $<

CLEANFILES = $(XAPIAN_CLASS_FILES) $(XAPIAN_INNER_CLASS_FILES)

# something like this, but javadoc doesn't run on my system
#make-java-docs:
#	javadoc sourcepath=$(srcdir) destdir=docs packagenames='org.xapian.*'
