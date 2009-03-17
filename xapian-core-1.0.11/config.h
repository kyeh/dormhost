/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if ftime returns void */
/* #undef FTIME_RETURNS_VOID */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `fork' function. */
#define HAVE_FORK 1

/* Define to 1 if you have the `fsync' function. */
#define HAVE_FSYNC 1

/* Define to 1 if you have the `ftime' function. */
#define HAVE_FTIME 1

/* Define to 1 if you have the `gethostname' function. */
#define HAVE_GETHOSTNAME 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `hstrerror' function. */
#define HAVE_HSTRERROR 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the `link' function. */
#define HAVE_LINK 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if pread is available on this system */
#define HAVE_PREAD 1

/* Define if pwrite is available on this system */
#define HAVE_PWRITE 1

/* Define to 1 if you have the 'socketpair' function. */
#define HAVE_SOCKETPAIR 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <streambuf> header file. */
#define HAVE_STREAMBUF 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/errno.h> header file. */
#define HAVE_SYS_ERRNO_H 1

/* Define to 1 if you have the <sys/select.h> header file. */
#define HAVE_SYS_SELECT_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/utsname.h> header file. */
#define HAVE_SYS_UTSNAME_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define if a suitable valgrind is installed */
/* #undef HAVE_VALGRIND */

/* Define to 1 if you have the <zlib.h> header file. */
#define HAVE_ZLIB_H 1

/* Name of package */
#define PACKAGE "xapian-core"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://xapian.org/bugs"

/* Define to the full name of this package. */
#define PACKAGE_NAME "xapian-core"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "xapian-core 1.0.11"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "xapian-core"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0.11"

/* explicit prototype needed for pread (if any) */
/* #undef PREAD_PROTOTYPE */

/* explicit prototype needed for pwrite (if any) */
/* #undef PWRITE_PROTOTYPE */

/* The size of `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* Define to the name of a function implementing snprintf but not caring about
   ISO C90 return value semantics (if one exists) */
#define SNPRINTF snprintf

/* Define to the name of a function implementing snprintf with ISO C90
   semantics (if one exists) */
#define SNPRINTF_ISO snprintf

/* type to use for 5th parameter to getsockopt */
#define SOCKLEN_T socklen_t

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define if the testsuite should use RTTI */
#define USE_RTTI 1

/* Version number of package */
#define VERSION "1.0.11"

/* Define if you want assertions (causes some slow-down) */
/* #undef XAPIAN_DEBUG */

/* Define if you want paranoid assertions (causes significant slow-down) */
/* #undef XAPIAN_DEBUG_PARANOID */

/* Define if you want code profiling via the logging infrastructure */
/* #undef XAPIAN_DEBUG_PROFILE */

/* Define if you want a log of methods called and other debug messages */
/* #undef XAPIAN_DEBUG_VERBOSE */

/* Define to disable use of visibility attributes */
/* #undef XAPIAN_DISABLE_VISIBILITY */

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

/* Disable stupid MSVC "performance" warning for converting int to bool. */
#ifdef _MSC_VER
# pragma warning(disable:4800)
#endif

/* _FORTIFY_SOURCE is only supported by GCC >= 4.1 and glibc >= 2.3.4, but it
 * shouldn't cause a problem to define it where it's not supported and some
 * distros may have backported support, so hardcoding version checks is
 * counter-productive.
 *
 * Check if _FORTIFY_SOURCE is already defined to allow the user to override
 * our choice with "./configure CPPFLAGS=-D_FORTIFY_SOURCE=0" or "...=1".
 */
#if defined __GNUC__ && !defined _FORTIFY_SOURCE
# define _FORTIFY_SOURCE 2
#endif

/* For GCC >= 3.0 (and Intel's C++ compiler, which also defines __GNUC__),
 * we can use __builtin_expect to give the compiler hints about branch
 * prediction.  See HACKING for how to use these.
 */
#if defined __GNUC__ && __GNUC__ >= 3
/* The arguments of __builtin_expect() are both long, so use !! to ensure that
 * the first argument is always an integer expression, and always 0 or 1, but
 * still has the same truth value for the if or while it is used in.
 */
# define rare(COND) __builtin_expect(!!(COND), 0)
# define usual(COND) __builtin_expect(!!(COND), 1)
#else
# define rare(COND) (COND)
# define usual(COND) (COND)
#endif

