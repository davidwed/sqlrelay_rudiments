/* include/rudiments/private/config.h.in.  Generated from configure.in by autoheader.  */

/* Use inline functions or not */
#undef ENABLE_RUDIMENTS_INLINES

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

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

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Mingw32 environment */
#undef MINGW32

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* ioctl/FIONBIO doesn't work on some systems */
#undef RUDIMENTS_DISABLE_FIONBIO

/* Enable runtime notification of methods that set ENOSYS */
#undef RUDIMENTS_ENOSYS_NOTIFY

/* getsockopt has a void * on most systems */
#define RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE void *

/* libcurl has CURLINFO_ACTIVESOCKET */
#undef RUDIMENTS_HAS_CURLINFO_ACTIVESOCKET

/* libcurl has CURLOPT_SSH_AUTH_TYPES */
#undef RUDIMENTS_HAS_CURLOPT_SSH_AUTH_TYPES

/* libcurl has CURLOPT_USERNAME */
#undef RUDIMENTS_HAS_CURLOPT_USERNAME

/* libcurl has CURLOPT_USERPWD */
#undef RUDIMENTS_HAS_CURLOPT_USERPWD

/* krb5 has gss_str_to_oid */
#undef RUDIMENTS_HAS_GSS_STR_TO_OID

/* Rudiments supports krb5/gssapi */
#undef RUDIMENTS_HAS_KRB5

/* Rudiments supports libcurl */
#undef RUDIMENTS_HAS_LIBCURL

/* Rudiments supports PCRE */
#undef RUDIMENTS_HAS_PCRE

/* Rudiments supports SSL */
#undef RUDIMENTS_HAS_SSL

/* Rudiments supports threads */
#undef RUDIMENTS_HAS_THREADS

/* Some systems have timezone */
#define RUDIMENTS_HAS_TIMEZONE 1

/* Some systems define tm_gmtoff in their struct tm */
#undef RUDIMENTS_HAS_TM_GMTOFF

/* Some systems define tm_name in their struct tm */
#undef RUDIMENTS_HAS_TM_NAME

/* Some systems define tm_tzadj in their struct tm */
#undef RUDIMENTS_HAS_TM_TZADJ

/* Some systems define tm_zone in their struct tm */
#undef RUDIMENTS_HAS_TM_ZONE

/* Some systems have tzname */
#define RUDIMENTS_HAS_TZNAME 1

/* Some systems have tzset */
#define RUDIMENTS_HAS_TZSET 1

/* Some systems have _get_timezone */
#undef RUDIMENTS_HAS__GET_TIMEZONE

/* Some systems have _get_tzname */
#undef RUDIMENTS_HAS__GET_TZNAME

/* Some systems have _timezone */
#undef RUDIMENTS_HAS__TIMEZONE

/* Some systems have _tzname */
#undef RUDIMENTS_HAS__TZNAME

/* Some systems have _tzset */
#undef RUDIMENTS_HAS__TZSET

/* Some systems define __tm_gmtoff in their struct tm */
#undef RUDIMENTS_HAS___TM_GMTOFF

/* Some systems define __tm_zone in their struct tm */
#undef RUDIMENTS_HAS___TM_ZONE

/* Some systems have accctrl.h */
#undef RUDIMENTS_HAVE_ACCCTRL_H

/* Most systems have access() */
#define RUDIMENTS_HAVE_ACCESS 1

/* Some systems have aclapi.h */
#undef RUDIMENTS_HAVE_ACLAPI_H

/* Some systems have alarm */
#define RUDIMENTS_HAVE_ALARM 1

/* Some systems have arc4random */
#undef RUDIMENTS_HAVE_ARC4RANDOM

/* Some systems have arpa/inet.h */
#define RUDIMENTS_HAVE_ARPA_INET_H 1

/* Some systems have atexit */
#define RUDIMENTS_HAVE_ATEXIT 1

/* SCO OSR5 has an incorrect struct msghdr definition */
#undef RUDIMENTS_HAVE_BAD_SCO_MSGHDR

/* Some systems dont have st_blocks in struct stat */
#undef RUDIMENTS_HAVE_BLKCNT_T

/* Some systems dont have st_blksize in struct stat */
#undef RUDIMENTS_HAVE_BLKSIZE_T

/* Most systems have bool */
#define RUDIMENTS_HAVE_BOOL 1

/* Some systems have BRoster::_ShutDown */
#undef RUDIMENTS_HAVE_BROSTER__SHUTDOWN

/* Some systems have bswap64 */
#undef RUDIMENTS_HAVE_BSWAP64

/* Some systems have bswap_64 */
#undef RUDIMENTS_HAVE_BSWAP_64

/* Some systems have byteswap.h */
#undef RUDIMENTS_HAVE_BYTESWAP_H

/* Some systems have chmod */
#define RUDIMENTS_HAVE_CHMOD 1

/* Some systems have chroot */
#undef RUDIMENTS_HAVE_CHROOT

/* Some systems have clearenv */
#undef RUDIMENTS_HAVE_CLEARENV

/* Some systems have clock_nanosleep */
#undef RUDIMENTS_HAVE_CLOCK_NANOSLEEP

/* Some systems have clock_settime */
#undef RUDIMENTS_HAVE_CLOCK_SETTIME

/* Some systems have close */
#define RUDIMENTS_HAVE_CLOSE 1

/* some systems have closesocket */
#undef RUDIMENTS_HAVE_CLOSESOCKET

/* Some systems have cmsghdr */
#define RUDIMENTS_HAVE_CMSGHDR 1

/* Some systems have _commit */
#undef RUDIMENTS_HAVE_COMMIT

/* connect has a const sockaddr * on most systems */
#define RUDIMENTS_HAVE_CONNECT_WITH_NON_CONST_SOCKADDR 1

/* Most systems have ftok(const char *, int) */
#undef RUDIMENTS_HAVE_CONST_CHAR_FTOK

/* Some systems have CreateDirectory */
#undef RUDIMENTS_HAVE_CREATEDIRECTORY

/* Some systems have CreateFile */
#undef RUDIMENTS_HAVE_CREATEFILE

