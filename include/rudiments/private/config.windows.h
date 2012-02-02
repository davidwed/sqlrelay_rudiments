/* include/rudiments/private/config.h.  Generated from config.h.in by configure.  */
/* include/rudiments/private/config.h.in.  Generated from configure.in by autoheader.  */

/* Use inline functions or not */
/* #undef ENABLE_RUDIMENTS_INLINES */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <poll.h> header file. */
/* #undef HAVE_POLL_H */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/poll.h> header file. */
/* #undef HAVE_SYS_POLL_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Mingw32 environment */
#define MINGW32 1

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

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Most modern systems support template <> */
#define RUDIMENTS_EXPLICIT_SPECIALIZATION template <>

/* getsockopt takes a void * on most systems */
#define RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE char *

/* Rudiments supports PCRE */
/* #undef RUDIMENTS_HAS_PCRE */

/* Rudiments supports SSL */
/* #undef RUDIMENTS_HAS_SSL */

/* Rudiments supports threads */
#define RUDIMENTS_HAS_THREADS 1

/* Some systems use _timezone instead of timezone */
#define RUDIMENTS_HAS_TIMEZONE 1

/* Some systems define tm_gmtoff in their struct tm */
/* #undef RUDIMENTS_HAS_TM_GMTOFF */

/* Some systems define tm_name in their struct tm */
/* #undef RUDIMENTS_HAS_TM_NAME */

/* Some systems define tm_tzadj in their struct tm */
/* #undef RUDIMENTS_HAS_TM_TZADJ */

/* Some systems define tm_zone in their struct tm */
/* #undef RUDIMENTS_HAS_TM_ZONE */

/* Some systems define __tm_gmtoff in their struct tm */
/* #undef RUDIMENTS_HAS___TM_GMTOFF */

/* Some systems define __tm_zone in their struct tm */
/* #undef RUDIMENTS_HAS___TM_ZONE */

/* Some systems have alarm */
/* #undef RUDIMENTS_HAVE_ALARM */

/* Some systems have arpa/inet.h */
/* #undef RUDIMENTS_HAVE_ARPA_INET_H */

/* Some systems dont have st_blocks in struct stat */
/* #undef RUDIMENTS_HAVE_BLKCNT_T */

/* Some systems dont have st_blksize in struct stat */
/* #undef RUDIMENTS_HAVE_BLKSIZE_T */

/* Some systems have bswap64 */
/* #undef RUDIMENTS_HAVE_BSWAP64 */

/* Some systems have bswap_64 */
/* #undef RUDIMENTS_HAVE_BSWAP_64 */

/* Some systems have byteswap.h */
/* #undef RUDIMENTS_HAVE_BYTESWAP_H */

/* Some systems have chroot */
/* #undef RUDIMENTS_HAVE_CHROOT */

/* Some systems have clearenv */
/* #undef RUDIMENTS_HAVE_CLEARENV */

/* Some systems have clock_nanosleep */
/* #undef RUDIMENTS_HAVE_CLOCK_NANOSLEEP */

/* some systems have closesocket */
#define RUDIMENTS_HAVE_CLOSESOCKET 1

/* Some systems have CMSG_LEN */
/* #undef RUDIMENTS_HAVE_CMSG_LEN */

/* Some systems have CMSG_SPACE */
/* #undef RUDIMENTS_HAVE_CMSG_SPACE */

/* Some systems have _commit */
#define RUDIMENTS_HAVE_COMMIT 1

/* Most systems have ftok(const char *, int) */
/* #undef RUDIMENTS_HAVE_CONST_CHAR_FTOK */

/* Some systems have CreateNamedPipe */
#define RUDIMENTS_HAVE_CREATENAMEDPIPE 1

/* Some systems have CreateSemaphore */
#define RUDIMENTS_HAVE_CREATESEMAPHORE 1

/* Some systems have CreateFileMapping */
#define RUDIMENTS_HAVE_CREATE_FILE_MAPPING 1

