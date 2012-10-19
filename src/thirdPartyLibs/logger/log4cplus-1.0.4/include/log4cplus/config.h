/* include/log4cplus/config.h.  Generated from config.h.in by configure.  */
/* include/log4cplus/config.h.in.  Generated from configure.in by autoheader.  */

#ifndef LOG4CPLUS_CONFIG_H

#define LOG4CPLUS_CONFIG_H

/* Define to 1 if you have the `clock_gettime' function. */
/* #undef HAVE_CLOCK_GETTIME */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the `ftime' function. */
#define HAVE_FTIME 1

/* */
#define HAVE_GETADDRINFO 1

/* */
#define HAVE_GETHOSTBYNAME_R 1

/* Define to 1 if you have the `getpid' function. */
#define HAVE_GETPID 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `gmtime_r' function. */
#define HAVE_GMTIME_R 1

/* Define to 1 if you have the `htonl' function. */
#define HAVE_HTONL 1

/* Define to 1 if you have the `htons' function. */
#define HAVE_HTONS 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `advapi32' library (-ladvapi32). */
/* #undef HAVE_LIBADVAPI32 */

/* Define to 1 if you have the `kernel32' library (-lkernel32). */
/* #undef HAVE_LIBKERNEL32 */

/* Define to 1 if you have the `ws2_32' library (-lws2_32). */
/* #undef HAVE_LIBWS2_32 */

/* Define to 1 if you have the `localtime_r' function. */
#define HAVE_LOCALTIME_R 1

/* Define to 1 if you have the `lstat' function. */
#define HAVE_LSTAT 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `ntohl' function. */
#define HAVE_NTOHL 1

/* Define to 1 if you have the `ntohs' function. */
#define HAVE_NTOHS 1

/* Define if you have POSIX threads libraries and header files. */
#define HAVE_PTHREAD 1

/* Define to 1 if you have the `stat' function. */
#define HAVE_STAT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Defined for --enable-debugging builds. */
/* #undef LOG4CPLUS_DEBUGGING */

/* Defined if the compiler understands __declspec(dllimport) or
   __attribute__((visibility("default"))) construct. */
#define LOG4CPLUS_DECLSPEC_EXPORT __attribute__ ((visibility("default")))

/* Defined if the compiler understands __declspec(dllexport) or construct. */
#define LOG4CPLUS_DECLSPEC_IMPORT /**/

/* */
/* #undef LOG4CPLUS_HAVE_CLOCK_GETTIME */

/* */
#define LOG4CPLUS_HAVE_ENAMETOOLONG 1

/* */
#define LOG4CPLUS_HAVE_ERRNO_H 1

/* */
#define LOG4CPLUS_HAVE_FTIME 1

/* */
#define LOG4CPLUS_HAVE_GETADDRINFO 1

/* */
#define LOG4CPLUS_HAVE_GETHOSTBYNAME_R 1

/* */
#define LOG4CPLUS_HAVE_GETPID 1

/* */
#define LOG4CPLUS_HAVE_GETTIMEOFDAY 1

/* */
#define LOG4CPLUS_HAVE_GMTIME_R 1

/* */
#define LOG4CPLUS_HAVE_HTONL 1

/* */
#define LOG4CPLUS_HAVE_HTONS 1

/* */
#define LOG4CPLUS_HAVE_LOCALTIME_R 1

/* */
#define LOG4CPLUS_HAVE_LSTAT 1

/* */
#define LOG4CPLUS_HAVE_NETDB_H 1

/* */
#define LOG4CPLUS_HAVE_NETINET_IN_H 1

/* */
#define LOG4CPLUS_HAVE_NTOHL 1

/* */
#define LOG4CPLUS_HAVE_NTOHS 1

/* */
#define LOG4CPLUS_HAVE_STAT 1

/* */
#define LOG4CPLUS_HAVE_STDARG_H 1

/* */
#define LOG4CPLUS_HAVE_STDIO_H 1

/* */
#define LOG4CPLUS_HAVE_SYSLOG_H 1

/* */
#define LOG4CPLUS_HAVE_SYS_SOCKET_H 1

/* */
#define LOG4CPLUS_HAVE_SYS_STAT_H 1

/* */
#define LOG4CPLUS_HAVE_SYS_TIMEB_H 1

/* */
#define LOG4CPLUS_HAVE_SYS_TIME_H 1

/* */
#define LOG4CPLUS_HAVE_SYS_TYPES_H 1

/* */
#define LOG4CPLUS_HAVE_TIME_H 1

/* */
#define LOG4CPLUS_HAVE_UNISTD_H 1

/* */
#define LOG4CPLUS_HAVE_WCHAR_H 1

/* Define if this is a single-threaded library. */
/* #undef LOG4CPLUS_SINGLE_THREADED */

/* */
/* #undef LOG4CPLUS_USE_PTHREADS */

/* Define for compilers/standard libraries that support more than just the "C"
   locale. */
/* #undef LOG4CPLUS_WORKING_LOCALE */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "log4cplus"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "log4cplus 1.0.4"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "log4cplus"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0.4"

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to int if undefined. */
/* #undef socklen_t */

#endif // LOG4CPLUS_CONFIG_H