/* Some systems have CreateNamedPipe */
#undef RUDIMENTS_HAVE_CREATENAMEDPIPE

/* Some systems have CreateSemaphore */
#undef RUDIMENTS_HAVE_CREATESEMAPHORE

/* CreateThread function exists */
#undef RUDIMENTS_HAVE_CREATETHREAD

/* Some systems have CreateTimerQueueTimer */
#undef RUDIMENTS_HAVE_CREATETIMERQUEUETIMER

/* Some systems have CreateFileMapping */
#undef RUDIMENTS_HAVE_CREATE_FILE_MAPPING

/* Some systems have CreateIoCompletionPort */
#undef RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT

/* CreateMutex function exists */
#undef RUDIMENTS_HAVE_CREATE_MUTEX

/* Some systems have CreateProcess */
#undef RUDIMENTS_HAVE_CREATE_PROCESS

/* Some systems have crypt */
#define RUDIMENTS_HAVE_CRYPT 1

/* Some systems have CryptGenRandom */
#undef RUDIMENTS_HAVE_CRYPTGENRANDOM

/* Some systems have crypt.h */
#undef RUDIMENTS_HAVE_CRYPT_H

/* Some systems have crypt_r */
#undef RUDIMENTS_HAVE_CRYPT_R

/* Some systems have ctype.h */
#define RUDIMENTS_HAVE_CTYPE_H 1

/* Cygwin style statfs */
#undef RUDIMENTS_HAVE_CYGWIN_STATFS

/* Darwin style statfs */
#undef RUDIMENTS_HAVE_DARWIN_STATFS

/* Some systems have direct.h */
#undef RUDIMENTS_HAVE_DIRECT_H

/* Some systems have dirent.h */
#define RUDIMENTS_HAVE_DIRENT_H 1

/* Some systems have dirfd */
#undef RUDIMENTS_HAVE_DIRFD

/* Some systems have DIR.dd_fd */
#undef RUDIMENTS_HAVE_DIR_DD_FD

/* Some systems have DIR.d_fd */
#undef RUDIMENTS_HAVE_DIR_D_FD

/* Some systems have dlfcn.h */
#define RUDIMENTS_HAVE_DLFCN_H 1

/* Some systems have dlopen */
#undef RUDIMENTS_HAVE_DLOPEN

/* Some systems have dup */
#define RUDIMENTS_HAVE_DUP 1

/* Some systems have dup2 */
#define RUDIMENTS_HAVE_DUP2 1

/* Some systems have DuplicateHandle */
#undef RUDIMENTS_HAVE_DUPLICATEHANDLE

/* Some systems define environ in unistd.h */
#undef RUDIMENTS_HAVE_ENVIRON

/* Some systems have epoll */
#undef RUDIMENTS_HAVE_EPOLL

/* Some systems have errno.h */
#define RUDIMENTS_HAVE_ERRNO_H 1

/* Some systems have execvp */
#define RUDIMENTS_HAVE_EXECVP 1

/* Some systems have ExitWindowsEx */
#undef RUDIMENTS_HAVE_EXITWINDOWSEX

/* Some systems have fchmod */
#undef RUDIMENTS_HAVE_FCHMOD

/* Some systems have fchown */
#undef RUDIMENTS_HAVE_FCHOWN

/* Some systems have fcntl */
#define RUDIMENTS_HAVE_FCNTL 1

/* Some systems have fcntl.h */
#define RUDIMENTS_HAVE_FCNTL_H 1

/* Some systems have fdatasync */
#undef RUDIMENTS_HAVE_FDATASYNC

/* Some systems have fdopen */
#define RUDIMENTS_HAVE_FDOPEN 1

/* Some systems have FD_CLOEXEC */
#define RUDIMENTS_HAVE_FD_CLOEXEC 1

/* Some systems have FILE->_file */
#define RUDIMENTS_HAVE_FILE_FILE 1

/* Some systems have FILE->__filedes */
/* #undef RUDIMENTS_HAVE_FILE_FILEDES */

/* Some systems have FILE->_fileno */
/* #undef RUDIMENTS_HAVE_FILE_FILENO */

/* Some systems have FILE->_fd */
/* #undef RUDIMENTS_HAVE_FILE__FD */

/* Some systems have FILE->__file */
/* #undef RUDIMENTS_HAVE_FILE__FILE */

/* Some systems have FindFirstFile */
#undef RUDIMENTS_HAVE_FINDFIRSTFILE

/* Some systems have FlushFileBuffers */
#undef RUDIMENTS_HAVE_FLUSHFILEBUFFERS

/* Some systems have fork */
#undef RUDIMENTS_HAVE_FORK

/* Some systems have fpathconf */
#define RUDIMENTS_HAVE_FPATHCONF 1

/* FreeBSD style statfs */
#undef RUDIMENTS_HAVE_FREEBSD_STATFS

/* Some systems have FreeConsole */
#undef RUDIMENTS_HAVE_FREECONSOLE

/* Some systems have fsync */
#define RUDIMENTS_HAVE_FSYNC 1

/* Most systems have ftok() */
#undef RUDIMENTS_HAVE_FTOK

/* Some systems have ftruncate */
#define RUDIMENTS_HAVE_FTRUNCATE 1

/* Some systems have GenerateConsoleCtrlEvent */
#undef RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT

/* Some systems have GetAce */
#undef RUDIMENTS_HAVE_GETACE

/* Some systems have getaddrinfo */
#undef RUDIMENTS_HAVE_GETADDRINFO

/* Some systems have GetCurrentDirectory */
#undef RUDIMENTS_HAVE_GETCURRENTDIRECTORY

/* Some systems have GetCurrentProcessId */
#undef RUDIMENTS_HAVE_GETCURRENTPROCESSID

/* Some systems have getegid */
#define RUDIMENTS_HAVE_GETEGID 1

/* Some systems have getenv */
#define RUDIMENTS_HAVE_GETENV 1

/* Some systems have geteuid */
#define RUDIMENTS_HAVE_GETEUID 1