/* CreateMutex function exists */
#define RUDIMENTS_HAVE_CREATE_MUTEX 1

/* Some systems have crypt */
/* #undef RUDIMENTS_HAVE_CRYPT */

/* Some systems have crypt.h */
/* #undef RUDIMENTS_HAVE_CRYPT_H */

/* Some systems have crypt_r */
/* #undef RUDIMENTS_HAVE_CRYPT_R */

/* Cygwin style statfs */
/* #undef RUDIMENTS_HAVE_CYGWIN_STATFS */

/* Darwin style statfs */
/* #undef RUDIMENTS_HAVE_DARWIN_STATFS */

/* Some systems have dirent.h */
/* #undef RUDIMENTS_HAVE_DIRENT_H */

/* Some systems have dirfd */
/* #undef RUDIMENTS_HAVE_DIRFD */

/* Some systems have DIR.dd_fd */
/* #undef RUDIMENTS_HAVE_DIR_DD_FD */

/* Some systems have DIR.d_fd */
/* #undef RUDIMENTS_HAVE_DIR_D_FD */

/* Some systems have dlopen */
/* #undef RUDIMENTS_HAVE_DLOPEN */

/* Some systems define environ in unistd.h */
#define RUDIMENTS_HAVE_ENVIRON 1

/* Some systems have fchmod */
/* #undef RUDIMENTS_HAVE_FCHMOD */

/* Some systems have fchown */
/* #undef RUDIMENTS_HAVE_FCHOWN */

/* Some systems have fcntl */
/* #undef RUDIMENTS_HAVE_FCNTL */

/* Some systems have fdatasync */
/* #undef RUDIMENTS_HAVE_FDATASYNC */

/* Some systems have FD_CLOEXEC */
/* #undef RUDIMENTS_HAVE_FD_CLOEXEC */

/* Some systems have fork */
/* #undef RUDIMENTS_HAVE_FORK */

/* Some systems have fpathconf */
/* #undef RUDIMENTS_HAVE_FPATHCONF */

/* FreeBSD style statfs */
/* #undef RUDIMENTS_HAVE_FREEBSD_STATFS */

/* Some systems have fsync */
/* #undef RUDIMENTS_HAVE_FSYNC */

/* Most systems have ftok() */
/* #undef RUDIMENTS_HAVE_FTOK */

/* Some systems have getaddrinfo */
/* #undef RUDIMENTS_HAVE_GETADDRINFO */

/* Some systems have getegid */
/* #undef RUDIMENTS_HAVE_GETEGID */

/* Some systems have geteuid */
/* #undef RUDIMENTS_HAVE_GETEUID */

/* Some systems have getgid */
/* #undef RUDIMENTS_HAVE_GETGID */

/* Some systems have getgrgid_r */
/* #undef RUDIMENTS_HAVE_GETGRGID_R */

/* Some systems have getgrgid_r */
/* #undef RUDIMENTS_HAVE_GETGRGID_R_4 */

/* Some systems have getgrgid_r */
/* #undef RUDIMENTS_HAVE_GETGRGID_R_5 */

/* Some systems have getgrnam_r */
/* #undef RUDIMENTS_HAVE_GETGRNAM_R */

/* Some systems have getgrnam_r */
/* #undef RUDIMENTS_HAVE_GETGRNAM_R_4 */

/* Some systems have getgrnam_r */
/* #undef RUDIMENTS_HAVE_GETGRNAM_R_5 */

/* Some systems have gethostbyaddr_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYADDR_R */

/* Some systems have gethostbyaddr_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYADDR_R_7 */

/* Some systems have gethostbyaddr_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYADDR_R_8 */

/* Some systems have gethostbyname_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYNAME_R */

/* Some systems have gethostbyname_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYNAME_R_5 */

/* Some systems have gethostbyname_r */
/* #undef RUDIMENTS_HAVE_GETHOSTBYNAME_R_6 */

/* Some systems have getpagesize */
/* #undef RUDIMENTS_HAVE_GETPAGESIZE */

