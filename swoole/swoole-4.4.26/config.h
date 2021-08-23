/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Whether to build swoole as dynamic module */
#define COMPILE_DL_SWOOLE 1

/* have accept4 */
#define HAVE_ACCEPT4 1

/* Whether you have AI_ALL */
#define HAVE_AI_ALL 1

/* Whether you have AI_IDN */
#define HAVE_AI_IDN 1

/* Whether you have AI_V4MAPPED */
#define HAVE_AI_V4MAPPED 1

/* have clock_gettime */
#define HAVE_CLOCK_GETTIME 1

/* Whether you have struct cmsghdr */
#define HAVE_CMSGHDR 1

/* cpu affinity? */
#define HAVE_CPU_AFFINITY 1

/* have daemon */
#define HAVE_DAEMON 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* have epoll */
#define HAVE_EPOLL 1

/* have eventfd */
#define HAVE_EVENTFD 1

/* have execinfo */
#define HAVE_EXECINFO 1

/* have FUTEX? */
#define HAVE_FUTEX 1

/* have gethostbyname2_r */
#define HAVE_GETHOSTBYNAME2_R 1

/* Define to 1 if you have the `hstrerror' function. */
#define HAVE_HSTRERROR 1

/* Define to 1 if you have the `if_indextoname' function. */
#define HAVE_IF_INDEXTONAME 1

/* Define to 1 if you have the `if_nametoindex' function. */
#define HAVE_IF_NAMETOINDEX 1

/* have inotify */
#define HAVE_INOTIFY 1

/* have inotify_init1 */
#define HAVE_INOTIFY_INIT1 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* have kqueue */
/* #undef HAVE_KQUEUE */

/* have malloc_trim */
#define HAVE_MALLOC_TRIM 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* have mkostemp */
#define HAVE_MKOSTEMP 1

/* have pthread_mutex_timedlock */
#define HAVE_MUTEX_TIMEDLOCK 1

/* Define to 1 if you have the <netdb.h> header file. */
#define HAVE_NETDB_H 1

/* Define to 1 if you have the <netinet/tcp.h> header file. */
#define HAVE_NETINET_TCP_H 1

/* have openssl */
/* #undef HAVE_OPENSSL */

/* have pcre */
#define HAVE_PCRE 1

/* have poll */
#define HAVE_POLL 1

/* have pthread_barrier_init */
#define HAVE_PTHREAD_BARRIER 1

/* have ptrace */
#define HAVE_PTRACE 1

/* have SO_REUSEPORT? */
#define HAVE_REUSEPORT 1

/* have pthread_rwlock_init */
#define HAVE_RWLOCK 1

/* Whether you have sockaddr_storage.ss_family */
#define HAVE_SA_SS_FAMILY 1

/* have sendfile */
#define HAVE_SENDFILE 1

/* have signalfd */
#define HAVE_SIGNALFD 1

/* Define to 1 if you have the `socketpair' function. */
#define HAVE_SOCKETPAIR 1

/* */
#define HAVE_SOCKETS 1

/* have pthread_spin_lock */
#define HAVE_SPINLOCK 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/sockio.h> header file. */
/* #undef HAVE_SYS_SOCKIO_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <sys/un.h> header file. */
#define HAVE_SYS_UN_H 1

/* have ucontext? */
#define HAVE_UCONTEXT 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* have valgrind? */
/* #undef HAVE_VALGRIND */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* do we enable swoole debug */
/* #undef SW_DEBUG */

/* have brotli encoder */
/* #undef SW_HAVE_BROTLI */

/* have compression */
#define SW_HAVE_COMPRESSION 1

/* have zlib */
#define SW_HAVE_ZLIB 1

/* enable trace log */
/* #undef SW_LOG_TRACE_OPEN */

/* enable sockets support */
/* #undef SW_SOCKETS */

/* use boost asm context */
#define SW_USE_ASM_CONTEXT 1

/* enable HTTP2 support */
/* #undef SW_USE_HTTP2 */

/* use jemalloc */
/* #undef SW_USE_JEMALLOC */

/* use mysqlnd */
/* #undef SW_USE_MYSQLND */

/* enable openssl support */
/* #undef SW_USE_OPENSSL */

/* enable thread support */
/* #undef SW_USE_THREAD */