/* Some systems have GetFileInformationByHandle */
#undef RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE

/* Some systems have GetFileType */
#undef RUDIMENTS_HAVE_GETFILETYPE

/* Some systems have getgid */
#define RUDIMENTS_HAVE_GETGID 1

/* Some systems have getgrgid_r */
#undef RUDIMENTS_HAVE_GETGRGID_R

/* Some systems have getgrgid_r */
#undef RUDIMENTS_HAVE_GETGRGID_R_4

/* Some systems have getgrgid_r */
#undef RUDIMENTS_HAVE_GETGRGID_R_5

/* Some systems have getgrnam_r */
#undef RUDIMENTS_HAVE_GETGRNAM_R

/* Some systems have getgrnam_r */
#undef RUDIMENTS_HAVE_GETGRNAM_R_4

/* Some systems have getgrnam_r */
#undef RUDIMENTS_HAVE_GETGRNAM_R_5

/* Some systems have gethostbyaddr_r */
#define RUDIMENTS_HAVE_GETHOSTBYADDR_R 1

/* Some systems have gethostbyaddr_r */
#undef RUDIMENTS_HAVE_GETHOSTBYADDR_R_7

/* Some systems have gethostbyaddr_r */
#undef RUDIMENTS_HAVE_GETHOSTBYADDR_R_8

/* Some systems have gethostbyname_r */
#define RUDIMENTS_HAVE_GETHOSTBYNAME_R 1

/* Some systems have gethostbyname_r */
#undef RUDIMENTS_HAVE_GETHOSTBYNAME_R_5

/* Some systems have gethostbyname_r */
#undef RUDIMENTS_HAVE_GETHOSTBYNAME_R_6

/* Some systems have gethostname */
#define RUDIMENTS_HAVE_GETHOSTNAME 1

/* Some systems have GetLastError */
#undef RUDIMENTS_HAVE_GETLASTERROR

/* Some systems have getloadavg */
#undef RUDIMENTS_HAVE_GETLOADAVG

/* Some systems have GetLogicalProcessorInformation */
#undef RUDIMENTS_HAVE_GETLOGICALPROCESSORINFORMATION

/* Some systems have getnameinfo */
#define RUDIMENTS_HAVE_GETNAMEINFO 1

/* Some systems have GetNativeSystemInfo */
#undef RUDIMENTS_HAVE_GETNATIVESYSTEMINFO

/* Some systems have getpagesize */
#define RUDIMENTS_HAVE_GETPAGESIZE 1

/* Some systems have getpgid */
#undef RUDIMENTS_HAVE_GETPGID

/* Some systems have getpid */
#define RUDIMENTS_HAVE_GETPID 1

/* Some systems have getppid */
#define RUDIMENTS_HAVE_GETPPID 1

/* Some systems have getprotobyname_r */
#define RUDIMENTS_HAVE_GETPROTOBYNAME_R 1

/* Some systems have getprotobyname_r */
#undef RUDIMENTS_HAVE_GETPROTOBYNAME_R_4

/* Some systems have getprotobyname_r */
#undef RUDIMENTS_HAVE_GETPROTOBYNAME_R_5

/* Some systems have getprotobynumber_r */
#define RUDIMENTS_HAVE_GETPROTOBYNUMBER_R 1

/* Some systems have getprotobynumber_r */
#undef RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_4

/* Some systems have getprotobynumber_r */
#undef RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_5

/* Some systems have getpwnam_r */
#define RUDIMENTS_HAVE_GETPWNAM_R 1

/* Some systems have getpwnam_r */
#undef RUDIMENTS_HAVE_GETPWNAM_R_4

/* Some systems have getpwnam_r */
#define RUDIMENTS_HAVE_GETPWNAM_R_5 1

/* Some systems have getpwuid_r */
#undef RUDIMENTS_HAVE_GETPWUID_R

/* Some systems have getpwuid_r */
#define RUDIMENTS_HAVE_GETPWUID_R_4 1

/* Some systems have getpwuid_r */
#undef RUDIMENTS_HAVE_GETPWUID_R_5

/* Some systems have getrpcbyname */
#undef RUDIMENTS_HAVE_GETRPCBYNAME

/* Some systems have getrpcbyname_r */
#undef RUDIMENTS_HAVE_GETRPCBYNAME_R

/* Some systems have getrpcbyname_r */
#undef RUDIMENTS_HAVE_GETRPCBYNAME_R_4

/* Some systems have getrpcbyname_r */
#undef RUDIMENTS_HAVE_GETRPCBYNAME_R_5

/* Some systems have getrpcbynumber */
#undef RUDIMENTS_HAVE_GETRPCBYNUMBER

/* Some systems have getrpcbynumber_r */
#undef RUDIMENTS_HAVE_GETRPCBYNUMBER_R

/* Some systems have getrpcbynumber_r */
#undef RUDIMENTS_HAVE_GETRPCBYNUMBER_R_4

/* Some systems have getrpcbynumber_r */
#undef RUDIMENTS_HAVE_GETRPCBYNUMBER_R_5

/* Some systems have GetSecurityInfo */
#undef RUDIMENTS_HAVE_GETSECURITYINFO

/* Some systems have getservbyname_r */
#define RUDIMENTS_HAVE_GETSERVBYNAME_R 1

/* Some systems have getservbyname_r */
#undef RUDIMENTS_HAVE_GETSERVBYNAME_R_5

/* Some systems have getservbyname_r */
#undef RUDIMENTS_HAVE_GETSERVBYNAME_R_6

/* Some systems have getservbyport_r */
#define RUDIMENTS_HAVE_GETSERVBYPORT_R 1

/* Some systems have getservbyport_r */
#undef RUDIMENTS_HAVE_GETSERVBYPORT_R_5

/* Some systems have getservbyport_r */
#undef RUDIMENTS_HAVE_GETSERVBYPORT_R_6

/* Some systems have getsid */
#undef RUDIMENTS_HAVE_GETSID

/* Some systems have getspnam */
#undef RUDIMENTS_HAVE_GETSPNAM