/* Some systems have getpgid */
/* #undef RUDIMENTS_HAVE_GETPGID */

/* Some systems have getpid */
#define RUDIMENTS_HAVE_GETPID 1

/* Some systems have getppid */
/* #undef RUDIMENTS_HAVE_GETPPID */

/* Some systems have getprotobyname_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNAME_R */

/* Some systems have getprotobyname_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNAME_R_4 */

/* Some systems have getprotobyname_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNAME_R_5 */

/* Some systems have getprotobynumber_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNUMBER_R */

/* Some systems have getprotobynumber_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_4 */

/* Some systems have getprotobynumber_r */
/* #undef RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_5 */

/* Some systems have getpwnam_r */
/* #undef RUDIMENTS_HAVE_GETPWNAM_R */

/* Some systems have getpwnam_r */
/* #undef RUDIMENTS_HAVE_GETPWNAM_R_4 */

/* Some systems have getpwnam_r */
/* #undef RUDIMENTS_HAVE_GETPWNAM_R_5 */

/* Some systems have getpwuid_r */
/* #undef RUDIMENTS_HAVE_GETPWUID_R */

/* Some systems have getpwuid_r */
/* #undef RUDIMENTS_HAVE_GETPWUID_R_4 */

/* Some systems have getpwuid_r */
/* #undef RUDIMENTS_HAVE_GETPWUID_R_5 */

/* Some systems have getrpcbyname */
/* #undef RUDIMENTS_HAVE_GETRPCBYNAME */

/* Some systems have getrpcbyname_r */
/* #undef RUDIMENTS_HAVE_GETRPCBYNAME_R */

/* Some systems have getrpcbyname_r */
/* #undef RUDIMENTS_HAVE_GETRPCBYNAME_R_4 */

/* Some systems have getrpcbyname_r */
/* #undef RUDIMENTS_HAVE_GETRPCBYNAME_R_5 */

/* Some systems have getrpcbynumber */
/* #undef RUDIMENTS_HAVE_GETRPCBYNUMBER */

/* Some systems have getrpcbynumber_r */
/* #undef RUDIMENTS_HAVE_GETRPCBYNUMBER_R */

/* Some systems have getrpcbynumber_r */
/* #undef RUDIMENTS_HAVE_GETRPCBYNUMBER_R_4 */

/* Some systems have getrpcbynumber_r */
/* #undef RUDIMENTS_HAVE_GETRPCBYNUMBER_R_5 */

/* Some systems have getservbyname_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYNAME_R */

/* Some systems have getservbyname_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYNAME_R_5 */

/* Some systems have getservbyname_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYNAME_R_6 */

/* Some systems have getservbyport_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYPORT_R */

/* Some systems have getservbyport_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYPORT_R_5 */

/* Some systems have getservbyport_r */
/* #undef RUDIMENTS_HAVE_GETSERVBYPORT_R_6 */

/* Some systems have getsid */
/* #undef RUDIMENTS_HAVE_GETSID */

/* Some systems have getspnam */
/* #undef RUDIMENTS_HAVE_GETSPNAM */

/* Some systems have getspnam_r */
/* #undef RUDIMENTS_HAVE_GETSPNAM_R */

/* Some systems have getspnam_r */
/* #undef RUDIMENTS_HAVE_GETSPNAM_R_4 */

/* Some systems have getspnam_r */
/* #undef RUDIMENTS_HAVE_GETSPNAM_R_5 */

/* Some systems have GetSystemInfo */
#define RUDIMENTS_HAVE_GETSYSTEMINFO 1

/* Some systems have GetSystemTime */
#define RUDIMENTS_HAVE_GETSYSTEMTIME 1

/* Some systems have getuid */
/* #undef RUDIMENTS_HAVE_GETUID */

/* Some systems have HANDLE_FLAG_INHERIT */
#define RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT 1

/* Some systems have inet_addr */
#define RUDIMENTS_HAVE_INET_ADDR 1

