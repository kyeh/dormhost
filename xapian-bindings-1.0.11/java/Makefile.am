## Process this file with automake to produce Makefile.in

# `make QUIET=' overrides `./configure --enable-quiet'.
# `make QUIET=y' overrides `./configure' without `--enable-quiet'.
LIBTOOL = @LIBTOOL@ $(QUIET:y=--quiet)

SUBDIRS = native org .

TESTS_ENVIRONMENT = \
  LIBTOOL="$(LIBTOOL)" JAVA="$(JAVA)" PATHSEP="$(JAVA_PATHSEP)" $(srcdir)/run-java-test

## Test programs to be run
TESTS = SmokeTest.class

EXTRA_DIST = run-java-test SmokeTest.java

CLEANFILES = SmokeTest.class MyMatchDecider.class MyExpandDecider.class \
	built/xapian_jni.jar

SmokeTest: SmokeTest.class

.java.class:
	$(JAVAC) -classpath $(srcdir)$(JAVA_PATHSEP). -d . $<

noinst_DATA = built/xapian_jni.jar

built/xapian_jni.jar:
	-test -d built || mkdir built
	$(JAR) -cf built/xapian_jni.jar org/xapian/*.class org/xapian/errors/*.class