/* Some systems have getspnam_r */
#undef RUDIMENTS_HAVE_GETSPNAM_R

/* Some systems have getspnam_r */
#undef RUDIMENTS_HAVE_GETSPNAM_R_4

/* Some systems have getspnam_r */
#undef RUDIMENTS_HAVE_GETSPNAM_R_5

/* Some systems have GetSystemInfo */
#undef RUDIMENTS_HAVE_GETSYSTEMINFO

/* Some systems have GetSystemTimeAsFileTime */
#undef RUDIMENTS_HAVE_GETSYSTEMTIMEASFILETIME

/* Some systems have gettimeofday */
#define RUDIMENTS_HAVE_GETTIMEOFDAY 1

/* Some systems have getuid */
#define RUDIMENTS_HAVE_GETUID 1

/* Some systems have GetUserName */
#undef RUDIMENTS_HAVE_GETUSERNAME

/* Some systems have GetVersionEx */
#undef RUDIMENTS_HAVE_GETVERSIONEX

/* Some systems have GlobalMemoryStatusEx */
#undef RUDIMENTS_HAVE_GLOBALMEMORYSTATUSEX

/* Some systems have grp.h */
#undef RUDIMENTS_HAVE_GRP_H

/* Some systems have HANDLE_FLAG_INHERIT */
#undef RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT

/* Some systems have htonll */
#undef RUDIMENTS_HAVE_HTONLL

/* Some systems have inet_addr */
#define RUDIMENTS_HAVE_INET_ADDR 1

/* Some systems have inet_aton */
#undef RUDIMENTS_HAVE_INET_ATON

/* Some systems have InitiateSystemShutdownEx */
#undef RUDIMENTS_HAVE_INITIATESYSTEMSHUTDOWNEX

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
#define RUDIMENTS_HAVE_IN_ADDR_T 1

/* Some systems have ioctl */
#define RUDIMENTS_HAVE_IOCTL 1

/* Some systems have ioctlsocket */
#undef RUDIMENTS_HAVE_IOCTLSOCKET

/* Some systems have io.h */
#undef RUDIMENTS_HAVE_IO_H

/* Some systems have kill */
#define RUDIMENTS_HAVE_KILL 1

/* Some systems have kqueue */
#undef RUDIMENTS_HAVE_KQUEUE

/* Some systems have void * kqueue user data */
#undef RUDIMENTS_HAVE_KQUEUE_VOID_UDATA

/* Some systems have limits.h */
#define RUDIMENTS_HAVE_LIMITS_H 1

/* Some systems have link */
#undef RUDIMENTS_HAVE_LINK

/* Linux libc4 style statfs */
#undef RUDIMENTS_HAVE_LINUX_LIBC4_STATFS

/* Some systems have linux/reboot.h */
#undef RUDIMENTS_HAVE_LINUX_REBOOT_H

/* Some systems have linux/rtc.h */
#undef RUDIMENTS_HAVE_LINUX_RTC_H

/* Linux style statfs */
#undef RUDIMENTS_HAVE_LINUX_STATFS

/* Some systems have llabs */
#undef RUDIMENTS_HAVE_LLABS

/* Some systems have lm.h */
#undef RUDIMENTS_HAVE_LM_H

/* Some systems have LoadLibrary */
#undef RUDIMENTS_HAVE_LOADLIBRARY

/* Some systems have LoadLibraryEx */
#undef RUDIMENTS_HAVE_LOADLIBRARYEX

/* Some systems have localtime_r */
#define RUDIMENTS_HAVE_LOCALTIME_R 1

/* Some systems have localtime_s */
#undef RUDIMENTS_HAVE_LOCALTIME_S

/* Some systems have LockFileEx */
#undef RUDIMENTS_HAVE_LOCKFILEEX

/* Most systems have long long */
#define RUDIMENTS_HAVE_LONG_LONG 1

/* Some systems have lrand48 */
#define RUDIMENTS_HAVE_LRAND48 1

/* Some systems have lrand48_r */
#undef RUDIMENTS_HAVE_LRAND48_R

/* Some systems have lseek */
#define RUDIMENTS_HAVE_LSEEK 1

/* Some systems have machine/endian.h */
#undef RUDIMENTS_HAVE_MACHINE_ENDIAN_H

/* Some systems have madvise */
#undef RUDIMENTS_HAVE_MADVISE

/* Some systems use caddr_t argument for madvise */
#undef RUDIMENTS_HAVE_MADVISE_CADDR_T

/* Some systems have memccpy with char argument */
#undef RUDIMENTS_HAVE_MEMCCPY_CHAR

/* Some systems have memmem */
#undef RUDIMENTS_HAVE_MEMMEM

/* Some systems have memrchr */
#undef RUDIMENTS_HAVE_MEMRCHR

/* Some systems have mincore */
#undef RUDIMENTS_HAVE_MINCORE

/* Some systems use caddr_t argument for mincore */
#undef RUDIMENTS_HAVE_MINCORE_CADDR_T

/* Some systems have mincore with char argument */
#undef RUDIMENTS_HAVE_MINCORE_CHAR

/* Some systems have mincore with unsigned char argument */
#undef RUDIMENTS_HAVE_MINCORE_UCHAR

/* statvfs */
#undef RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS

/* Some systems have sethostname but forgot to declare it */
#undef RUDIMENTS_HAVE_MISSING_SETHOSTNAME_DECLARATION

/* Some systems have mkdir */
#undef RUDIMENTS_HAVE_MKDIR_1

/* Some systems have mkdir */
#define RUDIMENTS_HAVE_MKDIR_2 1

/* Some systems have mkfifo */
#undef RUDIMENTS_HAVE_MKFIFO

/* Some systems have mknod */
#undef RUDIMENTS_HAVE_MKNOD

/* Some systems have mkstemp */
#define RUDIMENTS_HAVE_MKSTEMP 1

/* Some systems have mlock */
#undef RUDIMENTS_HAVE_MLOCK

/* Some systems have mlockall */
#undef RUDIMENTS_HAVE_MLOCKALL

/* Some systems use caddr_t argument for mlock */
#undef RUDIMENTS_HAVE_MLOCK_CADDR_T