/* Some systems have inet_aton */
/* #undef RUDIMENTS_HAVE_INET_ATON */

/* Some systems have int16_t */
#define RUDIMENTS_HAVE_INT16_T 1

/* Some systems have int32_t */
#define RUDIMENTS_HAVE_INT32_T 1

/* Some systems have int64_t */
#define RUDIMENTS_HAVE_INT64_T 1

/* Some systems have int8_t */
#define RUDIMENTS_HAVE_INT8_T 1

/* Some systems have inttypes.h */
#define RUDIMENTS_HAVE_INTTYPES_H 1

/* Some systems have in_addr_t type */
/* #undef RUDIMENTS_HAVE_IN_ADDR_T */

/* Some systems have ioctl */
/* #undef RUDIMENTS_HAVE_IOCTL */

/* Some systems have io.h */
#define RUDIMENTS_HAVE_IO_H 1

/* Some systems have kill */
/* #undef RUDIMENTS_HAVE_KILL */

/* Some systems have link */
/* #undef RUDIMENTS_HAVE_LINK */

/* Linux style statfs */
/* #undef RUDIMENTS_HAVE_LINUX_STATFS */

/* Some systems have LoadLibrary */
#define RUDIMENTS_HAVE_LOADLIBRARY 1

/* Some systems have LoadLibraryEx */
#define RUDIMENTS_HAVE_LOADLIBRARYEX 1

/* Some systems have localtime_r */
/* #undef RUDIMENTS_HAVE_LOCALTIME_R */

/* Some systems have LockFileEx */
#define RUDIMENTS_HAVE_LOCKFILEEX 1

/* Some systems have lrand48 */
/* #undef RUDIMENTS_HAVE_LRAND48 */

/* Some systems have machine/endian.h */
/* #undef RUDIMENTS_HAVE_MACHINE_ENDIAN_H */

/* Some systems have madvise */
/* #undef RUDIMENTS_HAVE_MADVISE */

/* Some systems use caddr_t argument for madvise */
#define RUDIMENTS_HAVE_MADVISE_CADDR_T 1

/* Some systems have memccpy with char argument */
#define RUDIMENTS_HAVE_MEMCCPY_CHAR 1

/* Some systems have memmem */
/* #undef RUDIMENTS_HAVE_MEMMEM */

/* Some systems have memrchr */
/* #undef RUDIMENTS_HAVE_MEMRCHR */

/* Some systems have mincore */
/* #undef RUDIMENTS_HAVE_MINCORE */

/* Some systems use caddr_t argument for mincore */
#define RUDIMENTS_HAVE_MINCORE_CADDR_T 1

/* Some systems have mincore with char argument */
/* #undef RUDIMENTS_HAVE_MINCORE_CHAR */

/* Some systems have mincore with unsigned char argument */
/* #undef RUDIMENTS_HAVE_MINCORE_UCHAR */

/* statvfs */
/* #undef RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS */

/* Some systems have mkdir */
#define RUDIMENTS_HAVE_MKDIR_1 1

/* Some systems have mkdir */
/* #undef RUDIMENTS_HAVE_MKDIR_2 */

/* Some systems have mkfifo */
/* #undef RUDIMENTS_HAVE_MKFIFO */

/* Some systems have mknod */
/* #undef RUDIMENTS_HAVE_MKNOD */

/* Some systems have mkstemp */
/* #undef RUDIMENTS_HAVE_MKSTEMP */

/* Some systems have mktime */
#define RUDIMENTS_HAVE_MKTIME 1

/* Some systems have mlock */
/* #undef RUDIMENTS_HAVE_MLOCK */

/* Some systems have mlockall */
/* #undef RUDIMENTS_HAVE_MLOCKALL */

/* Some systems use caddr_t argument for mlock */
#define RUDIMENTS_HAVE_MLOCK_CADDR_T 1

/* Some systems have mmap */
/* #undef RUDIMENTS_HAVE_MMAP */