/* Some systems have mmap */
#define RUDIMENTS_HAVE_MMAP 1

/* Some systems have mprotect */
#define RUDIMENTS_HAVE_MPROTECT 1

/* Some systems use caddr_t argument for mprotect */
#undef RUDIMENTS_HAVE_MPROTECT_CADDR_T

/* Some msghdr structs have a msg_accrights member */
#undef RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS

/* Some msghdr structs have a msg_controllen member */
#define RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN 1

/* Some msghdr structs have a msg_flags member */
#define RUDIMENTS_HAVE_MSGHDR_MSG_FLAGS 1

/* Some systems have msync */
#define RUDIMENTS_HAVE_MSYNC 1

/* Some systems use caddr_t argument for msync */
#undef RUDIMENTS_HAVE_MSYNC_CADDR_T

/* Some systems have munlock */
#undef RUDIMENTS_HAVE_MUNLOCK

/* Some systems have munlockall */
#undef RUDIMENTS_HAVE_MUNLOCKALL

/* Some systems use caddr_t argument for munlock */
#undef RUDIMENTS_HAVE_MUNLOCK_CADDR_T

/* Some systems use caddr_t argument for munmap */
#undef RUDIMENTS_HAVE_MUNMAP_CADDR_T

/* Some systems have nanosleep */
#undef RUDIMENTS_HAVE_NANOSLEEP

/* NetBSD style statfs */
#undef RUDIMENTS_HAVE_NETBSD_STATFS

/* NetBSD-3.0 style statvfs */
#undef RUDIMENTS_HAVE_NETBSD_STATVFS

/* Some systems have netdb.h */
#define RUDIMENTS_HAVE_NETDB_H 1

/* Some systems have NetGroupGetInfo */
#undef RUDIMENTS_HAVE_NETGROUPGETINFO

/* Some systems have netinet/in.h */
#define RUDIMENTS_HAVE_NETINET_IN_H 1

/* Some systems have netinet/tcp.h */
#define RUDIMENTS_HAVE_NETINET_TCP_H 1

/* Some systems have NetUserGetInfo */
#undef RUDIMENTS_HAVE_NETUSERGETINFO

/* Some systems have new */
#undef RUDIMENTS_HAVE_NEW

/* Some systems have new.h */
#undef RUDIMENTS_HAVE_NEW_H

/* Some systems define _NSGetEnviron in crt_externs.h */
#undef RUDIMENTS_HAVE_NSGETENVIRON

/* Some systems have ntohll */
#undef RUDIMENTS_HAVE_NTOHLL

/* Some systems have open */
#define RUDIMENTS_HAVE_OPEN 1

/* OpenBSD style statfs */
#undef RUDIMENTS_HAVE_OPENBSD_STATFS

/* Some systems have OSSwapHostToLittleInt64 */
#undef RUDIMENTS_HAVE_OSSWAPHOSTTOLITTLEINT64

/* Some systems have OSSwapLittleToHostInt64 */
#undef RUDIMENTS_HAVE_OSSWAPLITTLETOHOSTINT64

/* Some systems have os/kernel/OS.h */
#undef RUDIMENTS_HAVE_OS_KERNEL_OS_H

/* Some systems have os/support/ByteOrder.h */
#undef RUDIMENTS_HAVE_OS_SUPPORT_BYTEORDER_H

/* Some systems have pathconf */
#define RUDIMENTS_HAVE_PATHCONF 1

/* Some systems have pipe */
#define RUDIMENTS_HAVE_PIPE 1

/* Some systems have poll */
#undef RUDIMENTS_HAVE_POLL

/* Some systems have poll.h */
#define RUDIMENTS_HAVE_POLL_H 1

/* Some systems have port_create */
#undef RUDIMENTS_HAVE_PORT_CREATE

/* Some systems have port.h */
#undef RUDIMENTS_HAVE_PORT_H

/* Some systems have posix_fadvise */
#undef RUDIMENTS_HAVE_POSIX_FADVISE

/* Some systems have posix_fallocate */
#undef RUDIMENTS_HAVE_POSIX_FALLOCATE

/* Some systems have Process32First */
#undef RUDIMENTS_HAVE_PROCESS32FIRST

/* Some systems have process.h */
#undef RUDIMENTS_HAVE_PROCESS_H

/* Some systems have prototypes.h */
#undef RUDIMENTS_HAVE_PROTOTYPES_H

/* Some systems have pselect */
#undef RUDIMENTS_HAVE_PSELECT

/* Some systems have pthread_attr_setstacksize */
#undef RUDIMENTS_HAVE_PTHREAD_ATTR_SETSTACKSIZE

/* pthread_mutex_t type exists */
#undef RUDIMENTS_HAVE_PTHREAD_MUTEX_T

/* pthread_sigmask exists */
#undef RUDIMENTS_HAVE_PTHREAD_SIGMASK

/* pthread_t type exists */
#undef RUDIMENTS_HAVE_PTHREAD_T

/* Some systems have putenv */
#define RUDIMENTS_HAVE_PUTENV 1

/* Some systems have raise */
#define RUDIMENTS_HAVE_RAISE 1

/* Some systems have rand */
#define RUDIMENTS_HAVE_RAND 1

/* Some systems have random */
#define RUDIMENTS_HAVE_RANDOM 1

/* Some systems have random_r */
#undef RUDIMENTS_HAVE_RANDOM_R

/* Some systems have rand_r */
#undef RUDIMENTS_HAVE_RAND_R

/* Some systems have read */
#define RUDIMENTS_HAVE_READ 1

/* Some systems have readdir_r */
#undef RUDIMENTS_HAVE_READDIR_R

/* Some systems have readlink */
#undef RUDIMENTS_HAVE_READLINK

/* Some systems have reboot with 1 argument */
#undef RUDIMENTS_HAVE_REBOOT_1

/* Some systems have reboot with 2 argument */
#undef RUDIMENTS_HAVE_REBOOT_2

/* recv has a void * on most systems */
#undef RUDIMENTS_HAVE_RECV_WITH_VOID

/* some systems have regcomp */
#undef RUDIMENTS_HAVE_REGCOMP

/* Some systems have RemoveDirectory */
#undef RUDIMENTS_HAVE_REMOVEDIRECTORY

/* Some systems have Roster.h */
#undef RUDIMENTS_HAVE_ROSTER_H

/* Some systems have rpc/rpcent.h */
#undef RUDIMENTS_HAVE_RPCENT_H

/* Some systems have rpc/rpc.h */
#undef RUDIMENTS_HAVE_RPC_H

/* Do we have real-time-clock RTC_RD_TIME support */
#undef RUDIMENTS_HAVE_RTC_RD_TIME

/* Do we have real-time-clock RTC_SET_TIME support */
#undef RUDIMENTS_HAVE_RTC_SET_TIME

/* SCO has /dev/table/avenrun instead of getloadavg */
#undef RUDIMENTS_HAVE_SCO_AVENRUN

/* Some systems have sddl.h */
#undef RUDIMENTS_HAVE_SDDL_H

/* Some systems have semget */
#undef RUDIMENTS_HAVE_SEMGET

/* Some systems have semtimedop */
#undef RUDIMENTS_HAVE_SEMTIMEDOP

/* Some systems define union semun */
#undef RUDIMENTS_HAVE_SEMUN

/* send has a const void * on most systems */
#undef RUDIMENTS_HAVE_SEND_WITH_VOID

/* Some systems have SetConsoleCtrlHandler */
#undef RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER

/* Some systems have SetCurrentDirectory */
#undef RUDIMENTS_HAVE_SETCURRENTDIRECTORY

/* Some systems have setegid */
#undef RUDIMENTS_HAVE_SETEGID

/* Some systems have setegid but dont define it in a header file */
#undef RUDIMENTS_HAVE_SETEGID_BUT_NOT_DEFINED

/* Some systems have SetEndOfFile */
#undef RUDIMENTS_HAVE_SETENDOFFILE

/* Some systems have SetEntriesInAcl */
#undef RUDIMENTS_HAVE_SETENTRIESINACL

/* Some systems have setenv */
#define RUDIMENTS_HAVE_SETENV 1

/* Some systems have seteuid */
#undef RUDIMENTS_HAVE_SETEUID

/* Some systems have seteuid but dont define it in a header file */
#undef RUDIMENTS_HAVE_SETEUID_BUT_NOT_DEFINED

/* Some systems have SetFileTime */
#undef RUDIMENTS_HAVE_SETFILETIME

/* Some systems have setgid */
#define RUDIMENTS_HAVE_SETGID 1

/* Some systems have setgroups */
#undef RUDIMENTS_HAVE_SETGROUPS

/* Some systems have sethostname */
#undef RUDIMENTS_HAVE_SETHOSTNAME

/* Some systems have setitimer */
#define RUDIMENTS_HAVE_SETITIMER 1

/* Some systems have setpgid */
#undef RUDIMENTS_HAVE_SETPGID

/* Some systems have setregid */
#undef RUDIMENTS_HAVE_SETREGID

/* Some systems have setreuid */
#undef RUDIMENTS_HAVE_SETREUID

/* Some systems have SetSecurityInfo */
#undef RUDIMENTS_HAVE_SETSECURITYINFO

/* Some systems have setsid */
#undef RUDIMENTS_HAVE_SETSID

/* Some systems have SetSystemTime */
#undef RUDIMENTS_HAVE_SETSYSTEMTIME

/* Some systems have settimeofday */
#undef RUDIMENTS_HAVE_SETTIMEOFDAY

/* Some systems have setuid */
#define RUDIMENTS_HAVE_SETUID 1

/* Some systems have SetUnhandledExceptionFilter */
#undef RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER

/* Some systems have set_real_time_clock */
#undef RUDIMENTS_HAVE_SET_REAL_TIME_CLOCK

/* Some systems have shmget */
#undef RUDIMENTS_HAVE_SHMGET

/* Some systems have sigaction */
#define RUDIMENTS_HAVE_SIGACTION 1

/* Some systems have signal */
#define RUDIMENTS_HAVE_SIGNAL 1

/* Some systems have sigpending */
#define RUDIMENTS_HAVE_SIGPENDING 1

/* Some systems have sigsuspend */
#define RUDIMENTS_HAVE_SIGSUSPEND 1

/* Some systems have sockaddr_un */
#undef RUDIMENTS_HAVE_SOCKADDR_UN

/* some type of statfs */
#undef RUDIMENTS_HAVE_SOME_KIND_OF_STATFS

/* some type of statvfs */
#define RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS 1

/* struct spwd has sp_expire */
#undef RUDIMENTS_HAVE_SP_EXPIRE

/* struct spwd has sp_flag */
#undef RUDIMENTS_HAVE_SP_FLAG

/* struct spwd has sp_inact */
#undef RUDIMENTS_HAVE_SP_INACT

/* struct spwd has sp_warn */
#undef RUDIMENTS_HAVE_SP_WARN

/* statvfs */
#define RUDIMENTS_HAVE_STATVFS 1

/* Some systems have stdarg.h */
#define RUDIMENTS_HAVE_STDARG_H 1

/* Some systems have stdint.h */
#undef RUDIMENTS_HAVE_STDINT_H

/* Some systems have stdlib.h */
#define RUDIMENTS_HAVE_STDLIB_H 1

/* Some systems have strcasecmp */
#define RUDIMENTS_HAVE_STRCASECMP 1

/* Some systems have strchrnul */
#undef RUDIMENTS_HAVE_STRCHRNUL

/* Some systems have strcspn */
#define RUDIMENTS_HAVE_STRCSPN 1

/* Some systems have strerror */
#define RUDIMENTS_HAVE_STRERROR 1

/* Some systems have strerror_r */
#undef RUDIMENTS_HAVE_STRERROR_R

/* Some systems have strerror_s */
#undef RUDIMENTS_HAVE_STRERROR_S

/* Some systems have strings.h */
#define RUDIMENTS_HAVE_STRINGS_H 1