/* Some systems have mprotect */
/* #undef RUDIMENTS_HAVE_MPROTECT */

/* Some systems use caddr_t argument for mprotect */
#define RUDIMENTS_HAVE_MPROTECT_CADDR_T 1

/* Some msghdr structs have a msg_controllen member */
/* #undef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN */

/* Some msghdr structs have a msg_flags member */
/* #undef RUDIMENTS_HAVE_MSGHDR_MSG_FLAGS */

/* Some systems have msync */
/* #undef RUDIMENTS_HAVE_MSYNC */

/* Some systems use caddr_t argument for msync */
#define RUDIMENTS_HAVE_MSYNC_CADDR_T 1

/* Some systems have munlock */
/* #undef RUDIMENTS_HAVE_MUNLOCK */

/* Some systems have munlockall */
/* #undef RUDIMENTS_HAVE_MUNLOCKALL */

/* Some systems use caddr_t argument for munlock */
#define RUDIMENTS_HAVE_MUNLOCK_CADDR_T 1

/* Some systems use caddr_t argument for munmap */
#define RUDIMENTS_HAVE_MUNMAP_CADDR_T 1

/* Some systems have nanosleep */
/* #undef RUDIMENTS_HAVE_NANOSLEEP */

/* NetBSD style statfs */
/* #undef RUDIMENTS_HAVE_NETBSD_STATFS */

/* NetBSD-3.0 style statvfs */
/* #undef RUDIMENTS_HAVE_NETBSD_STATVFS */

/* Some systems have netdb.h */
/* #undef RUDIMENTS_HAVE_NETDB_H */

/* Some systems have NetGroupGetInfo */
#define RUDIMENTS_HAVE_NETGROUPGETINFO 1

/* Some systems have netinet/in.h */
/* #undef RUDIMENTS_HAVE_NETINET_IN_H */

/* Some systems have netinet/tcp.h */
/* #undef RUDIMENTS_HAVE_NETINET_TCP_H */

/* Some systems have NetUserGetInfo */
#define RUDIMENTS_HAVE_NETUSERGETINFO 1

/* Some systems define _NSGetEnviron in crt_externs.h */
/* #undef RUDIMENTS_HAVE_NSGETENVIRON */

/* OpenBSD style statfs */
/* #undef RUDIMENTS_HAVE_OPENBSD_STATFS */

/* Some systems have OSSwapHostToLittleInt64 */
/* #undef RUDIMENTS_HAVE_OSSWAPHOSTTOLITTLEINT64 */

/* Some systems have OSSwapLittleToHostInt64 */
/* #undef RUDIMENTS_HAVE_OSSWAPLITTLETOHOSTINT64 */

/* Some systems have os/kernel/OS.h */
/* #undef RUDIMENTS_HAVE_OS_KERNEL_OS_H */

/* Some systems have os/support/ByteOrder.h */
/* #undef RUDIMENTS_HAVE_OS_SUPPORT_BYTEORDER_H */

/* Some systems have pathconf */
/* #undef RUDIMENTS_HAVE_PATHCONF */

/* Some systems have poll */
/* #undef RUDIMENTS_HAVE_POLL */

/* Some systems have posix_fadvise */
/* #undef RUDIMENTS_HAVE_POSIX_FADVISE */

/* Some systems have posix_fallocate */
/* #undef RUDIMENTS_HAVE_POSIX_FALLOCATE */

/* Some systems have Process32First */
#define RUDIMENTS_HAVE_PROCESS32FIRST 1

/* Some systems have pselect */
/* #undef RUDIMENTS_HAVE_PSELECT */

/* pthread_mutex_t type exists */
#define RUDIMENTS_HAVE_PTHREAD_MUTEX_T 1

/* pthread_sigmask exists */
/* #undef RUDIMENTS_HAVE_PTHREAD_SIGMASK */

/* Some systems have putenv */
#define RUDIMENTS_HAVE_PUTENV 1

/* Some systems have rand */
#define RUDIMENTS_HAVE_RAND 1

/* Some systems have rand_r */
/* #undef RUDIMENTS_HAVE_RAND_R */

/* Some systems have readdir_r */
/* #undef RUDIMENTS_HAVE_READDIR_R */

/* Some systems have readlink */
/* #undef RUDIMENTS_HAVE_READLINK */

/* recv takes a void * on most systems */
/* #undef RUDIMENTS_HAVE_RECV_WITH_VOID */

/* some systems have regcomp */
/* #undef RUDIMENTS_HAVE_REGCOMP */

/* Some systems have rpc/rpcent.h */
/* #undef RUDIMENTS_HAVE_RPCENT_H */

/* Some systems have rpc/rpc.h */
/* #undef RUDIMENTS_HAVE_RPC_H */

/* Do we have a real-time-clock */
/* #undef RUDIMENTS_HAVE_RTC */

/* Some systems have semget */
/* #undef RUDIMENTS_HAVE_SEMGET */

/* Some systems have semtimedop */
/* #undef RUDIMENTS_HAVE_SEMTIMEDOP */

/* Some systems define union semun */
/* #undef RUDIMENTS_HAVE_SEMUN */

/* send takes a const void * on most systems */
/* #undef RUDIMENTS_HAVE_SEND_WITH_VOID */

/* Some systems have setegid */
/* #undef RUDIMENTS_HAVE_SETEGID */

/* Some systems have setegid but dont define it in a header file */
/* #undef RUDIMENTS_HAVE_SETEGID_BUT_NOT_DEFINED */

/* Some systems have setenv */
/* #undef RUDIMENTS_HAVE_SETENV */

/* Some systems have seteuid */
/* #undef RUDIMENTS_HAVE_SETEUID */

/* Some systems have seteuid but dont define it in a header file */
/* #undef RUDIMENTS_HAVE_SETEUID_BUT_NOT_DEFINED */

/* Some systems have SetFileTime */
#define RUDIMENTS_HAVE_SETFILETIME 1

/* Some systems have setgid */
/* #undef RUDIMENTS_HAVE_SETGID */

/* Some systems have setitimer */
/* #undef RUDIMENTS_HAVE_SETITIMER */

/* Some systems have setpgid */
/* #undef RUDIMENTS_HAVE_SETPGID */

/* Some systems have setregid */
/* #undef RUDIMENTS_HAVE_SETREGID */

/* Some systems have setreuid */
/* #undef RUDIMENTS_HAVE_SETREUID */

/* Some systems have setsid */
/* #undef RUDIMENTS_HAVE_SETSID */

/* Some systems have SetSystemTime */
#define RUDIMENTS_HAVE_SETSYSTEMTIME 1

/* Some systems have settimeofday */
/* #undef RUDIMENTS_HAVE_SETTIMEOFDAY */

/* Some systems have setuid */
/* #undef RUDIMENTS_HAVE_SETUID */

/* Some systems have set_real_time_clock */
/* #undef RUDIMENTS_HAVE_SET_REAL_TIME_CLOCK */

/* Some systems have shmget */
/* #undef RUDIMENTS_HAVE_SHMGET */

/* Some systems have shm_open */
/* #undef RUDIMENTS_HAVE_SHM_OPEN */

/* Some systems have sigaction */
/* #undef RUDIMENTS_HAVE_SIGACTION */

/* Some systems have signal */
#define RUDIMENTS_HAVE_SIGNAL 1

/* Some systems have sigpending */
/* #undef RUDIMENTS_HAVE_SIGPENDING */

/* Some systems have sigsuspend */
/* #undef RUDIMENTS_HAVE_SIGSUSPEND */

/* Some systems have snprintf */
/* #undef RUDIMENTS_HAVE_SNPRINTF */

/* Some systems have sockaddr_un */
/* #undef RUDIMENTS_HAVE_SOCKADDR_UN */

/* some type of statfs */
/* #undef RUDIMENTS_HAVE_SOME_KIND_OF_STATFS */