/* Some systems have string.h */
#define RUDIMENTS_HAVE_STRING_H 1

/* Some systems have strncasecmp */
#define RUDIMENTS_HAVE_STRNCASECMP 1

/* Some systems have strpbrk */
#define RUDIMENTS_HAVE_STRPBRK 1

/* Some systems have strspn */
#define RUDIMENTS_HAVE_STRSPN 1

/* Some systems have strtold */
#undef RUDIMENTS_HAVE_STRTOLD

/* Some systems have strtoll */
#define RUDIMENTS_HAVE_STRTOLL 1

/* Some systems have strtoull */
#define RUDIMENTS_HAVE_STRTOULL 1

/* Some systems have struct itimerval */
#define RUDIMENTS_HAVE_STRUCT_ITIMERVAL 1

/* Some systems have struct timespec in pthread.h */
#undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD

/* Some systems have struct timespec in sys/siginfo.h */
#undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO

/* Some systems have struct timespec in sys/time.h */
#undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SYS_TIME_H

/* Some systems have struct timespec in time.h */
#undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_TIME_H

/* Some systems have struct timeval */
#define RUDIMENTS_HAVE_STRUCT_TIMEVAL 1

/* Some systems have swab with a char * argument */
#undef RUDIMENTS_HAVE_SWAB_CHAR

/* Some systems have swab with a const char * argument */
#undef RUDIMENTS_HAVE_SWAB_CONST_CHAR

/* Some systems have swab with a const void * argument */
#define RUDIMENTS_HAVE_SWAB_CONST_VOID 1

/* Some systems have swap64 */
#undef RUDIMENTS_HAVE_SWAP64

/* Some systems have __swap_int64 */
#undef RUDIMENTS_HAVE_SWAP_INT64

/* Some systems have symlink */
#undef RUDIMENTS_HAVE_SYMLINK

/* Some systems have sync */
#define RUDIMENTS_HAVE_SYNC 1

/* Some systems have sysconf */
#define RUDIMENTS_HAVE_SYSCONF 1

/* Some systems have syslog.h */
#undef RUDIMENTS_HAVE_SYSLOG_H

/* Some systems have sys/bitypes.h */
#define RUDIMENTS_HAVE_SYS_BITYPES_H 1

/* Some systems have sys/byteorder.h */
#undef RUDIMENTS_HAVE_SYS_BYTEORDER_H

/* Some systems have sys/devpoll.h */
#undef RUDIMENTS_HAVE_SYS_DEVPOLL_H

/* Some systems have sys/epoll.h */
#undef RUDIMENTS_HAVE_SYS_EPOLL_H

/* Some systems have sys/event.h */
#undef RUDIMENTS_HAVE_SYS_EVENT_H

/* Some systems have sys/fcntl.h */
#define RUDIMENTS_HAVE_SYS_FCNTL_H 1

/* Some systems have sys/ioctl.h */
#define RUDIMENTS_HAVE_SYS_IOCTL_H 1

/* Some systems have sys/ipc.h */
#undef RUDIMENTS_HAVE_SYS_IPC_H

/* Some systems have sys/loadavg.h */
#undef RUDIMENTS_HAVE_SYS_LOADAVG_H

/* Some systems have sys/mman.h */
#define RUDIMENTS_HAVE_SYS_MMAN_H 1

/* Some systems have sys/param.h */
#undef RUDIMENTS_HAVE_SYS_PARAM_H

/* Some systems have sys/poll.h */
#undef RUDIMENTS_HAVE_SYS_POLL_H

/* Some systems have sys/reboot.h */
#undef RUDIMENTS_HAVE_SYS_REBOOT_H

/* Some systems have sys/select.h */
#undef RUDIMENTS_HAVE_SYS_SELECT_H

/* Some systems have sys/sem.h */
#undef RUDIMENTS_HAVE_SYS_SEM_H

/* Some systems have sys/signal.h */
#define RUDIMENTS_HAVE_SYS_SIGNAL_H 1

/* Some systems have sys/socket.h */
#define RUDIMENTS_HAVE_SYS_SOCKET_H 1

/* Some systems have sys/stat.h */
#define RUDIMENTS_HAVE_SYS_STAT_H 1

/* Some systems have sys/time.h */
#define RUDIMENTS_HAVE_SYS_TIME_H 1

/* Some systems have sys/types.h */
#define RUDIMENTS_HAVE_SYS_TYPES_H 1

/* Some systems have sys/uadmin.h */
#undef RUDIMENTS_HAVE_SYS_UADMIN_H

/* Some systems have sys/uio.h */
#define RUDIMENTS_HAVE_SYS_UIO_H 1

/* Some systems have sys/un.h */
#undef RUDIMENTS_HAVE_SYS_UN_H

/* Some systems have sys/utsname.h */
#define RUDIMENTS_HAVE_SYS_UTSNAME_H 1

/* Some systems have sys/wait.h */
#define RUDIMENTS_HAVE_SYS_WAIT_H 1

/* Some systems dont have S_ISBLK */
#define RUDIMENTS_HAVE_S_ISBLK 1

/* Some systems dont have S_ISLNK */
#define RUDIMENTS_HAVE_S_ISLNK 1

/* Some systems dont have S_ISSOCK */
#define RUDIMENTS_HAVE_S_ISSOCK 1

/* Some systems have tcsetattr */
#undef RUDIMENTS_HAVE_TCSETATTR

/* Some systems have termios.h */
#undef RUDIMENTS_HAVE_TERMIOS_H

/* Some systems have time.h */
#define RUDIMENTS_HAVE_TIME_H 1

/* Some systems have tlhelp32.h */
#undef RUDIMENTS_HAVE_TLHELP32_H

/* Most systems have true/false */
#define RUDIMENTS_HAVE_TRUE_FALSE 1

/* Some systems have truncate */
#define RUDIMENTS_HAVE_TRUNCATE 1

/* Some systems have uadmin with a char * argument */
#undef RUDIMENTS_HAVE_UADMIN_CHAR

/* Some systems have uadmin with a integer argument */
#undef RUDIMENTS_HAVE_UADMIN_INT