/* some type of statvfs */
/* #undef RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS */

/* struct spwd has sp_expire */
/* #undef RUDIMENTS_HAVE_SP_EXPIRE */

/* struct spwd has sp_flag */
/* #undef RUDIMENTS_HAVE_SP_FLAG */

/* struct spwd has sp_inact */
/* #undef RUDIMENTS_HAVE_SP_INACT */

/* struct spwd has sp_warn */
/* #undef RUDIMENTS_HAVE_SP_WARN */

/* Some systems have srand */
#define RUDIMENTS_HAVE_SRAND 1

/* Some systems have srand48 */
/* #undef RUDIMENTS_HAVE_SRAND48 */

/* statvfs */
/* #undef RUDIMENTS_HAVE_STATVFS */

/* Some systems have stdint.h */
#define RUDIMENTS_HAVE_STDINT_H 1

/* Some systems have strchrnul */
/* #undef RUDIMENTS_HAVE_STRCHRNUL */

/* Some systems have strcspn */
#define RUDIMENTS_HAVE_STRCSPN 1

/* Some systems have strings.h */
/* #undef RUDIMENTS_HAVE_STRINGS_H */

/* Some systems have strpbrk */
#define RUDIMENTS_HAVE_STRPBRK 1

/* Some systems have strspn */
#define RUDIMENTS_HAVE_STRSPN 1

/* Some systems have strtold */
#define RUDIMENTS_HAVE_STRTOLD 1

/* Some systems have strtoll */
#define RUDIMENTS_HAVE_STRTOLL 1

/* Some systems have strtoull */
#define RUDIMENTS_HAVE_STRTOULL 1

/* Some systems have struct timespec in pthread.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD */

/* Some systems have struct timespec in sys/siginfo.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO */

/* Some systems have struct timespec in sys/time.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SYS_TIME_H */

/* Some systems have struct timespec in time.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_TIME_H */

/* Some systems have swab with a char * argument */
#define RUDIMENTS_HAVE_SWAB_CHAR 1

/* Some systems have swap64 */
/* #undef RUDIMENTS_HAVE_SWAP64 */

/* Some systems have __swap_int64 */
/* #undef RUDIMENTS_HAVE_SWAP_INT64 */

/* Some systems have symlink */
/* #undef RUDIMENTS_HAVE_SYMLINK */

/* Some systems have syslog.h */
/* #undef RUDIMENTS_HAVE_SYSLOG_H */

/* Some systems have sys/bitypes.h */
/* #undef RUDIMENTS_HAVE_SYS_BITYPES_H */

/* Some systems have sys/fcntl.h */
/* #undef RUDIMENTS_HAVE_SYS_FCNTL_H */

/* Some systems have sys/ioctl.h */
/* #undef RUDIMENTS_HAVE_SYS_IOCTL_H */

/* Some systems have sys/ipc.h */
/* #undef RUDIMENTS_HAVE_SYS_IPC_H */

/* Some systems have sys/param.h */
/* #undef RUDIMENTS_HAVE_SYS_PARAM_H */

/* Some systems have sys/select.h */
/* #undef RUDIMENTS_HAVE_SYS_SELECT_H */

/* Some systems have sys/sem.h */
/* #undef RUDIMENTS_HAVE_SYS_SEM_H */

/* Some systems have sys/socket.h */
/* #undef RUDIMENTS_HAVE_SYS_SOCKET_H */

/* Some systems have sys/time.h */
/* #undef RUDIMENTS_HAVE_SYS_TIME_H */

/* Some systems have sys/uio.h */
/* #undef RUDIMENTS_HAVE_SYS_UIO_H */

/* Some systems have sys/un.h */
/* #undef RUDIMENTS_HAVE_SYS_UN_H */

/* Some systems dont have S_ISLNK */
/* #undef RUDIMENTS_HAVE_S_ISLNK */

/* Some systems dont have S_ISSOCK */
/* #undef RUDIMENTS_HAVE_S_ISSOCK */

/* Some systems have tcsetattr */
/* #undef RUDIMENTS_HAVE_TCSETATTR */

/* Some systems have termios.h */
/* #undef RUDIMENTS_HAVE_TERMIOS_H */

/* Some systems have tlhelp32.h */
#define RUDIMENTS_HAVE_TLHELP32_H 1

/* Some systems have truncate */
/* #undef RUDIMENTS_HAVE_TRUNCATE */

/* Some systems have uint16_t */
#define RUDIMENTS_HAVE_UINT16_T 1

/* Some systems have uint32_t */
#define RUDIMENTS_HAVE_UINT32_T 1

/* Some systems have uint64_t */
#define RUDIMENTS_HAVE_UINT64_T 1

/* Some systems have uint8_t */
#define RUDIMENTS_HAVE_UINT8_T 1

/* Ultrix style statfs */
/* #undef RUDIMENTS_HAVE_ULTRIX_STATFS */

/* Some systems have unistd.h */
/* #undef RUDIMENTS_HAVE_UNISTD_H */

/* Some systems have unsetenv */
/* #undef RUDIMENTS_HAVE_UNSETENV */

/* Some systems have utime */
/* #undef RUDIMENTS_HAVE_UTIME */

/* Some systems have utimes */
/* #undef RUDIMENTS_HAVE_UTIMES */

/* Some systems have utime.h */
#define RUDIMENTS_HAVE_UTIME_H 1

/* GetDiskFreeSpace */
#define RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE 1

/* Some systems have windows.h */
#define RUDIMENTS_HAVE_WINDOWS_H 1

/* Some systems have Sleep */
#define RUDIMENTS_HAVE_WINDOWS_SLEEP 1

/* Some systems have winsock2.h */
#define RUDIMENTS_HAVE_WINSOCK2_H 1

/* Some systems have extended filesystem attributes */
/* #undef RUDIMENTS_HAVE_XATTRS */

/* Some systems have _getpid */
/* #undef RUDIMENTS_HAVE__GETPID */

/* Some systems have _snprintf */
#define RUDIMENTS_HAVE__SNPRINTF 1

/* Some systems have __bswap64 */
/* #undef RUDIMENTS_HAVE___BSWAP64 */

/* iov_base is char * */
#define RUDIMENTS_IOV_BASE_TYPE char *

/* Some systems have swab but dont define it */
/* #undef RUDIMENTS_MUST_DEFINE_SWAB */

/* Compiler supports namespaces */
#define RUDIMENTS_NAMESPACE 1

/* Solaris 2.6 has a few missing function prototypes */
/* #undef RUDIMENTS_NEED_XNET_PROTOTYPES */

/* setsockopt takes a const void * on most systems */
#define RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE const char *

/* Most systems define signal handlers with an integer parameter */
#define RUDIMENTS_SIGNAL_HANDLER_INT 1

/* Use small rather than fast code */
/* #undef RUDIMENTS_SMALL_CODE */

/* SSL_read/write can use a void * parameter instead of char * */
/* #undef RUDIMENTS_SSL_VOID_PTR */

/* Version */
#define RUDIMENTS_VERSION "0.36"

/* Define to 1 if you have the ANSI C header files. */
/* #undef STDC_HEADERS */

/* Hack for gcc on SCO OSR6 */
/* #undef _CFRONT_3_0 */

/* Some systems dont have bool */
/* #undef bool */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define. */
#define gid_t int

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Some systems dont have key_t */
#define key_t int

/* Define to `int' if <sys/types.h> does not define. */
#define mode_t int

/* Some systems dont have nlink_t */
#define nlink_t short

/* Some systems dont have off64_t */
#define off64_t int64_t

/* Define to `int' if <sys/types.h> does not define. */
#define pid_t int

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Some systems dont define socklen_t */
#define socklen_t int

/* Define to `int' if <sys/types.h> does not define. */
#define ssize_t int

/* Define to `int' if <sys/types.h> doesn't define. */
#define uid_t int