/* Some systems have uint16_t */
#define RUDIMENTS_HAVE_UINT16_T 1

/* Some systems have uint32_t */
#define RUDIMENTS_HAVE_UINT32_T 1

/* Some systems have uint64_t */
#define RUDIMENTS_HAVE_UINT64_T 1

/* Some systems have uint8_t */
#define RUDIMENTS_HAVE_UINT8_T 1

/* Ultrix style statfs */
#undef RUDIMENTS_HAVE_ULTRIX_STATFS

/* Some systems have umask */
#define RUDIMENTS_HAVE_UMASK 1

/* Some systems have uname */
#define RUDIMENTS_HAVE_UNAME 1

/* Some systems have __vsnprintf but its not defined */
#undef RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF

/* Some systems have unistd.h */
#define RUDIMENTS_HAVE_UNISTD_H 1

/* Some systems have unlink */
#define RUDIMENTS_HAVE_UNLINK 1

/* Some systems have unsetenv */
#define RUDIMENTS_HAVE_UNSETENV 1

/* Some systems have utime */
#undef RUDIMENTS_HAVE_UTIME

/* Some systems have utimes with char * argument */
#undef RUDIMENTS_HAVE_UTIMES_CHAR

/* Some systems have utimes with const char * argument */
#undef RUDIMENTS_HAVE_UTIMES_CONST_CHAR

/* Some systems have utime.h */
#define RUDIMENTS_HAVE_UTIME_H 1

/* Some systems have vasprintf */
#undef RUDIMENTS_HAVE_VASPRINTF

/* Some systems have vdprintf */
#undef RUDIMENTS_HAVE_VDPRINTF

/* Most systems have vsnprintf */
#undef RUDIMENTS_HAVE_VSNPRINTF

/* Some systems have vsnprintf_s */
#undef RUDIMENTS_HAVE_VSNPRINTF_S

/* GetDiskFreeSpace */
#undef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE

/* Some systems have windows.h */
#undef RUDIMENTS_HAVE_WINDOWS_H

/* Some systems have Sleep */
#undef RUDIMENTS_HAVE_WINDOWS_SLEEP

/* Some systems have winsock2.h */
#undef RUDIMENTS_HAVE_WINSOCK2_H

/* Some systems have write */
#define RUDIMENTS_HAVE_WRITE 1

/* Some systems have WSAConnect */
#undef RUDIMENTS_HAVE_WSACONNECT

/* Some systems have WSADuplicateSocket */
#undef RUDIMENTS_HAVE_WSADUPLICATESOCKET

/* Some systems have _access_s() */
#undef RUDIMENTS_HAVE__ACCESS_S

/* Some systems have _chsize_s */
#undef RUDIMENTS_HAVE__CHSIZE_S

/* Some systems have _close */
#undef RUDIMENTS_HAVE__CLOSE

/* Some systems have _dup */
#undef RUDIMENTS_HAVE__DUP

/* Some systems have _dup2 */
#undef RUDIMENTS_HAVE__DUP2

/* Some systems have _dupenv_s */
#undef RUDIMENTS_HAVE__DUPENV_S

/* Some systems have _lseek */
#undef RUDIMENTS_HAVE__LSEEK

/* Some systems have _memccpy */
#undef RUDIMENTS_HAVE__MEMCCPY

/* Some systems have _open */
#undef RUDIMENTS_HAVE__OPEN

/* Some systems have _pipe */
#undef RUDIMENTS_HAVE__PIPE

/* Some systems have _putenv */
#undef RUDIMENTS_HAVE__PUTENV

/* Some systems have _read */
#undef RUDIMENTS_HAVE__READ

/* Some systems have _swab */
#undef RUDIMENTS_HAVE__SWAB

/* Some systems have _unlink */
#undef RUDIMENTS_HAVE__UNLINK

/* Some systems have _vsnprintf */
#undef RUDIMENTS_HAVE__VSNPRINTF

/* Some systems have _write */
#undef RUDIMENTS_HAVE__WRITE

/* Some systems have __bswap64 */
#undef RUDIMENTS_HAVE___BSWAP64

/* Some systems have __vsnprintf */
#undef RUDIMENTS_HAVE___VSNPRINTF

/* iov_base is char * */
#define RUDIMENTS_IOV_BASE_TYPE char *

/* Some systems have swab but dont define it */
#undef RUDIMENTS_MUST_DEFINE_SWAB

/* Solaris 2.6 has a few missing function prototypes */
#undef RUDIMENTS_NEED_XNET_PROTOTYPES

/* Redfine NULL as 0 */
#undef RUDIMENTS_REDEFINE_NULL

/* setsockopt has a const void * on most systems */
#define RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE const void *

/* Most systems define signal handlers with an integer parameter */
#define RUDIMENTS_SIGNAL_HANDLER_INT 1

/* Use small rather than fast code */
#undef RUDIMENTS_SMALL_CODE

/* getsockopt has a socklen_t * on most systems */
#define RUDIMENTS_SOCKLEN_OR_SIZE_T size_t

/* SSL_read/write can use a void * parameter instead of char * */
#undef RUDIMENTS_SSL_VOID_PTR

/* Version */
#define RUDIMENTS_VERSION "0.55"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Hack for gcc on SCO OSR6 */
#undef _CFRONT_3_0

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define to `int' if <sys/types.h> doesn't define. */
#undef gid_t

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif

/* Some systems dont have key_t */
#undef key_t

/* Define to `int' if <sys/types.h> does not define. */
#undef mode_t

/* Some systems dont have nlink_t */
#undef nlink_t

/* Some systems dont have off64_t */
#define off64_t off_t

/* Define to `int' if <sys/types.h> does not define. */
#undef pid_t

/* Define to `unsigned int' if <sys/types.h> does not define. */
#undef size_t

/* Some systems dont define socklen_t */
#undef socklen_t

/* Define to `int' if <sys/types.h> does not define. */
#undef ssize_t

/* Define to `int' if <sys/types.h> doesn't define. */
#undef uid_t


#define socklen_t size_t
