/* include/rudiments/private/config.h.in.  Generated from configure.in by autoheader.  */

/* On some platforms */
/* #undef ADD_NEWLINE_AFTER_READ_FROM_STDIN */

/* Use inline functions or not */
/* #undef ENABLE_RUDIMENTS_INLINES */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define to 1 if you have the <inttypes.h> header file. */
/* #undef HAVE_INTTYPES_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
/* #undef HAVE_STDINT_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
/* #undef HAVE_STRINGS_H */

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

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
/* #undef MINGW32 */

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

/* Platform requires default TLS ciper of PROFILE=SYSTEM */
/* #undef RUDIMENTS_DEFAULT_CIPHER_PROFILE_SYSTEM */

/* ioctl/FIONBIO doesn't work on some systems */
/* #undef RUDIMENTS_DISABLE_FIONBIO */

/* Enable runtime notification of methods that set ENOSYS */
/* #undef RUDIMENTS_ENOSYS_NOTIFY */

/* getsockopt has a void * on most systems */
#define RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE char *

/* SSL has ASN1_STRING_get0_data */
/* #undef RUDIMENTS_HAS_ASN1_STRING_GET0_DATA */

/* libcurl has CURLOPT_SSH_AUTH_TYPES */
/* #undef RUDIMENTS_HAS_CURLOPT_SSH_AUTH_TYPES */

/* libcurl has CURLOPT_USERNAME */
/* #undef RUDIMENTS_HAS_CURLOPT_USERNAME */

/* libcurl has CURLOPT_USERPWD */
/* #undef RUDIMENTS_HAS_CURLOPT_USERPWD */

/* libcurl has curl_multi_timeout */
/* #undef RUDIMENTS_HAS_CURL_MULTI_TIMEOUT */

/* SSL has EVP_PKEY_base_id */
/* #undef RUDIMENTS_HAS_EVP_PKEY_BASE_ID */

/* SSL has EVP_PKEY_get0 */
/* #undef RUDIMENTS_HAS_EVP_PKEY_GET0 */

/* Rudiments supports GSS */
/* #undef RUDIMENTS_HAS_GSS */

/* GSS has gssapi/gssapi_ext.h */
/* #undef RUDIMENTS_HAS_GSSAPI_GSSAPI_EXT_H */

/* GSS has gssapi/gssapi_generic.h */
/* #undef RUDIMENTS_HAS_GSSAPI_GSSAPI_GENERIC_H */

/* GSS has gssapi/gssapi.h */
/* #undef RUDIMENTS_HAS_GSSAPI_GSSAPI_H */

/* GSS has gssapi.h */
/* #undef RUDIMENTS_HAS_GSSAPI_H */

/* GSS has GSS_C_NT_ANONYMOUS */
/* #undef RUDIMENTS_HAS_GSS_C_NT_ANONYMOUS */

/* GSS has gss name types */
/* #undef RUDIMENTS_HAS_GSS_NAME_TYPES */

/* GSS has gss_oid_to_str */
/* #undef RUDIMENTS_HAS_GSS_OID_TO_STR */

/* GSS has gss_release_oid */
/* #undef RUDIMENTS_HAS_GSS_RELEASE_OID */

/* GSS has gss_str_to_oid */
/* #undef RUDIMENTS_HAS_GSS_STR_TO_OID */

/* Rudiments supports libcurl */
/* #undef RUDIMENTS_HAS_LIBCURL */

/* Rudiments supports PCRE */
/* #undef RUDIMENTS_HAS_PCRE */

/* Rudiments supports SSL */
/* #undef RUDIMENTS_HAS_SSL */

/* SSL has SSLv23_method */
/* #undef RUDIMENTS_HAS_SSLV23_METHOD */

/* SSL has SSLv2_method */
/* #undef RUDIMENTS_HAS_SSLV2_METHOD */

/* SSL has SSLv3_method */
/* #undef RUDIMENTS_HAS_SSLV3_METHOD */

/* SSL has dNSName */
/* #undef RUDIMENTS_HAS_SSL_DNSNAME */

/* Rudiments supports SSPI */
#define RUDIMENTS_HAS_SSPI 1

/* Rudiments supports threads */
#define RUDIMENTS_HAS_THREADS 1

/* Some systems have timezone */
#define RUDIMENTS_HAS_TIMEZONE 1

/* SSL has TLSv1_1_method */
/* #undef RUDIMENTS_HAS_TLSV1_1_METHOD */

/* SSL has TLSv1_2_method */
/* #undef RUDIMENTS_HAS_TLSV1_2_METHOD */

/* SSL has TLSv1_method */
/* #undef RUDIMENTS_HAS_TLSV1_METHOD */

/* SSL has TLS_method */
/* #undef RUDIMENTS_HAS_TLS_METHOD */

/* Some systems define tm_gmtoff in their struct tm */
/* #undef RUDIMENTS_HAS_TM_GMTOFF */

/* Some systems define tm_name in their struct tm */
/* #undef RUDIMENTS_HAS_TM_NAME */

/* Some systems define tm_tzadj in their struct tm */
/* #undef RUDIMENTS_HAS_TM_TZADJ */

/* Some systems define tm_zone in their struct tm */
/* #undef RUDIMENTS_HAS_TM_ZONE */

/* Some systems have tzname */
/* #undef RUDIMENTS_HAS_TZNAME */

/* Some systems have tzset */
/* #undef RUDIMENTS_HAS_TZSET */

/* SSL has X509_get_signature_nid */
/* #undef RUDIMENTS_HAS_X509_GET_SIGNATURE_NID */

/* Some systems have _get_timezone */
/* #undef RUDIMENTS_HAS__GET_TIMEZONE */

/* Some systems have _get_tzname */
/* #undef RUDIMENTS_HAS__GET_TZNAME */

/* Some systems have _timezone */
#define RUDIMENTS_HAS__TIMEZONE 1

/* Some systems have _tzname */
#define RUDIMENTS_HAS__TZNAME 1

/* Some systems have _tzset */
#define RUDIMENTS_HAS__TZSET 1

/* Some systems define __tm_gmtoff in their struct tm */
/* #undef RUDIMENTS_HAS___TM_GMTOFF */

/* Some systems define __tm_zone in their struct tm */
/* #undef RUDIMENTS_HAS___TM_ZONE */

/* Some systems have AccCtrl.h */
#define RUDIMENTS_HAVE_ACCCTRL_H 1

/* Most systems have access() */
#define RUDIMENTS_HAVE_ACCESS 1

/* Some systems have Aclapi.h */
#define RUDIMENTS_HAVE_ACLAPI_H 1

/* Some systems have alarm */
/* #undef RUDIMENTS_HAVE_ALARM */

/* Some systems have arc4random */
/* #undef RUDIMENTS_HAVE_ARC4RANDOM */

/* Some systems have arpa/inet.h */
/* #undef RUDIMENTS_HAVE_ARPA_INET_H */

/* Some systems have atexit */
#define RUDIMENTS_HAVE_ATEXIT 1

/* Some systems have backtrace */
/* #undef RUDIMENTS_HAVE_BACKTRACE */

/* SCO OSR < 5.0.7 has an incorrect struct msghdr definition */
/* #undef RUDIMENTS_HAVE_BAD_SCO_MSGHDR */

/* Some systems dont have st_blocks in struct stat */
/* #undef RUDIMENTS_HAVE_BLKCNT_T */

/* Some systems dont have st_blksize in struct stat */
/* #undef RUDIMENTS_HAVE_BLKSIZE_T */

/* Most systems have bool */
#define RUDIMENTS_HAVE_BOOL 1

/* Some systems have BRoster::_ShutDown */
/* #undef RUDIMENTS_HAVE_BROSTER__SHUTDOWN */

/* Some systems have bswap64 */
/* #undef RUDIMENTS_HAVE_BSWAP64 */

/* Some systems have bswap_64 */
/* #undef RUDIMENTS_HAVE_BSWAP_64 */

/* Some systems have byteswap.h */
/* #undef RUDIMENTS_HAVE_BYTESWAP_H */

/* Some systems have CaptureStackBackTrace */
#define RUDIMENTS_HAVE_CAPTURESTACKBACKTRACE 1

/* Some systems have char16_t */
/* #undef RUDIMENTS_HAVE_CHAR16_T */

/* Some systems have chmod */
/* #undef RUDIMENTS_HAVE_CHMOD */

/* Some systems have chroot */
/* #undef RUDIMENTS_HAVE_CHROOT */

/* Some systems have clearenv */
/* #undef RUDIMENTS_HAVE_CLEARENV */

/* Some systems have clock_nanosleep */
/* #undef RUDIMENTS_HAVE_CLOCK_NANOSLEEP */

/* Some systems have clock_settime */
/* #undef RUDIMENTS_HAVE_CLOCK_SETTIME */

/* Some systems have close */
/* #undef RUDIMENTS_HAVE_CLOSE */

/* some systems have closesocket */
#define RUDIMENTS_HAVE_CLOSESOCKET 1

/* Some systems have cmsghdr */
/* #undef RUDIMENTS_HAVE_CMSGHDR */

/* Some systems have _commit */
#define RUDIMENTS_HAVE_COMMIT 1

/* connect has a const sockaddr * on most systems */
/* #undef RUDIMENTS_HAVE_CONNECT_WITH_NON_CONST_SOCKADDR */

/* Most systems have ftok(const char *, int) */
/* #undef RUDIMENTS_HAVE_CONST_CHAR_FTOK */

/* Some systems have CreateDirectory */
#define RUDIMENTS_HAVE_CREATEDIRECTORY 1

/* Some systems have CreateFile */
#define RUDIMENTS_HAVE_CREATEFILE 1

/* Some systems have CreateNamedPipe */
#define RUDIMENTS_HAVE_CREATENAMEDPIPE 1

/* Some systems have CreateSemaphore */
#define RUDIMENTS_HAVE_CREATESEMAPHORE 1

/* CreateThread function exists */
#define RUDIMENTS_HAVE_CREATETHREAD 1

/* Some systems have CreateTimerQueueTimer */
#define RUDIMENTS_HAVE_CREATETIMERQUEUETIMER 1

/* Some systems have CreateFileMapping */
#define RUDIMENTS_HAVE_CREATE_FILE_MAPPING 1

/* Some systems have CreateIoCompletionPort */
#define RUDIMENTS_HAVE_CREATE_IO_COMPLETION_PORT 1

/* CreateMutex function exists */
#define RUDIMENTS_HAVE_CREATE_MUTEX 1

/* Some systems have CreateProcess */
#define RUDIMENTS_HAVE_CREATE_PROCESS 1

/* Some systems have crypt */
/* #undef RUDIMENTS_HAVE_CRYPT */

/* Some systems have CryptGenRandom */
#define RUDIMENTS_HAVE_CRYPTGENRANDOM 1

/* Some systems have crypt.h */
/* #undef RUDIMENTS_HAVE_CRYPT_H */

/* Some systems have crypt_r */
/* #undef RUDIMENTS_HAVE_CRYPT_R */

/* Some systems have ctype.h */
#define RUDIMENTS_HAVE_CTYPE_H 1

/* Cygwin style statfs */
/* #undef RUDIMENTS_HAVE_CYGWIN_STATFS */

/* Darwin style statfs */
/* #undef RUDIMENTS_HAVE_DARWIN_STATFS */

/* Some systems have dbghelp.h */
#define RUDIMENTS_HAVE_DBGHELP_H 1

/* Some systems have direct.h */
#define RUDIMENTS_HAVE_DIRECT_H 1

/* Some systems have dirent.h */
/* #undef RUDIMENTS_HAVE_DIRENT_H */

/* Some systems have dirfd */
/* #undef RUDIMENTS_HAVE_DIRFD */

/* Some systems have DIR.dd_fd */
/* #undef RUDIMENTS_HAVE_DIR_DD_FD */

/* Some systems have DIR.d_fd */
/* #undef RUDIMENTS_HAVE_DIR_D_FD */

/* Some systems have dlfcn.h */
/* #undef RUDIMENTS_HAVE_DLFCN_H */

/* Some systems have dlopen */
/* #undef RUDIMENTS_HAVE_DLOPEN */

/* Some systems have dup */
/* #undef RUDIMENTS_HAVE_DUP */

/* Some systems have dup2 */
/* #undef RUDIMENTS_HAVE_DUP2 */

/* Some systems have DuplicateHandle */
#define RUDIMENTS_HAVE_DUPLICATEHANDLE 1

/* Some systems define environ in unistd.h */
#define RUDIMENTS_HAVE_ENVIRON 1

/* Some systems have epoll */
/* #undef RUDIMENTS_HAVE_EPOLL */

/* Some systems have errno.h */
#define RUDIMENTS_HAVE_ERRNO_H 1

/* Some systems have execinfo.h */
/* #undef RUDIMENTS_HAVE_EXECINFO_H */

/* Some systems have execvp */
/* #undef RUDIMENTS_HAVE_EXECVP */

/* Some systems have ExitWindowsEx */
#define RUDIMENTS_HAVE_EXITWINDOWSEX 1

/* Some systems have fchmod */
/* #undef RUDIMENTS_HAVE_FCHMOD */

/* Some systems have fchown */
/* #undef RUDIMENTS_HAVE_FCHOWN */

/* Some systems have fcntl */
/* #undef RUDIMENTS_HAVE_FCNTL */

/* Some systems have fcntl.h */
#define RUDIMENTS_HAVE_FCNTL_H 1

/* Some systems have fdatasync */
/* #undef RUDIMENTS_HAVE_FDATASYNC */

/* Some systems have fdopen */
/* #undef RUDIMENTS_HAVE_FDOPEN */

/* Some systems have FD_CLOEXEC */
/* #undef RUDIMENTS_HAVE_FD_CLOEXEC */

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
#define RUDIMENTS_HAVE_FINDFIRSTFILE 1

/* Some systems have FlushFileBuffers */
#define RUDIMENTS_HAVE_FLUSHFILEBUFFERS 1

/* Some systems have fork */
/* #undef RUDIMENTS_HAVE_FORK */

/* Some systems have fpathconf */
/* #undef RUDIMENTS_HAVE_FPATHCONF */

/* FreeBSD style statfs */
/* #undef RUDIMENTS_HAVE_FREEBSD_STATFS */

/* Some systems have FreeConsole */
#define RUDIMENTS_HAVE_FREECONSOLE 1

/* Some systems have fsync */
/* #undef RUDIMENTS_HAVE_FSYNC */

/* Most systems have ftok() */
/* #undef RUDIMENTS_HAVE_FTOK */

/* Some systems have ftruncate */
/* #undef RUDIMENTS_HAVE_FTRUNCATE */

/* Some systems have GenerateConsoleCtrlEvent */
#define RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT 1

/* Some systems have GetAce */
#define RUDIMENTS_HAVE_GETACE 1

/* Some systems have getaddrinfo */
/* #undef RUDIMENTS_HAVE_GETADDRINFO */

/* Some systems have GetCurrentDirectory */
#define RUDIMENTS_HAVE_GETCURRENTDIRECTORY 1

/* Some systems have GetCurrentProcessId */
#define RUDIMENTS_HAVE_GETCURRENTPROCESSID 1

/* Some systems have getegid */
/* #undef RUDIMENTS_HAVE_GETEGID */

/* Some systems have getenv */
#define RUDIMENTS_HAVE_GETENV 1

/* Some systems have geteuid */
/* #undef RUDIMENTS_HAVE_GETEUID */

/* Some systems have GetFileInformationByHandle */
#define RUDIMENTS_HAVE_GETFILEINFORMATIONBYHANDLE 1

/* Some systems have GetFileType */
#define RUDIMENTS_HAVE_GETFILETYPE 1

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

/* Some systems have gethostname */
#define RUDIMENTS_HAVE_GETHOSTNAME 1

/* Some systems have GetLastError */
#define RUDIMENTS_HAVE_GETLASTERROR 1

/* Some systems have getloadavg */
/* #undef RUDIMENTS_HAVE_GETLOADAVG */

/* Some systems have GetLogicalProcessorInformation */
/* #undef RUDIMENTS_HAVE_GETLOGICALPROCESSORINFORMATION */

/* Some systems have getnameinfo */
#define RUDIMENTS_HAVE_GETNAMEINFO 1

/* Some systems have GetNativeSystemInfo */
/* #undef RUDIMENTS_HAVE_GETNATIVESYSTEMINFO */

/* Some systems have getpagesize */
/* #undef RUDIMENTS_HAVE_GETPAGESIZE */

/* Some systems have getpgid */
/* #undef RUDIMENTS_HAVE_GETPGID */

/* Some systems have getpid */
/* #undef RUDIMENTS_HAVE_GETPID */

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

/* Some systems have GetSecurityInfo */
#define RUDIMENTS_HAVE_GETSECURITYINFO 1

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

/* Some systems have GetSystemTimeAsFileTime */
#define RUDIMENTS_HAVE_GETSYSTEMTIMEASFILETIME 1
 
/* Some systems have gettimeofday */
/* #undef RUDIMENTS_HAVE_GETTIMEOFDAY */

/* Some systems have getuid */
/* #undef RUDIMENTS_HAVE_GETUID */

/* Some systems have GetUserName */
#define RUDIMENTS_HAVE_GETUSERNAME 1

/* Some systems have GetVersionEx */
#define RUDIMENTS_HAVE_GETVERSIONEX 1

/* Some systems have GlobalMemoryStatusEx */
#define RUDIMENTS_HAVE_GLOBALMEMORYSTATUSEX 1

/* Some systems have grp.h */
/* #undef RUDIMENTS_HAVE_GRP_H */

/* Some systems have _G_config.h */
#undef RUDIMENTS_HAVE_G_CONFIG_H

/* Some systems have HANDLE_FLAG_INHERIT */
#define RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT 1

/* Some systems have htonll */
/* #undef RUDIMENTS_HAVE_HTONLL */

/* Some systems have inet_addr */
#define RUDIMENTS_HAVE_INET_ADDR 1

/* Some systems have inet_aton */
/* #undef RUDIMENTS_HAVE_INET_ATON */

/* Some systems have InitiateSystemShutdownEx */
#define RUDIMENTS_HAVE_INITIATESYSTEMSHUTDOWNEX 1

/* Some systems have int16_t */
/* #undef RUDIMENTS_HAVE_INT16_T */

/* Some systems have int32_t */
/* #undef RUDIMENTS_HAVE_INT32_T */

/* Some systems have int64_t */
/* #undef RUDIMENTS_HAVE_INT64_T */

/* Some systems have int8_t */
/* #undef RUDIMENTS_HAVE_INT8_T */

/* Some systems have inttypes.h */
/* #undef RUDIMENTS_HAVE_INTTYPES_H */

/* Some systems have in_addr_t type */
/* #undef RUDIMENTS_HAVE_IN_ADDR_T */

/* Some systems have ioctl */
/* #undef RUDIMENTS_HAVE_IOCTL */

/* Some systems have ioctlsocket */
#define RUDIMENTS_HAVE_IOCTLSOCKET 1

/* Some systems have io.h */
#define RUDIMENTS_HAVE_IO_H 1

/* Some systems have kill */
/* #undef RUDIMENTS_HAVE_KILL */

/* Some systems have kqueue */
/* #undef RUDIMENTS_HAVE_KQUEUE */

/* Some systems have void * kqueue user data */
/* #undef RUDIMENTS_HAVE_KQUEUE_VOID_UDATA */

/* Do we have libedit */
/* #undef RUDIMENTS_HAVE_LIBEDIT */

/* Some systems have limits.h */
#define RUDIMENTS_HAVE_LIMITS_H 1

/* Some systems have link */
/* #undef RUDIMENTS_HAVE_LINK */

/* Linux libc4 style statfs */
/* #undef RUDIMENTS_HAVE_LINUX_LIBC4_STATFS */

/* Some systems have linux/reboot.h */
/* #undef RUDIMENTS_HAVE_LINUX_REBOOT_H */

/* Some systems have linux/rtc.h */
/* #undef RUDIMENTS_HAVE_LINUX_RTC_H */

/* Linux style statfs */
/* #undef RUDIMENTS_HAVE_LINUX_STATFS */

/* Some systems have llabs */
/* #undef RUDIMENTS_HAVE_LLABS */

/* Some systems have lm.h */
#define RUDIMENTS_HAVE_LM_H 1

/* Some systems have LoadLibrary */
#define RUDIMENTS_HAVE_LOADLIBRARY 1

/* Some systems have LoadLibraryEx */
#define RUDIMENTS_HAVE_LOADLIBRARYEX 1

/* Some systems have localtime_r */
/* #undef RUDIMENTS_HAVE_LOCALTIME_R */

/* Some systems have localtime_s */
/* #undef RUDIMENTS_HAVE_LOCALTIME_S */

/* Some systems have LockFileEx */
#define RUDIMENTS_HAVE_LOCKFILEEX 1

/* Most systems have long long */
/* #undef RUDIMENTS_HAVE_LONG_LONG */

/* Some systems have lrand48 */
/* #undef RUDIMENTS_HAVE_LRAND48 */

/* Some systems have lrand48_r */
/* #undef RUDIMENTS_HAVE_LRAND48_R */

/* Some systems have lseek */
/* #undef RUDIMENTS_HAVE_LSEEK */

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

/* Some systems have sethostname but forgot to declare it */
/* #undef RUDIMENTS_HAVE_MISSING_SETHOSTNAME_DECLARATION */

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

/* Some msghdr structs have a msg_accrights member */
/* #define RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS */

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

/* Some systems have new */
#define RUDIMENTS_HAVE_NEW 1

/* Some systems have new.h */
/* #undef RUDIMENTS_HAVE_NEW_H */

/* Some systems define _NSGetEnviron in crt_externs.h */
/* #undef RUDIMENTS_HAVE_NSGETENVIRON */

/* Some systems have ntohll */
/* #undef RUDIMENTS_HAVE_NTOHLL */

/* Some systems have open */
/* #undef RUDIMENTS_HAVE_OPEN */

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

/* Some systems have pipe */
/* #undef RUDIMENTS_HAVE_PIPE */

/* Some systems have poll */
/* #undef RUDIMENTS_HAVE_POLL */

/* Some systems have poll.h */
/* #undef RUDIMENTS_HAVE_POLL_H */

/* Some systems have posix_fadvise */
/* #undef RUDIMENTS_HAVE_POSIX_FADVISE */

/* Some systems have posix_fallocate */
/* #undef RUDIMENTS_HAVE_POSIX_FALLOCATE */

/* Some systems have Process32First */
#define RUDIMENTS_HAVE_PROCESS32FIRST 1

/* Some systems have process.h */
#define RUDIMENTS_HAVE_PROCESS_H 1

/* Some systems have prototypes.h */
/* #undef RUDIMENTS_HAVE_PROTOTYPES_H */

/* Some systems have psapi.h */
#define RUDIMENTS_HAVE_PSAPI_H 1

/* Some systems have pselect */
/* #undef RUDIMENTS_HAVE_PSELECT */

/* Some systems have pthread_attr_setstacksize */
/* #undef RUDIMENTS_HAVE_PTHREAD_ATTR_SETSTACKSIZE */

/* pthread_mutex_t type exists */
/* #undef RUDIMENTS_HAVE_PTHREAD_MUTEX_T */

/* pthread_sigmask exists */
/* #undef RUDIMENTS_HAVE_PTHREAD_SIGMASK */

/* pthread_t type exists */
/* #undef RUDIMENTS_HAVE_PTHREAD_T */

/* Some systems have putenv */
#define RUDIMENTS_HAVE_PUTENV 1

/* Some systems have raise */
#define RUDIMENTS_HAVE_RAISE 1

/* Some systems have rand */
#define RUDIMENTS_HAVE_RAND 1

/* Some systems have random */
/* #undef RUDIMENTS_HAVE_RANDOM */

/* Some systems have random_r */
/* #undef RUDIMENTS_HAVE_RANDOM_R */

/* Some systems have rand_r */
/* #undef RUDIMENTS_HAVE_RAND_R */

/* Some systems have read */
/* #undef RUDIMENTS_HAVE_READ */

/* Some systems have readlink */
/* #undef RUDIMENTS_HAVE_READLINK */

/* Some systems have reboot with 1 argument */
/* #undef RUDIMENTS_HAVE_REBOOT_1 */

/* Some systems have reboot with 2 argument */
/* #undef RUDIMENTS_HAVE_REBOOT_2 */

/* recv has a void * on most systems */
/* #undef RUDIMENTS_HAVE_RECV_WITH_VOID */

/* some systems have regcomp */
/* #undef RUDIMENTS_HAVE_REGCOMP */

/* Some systems have RemoveDirectory */
#define RUDIMENTS_HAVE_REMOVEDIRECTORY 1

/* Some systems have Roster.h */
/* #undef RUDIMENTS_HAVE_ROSTER_H */

/* Some systems have rpc/rpcent.h */
/* #undef RUDIMENTS_HAVE_RPCENT_H */

/* Some systems have rpc/rpc.h */
/* #undef RUDIMENTS_HAVE_RPC_H */

/* Do we have real-time-clock RTC_RD_TIME support */
/* #undef RUDIMENTS_HAVE_RTC_RD_TIME */

/* Do we have real-time-clock RTC_SET_TIME support */
/* #undef RUDIMENTS_HAVE_RTC_SET_TIME */

/* SCO has /dev/table/avenrun instead of getloadavg */
/* #undef RUDIMENTS_HAVE_SCO_AVENRUN */

/* Some systems have sddl.h */
/* #undef RUDIMENTS_HAVE_SDDL_H */

/* Some systems have semget */
/* #undef RUDIMENTS_HAVE_SEMGET */

/* Some systems have semtimedop */
/* #undef RUDIMENTS_HAVE_SEMTIMEDOP */

/* Some systems define union semun */
/* #undef RUDIMENTS_HAVE_SEMUN */

/* send has a const void * on most systems */
/* #undef RUDIMENTS_HAVE_SEND_WITH_VOID */

/* Some systems have SetConsoleCtrlHandler */
#define RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER 1

/* Some systems have SetCurrentDirectory */
#define RUDIMENTS_HAVE_SETCURRENTDIRECTORY 1

/* Some systems have setegid */
/* #undef RUDIMENTS_HAVE_SETEGID */

/* Some systems have setegid but dont define it in a header file */
/* #undef RUDIMENTS_HAVE_SETEGID_BUT_NOT_DEFINED */

/* Some systems have SetEndOfFile */
#define RUDIMENTS_HAVE_SETENDOFFILE 1

/* Some systems have SetEntriesInAcl */
#define RUDIMENTS_HAVE_SETENTRIESINACL 1

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

/* Some systems have setgroups */
/* #undef RUDIMENTS_HAVE_SETGROUPS */

/* Some systems have sethostname */
/* #undef RUDIMENTS_HAVE_SETHOSTNAME */

/* Some systems have setitimer */
/* #undef RUDIMENTS_HAVE_SETITIMER */

/* Some systems have setpgid */
/* #undef RUDIMENTS_HAVE_SETPGID */

/* Some systems have setregid */
/* #undef RUDIMENTS_HAVE_SETREGID */

/* Some systems have SetSecurityInfo */
#define RUDIMENTS_HAVE_SETSECURITYINFO 1

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

/* Some systems have SetUnhandledExceptionFilter */
#define RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER 1

/* Some systems have set_real_time_clock */
/* #undef RUDIMENTS_HAVE_SET_REAL_TIME_CLOCK */

/* Some systems have shmget */
/* #undef RUDIMENTS_HAVE_SHMGET */

/* Some systems have sigaction */
/* #undef RUDIMENTS_HAVE_SIGACTION */

/* Some systems have signal */
#define RUDIMENTS_HAVE_SIGNAL 1

/* Some systems have sigpending */
/* #undef RUDIMENTS_HAVE_SIGPENDING */

/* Some systems have sigsuspend */
/* #undef RUDIMENTS_HAVE_SIGSUSPEND */

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

/* statvfs has f_b/fresvd */
/* #undef RUDIMENTS_HAVE_STATVFS_RESVD */

/* Some systems have stdarg.h */
#define RUDIMENTS_HAVE_STDARG_H 1

/* Some systems have stdint.h */
/* #undef RUDIMENTS_HAVE_STDINT_H */

/* Some systems have stdlib.h */
#define RUDIMENTS_HAVE_STDLIB_H 1

/* Some systems have strcasecmp */
/* #undef RUDIMENTS_HAVE_STRCASECMP */

/* Some systems have strchrnul */
/* #undef RUDIMENTS_HAVE_STRCHRNUL */

/* Some systems have strcspn */
#define RUDIMENTS_HAVE_STRCSPN 1

/* Some systems have strerror */
#define RUDIMENTS_HAVE_STRERROR 1

/* Some systems have strerror_r */
/* #undef RUDIMENTS_HAVE_STRERROR_R */

/* Some systems have strerror_s */
/* #undef RUDIMENTS_HAVE_STRERROR_S */

/* Some systems have strings.h */
/* #undef RUDIMENTS_HAVE_STRINGS_H */

/* Some systems have string.h */
#define RUDIMENTS_HAVE_STRING_H 1

/* Some systems have strncasecmp */
/* #undef RUDIMENTS_HAVE_STRNCASECMP */

/* Some systems have strpbrk */
#define RUDIMENTS_HAVE_STRPBRK 1

/* Some systems have strspn */
#define RUDIMENTS_HAVE_STRSPN 1

/* Some systems have strtold */
/* #undef RUDIMENTS_HAVE_STRTOLD */

/* Some systems have strtoll */
/* #undef RUDIMENTS_HAVE_STRTOLL */

/* Some systems have strtoull */
/* #undef RUDIMENTS_HAVE_STRTOULL */

/* Some systems have struct itimerval */
/* #undef RUDIMENTS_HAVE_STRUCT_ITIMERVAL */

/* Some systems have struct timespec in pthread.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD */

/* Some systems have struct timespec in sys/siginfo.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO */

/* Some systems have struct timespec in sys/time.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SYS_TIME_H */

/* Some systems have struct timespec in time.h */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_TIME_H */

/* Some systems have struct timeval */
/* #undef RUDIMENTS_HAVE_STRUCT_TIMEVAL */

/* Some systems have swab with a char * argument */
#define RUDIMENTS_HAVE_SWAB_CHAR 1

/* Some systems have swab with a const char * argument */
/* #undef RUDIMENTS_HAVE_SWAB_CONST_CHAR */

/* Some systems have swab with a const void * argument */
/* #undef RUDIMENTS_HAVE_SWAB_CONST_VOID */

/* Some systems have swap64 */
/* #undef RUDIMENTS_HAVE_SWAP64 */

/* Some systems have __swap_int64 */
/* #undef RUDIMENTS_HAVE_SWAP_INT64 */

/* Some systems have symlink */
/* #undef RUDIMENTS_HAVE_SYMLINK */

/* Some systems have sync */
/* #undef RUDIMENTS_HAVE_SYNC */

/* Some systems have sysconf */
/* #undef RUDIMENTS_HAVE_SYSCONF */

/* Some systems have syslog.h */
/* #undef RUDIMENTS_HAVE_SYSLOG_H */

/* Some systems have sys/bitypes.h */
/* #undef RUDIMENTS_HAVE_SYS_BITYPES_H */

/* Some systems have sys/byteorder.h */
/* #undef RUDIMENTS_HAVE_SYS_BYTEORDER_H */

/* Some systems have sys/devpoll.h */
/* #undef RUDIMENTS_HAVE_SYS_DEVPOLL_H */

/* Some systems have sys/epoll.h */
/* #undef RUDIMENTS_HAVE_SYS_EPOLL_H */

/* Some systems have sys/event.h */
/* #undef RUDIMENTS_HAVE_SYS_EVENT_H */

/* Some systems have sys/fcntl.h */
/* #undef RUDIMENTS_HAVE_SYS_FCNTL_H */

/* Some systems have sys/ioctl.h */
/* #undef RUDIMENTS_HAVE_SYS_IOCTL_H */

/* Some systems have sys/ipc.h */
/* #undef RUDIMENTS_HAVE_SYS_IPC_H */

/* Some systems have sys/loadavg.h */
/* #undef RUDIMENTS_HAVE_SYS_LOADAVG_H */

/* Some systems have sys/mman.h */
/* #undef RUDIMENTS_HAVE_SYS_MMAN_H */

/* Some systems have sys/param.h */
/* #undef RUDIMENTS_HAVE_SYS_PARAM_H */

/* Some systems have sys/poll.h */
/* #undef RUDIMENTS_HAVE_SYS_POLL_H */

/* Some systems have port_create */
/* #undef RUDIMENTS_HAVE_PORT_CREATE */

/* Some systems have port.h */
/* #undef RUDIMENTS_HAVE_PORT_H */

/* Some systems have sys/reboot.h */
/* #undef RUDIMENTS_HAVE_SYS_REBOOT_H */

/* Some systems have sys/select.h */
/* #undef RUDIMENTS_HAVE_SYS_SELECT_H */

/* Some systems have sys/sem.h */
/* #undef RUDIMENTS_HAVE_SYS_SEM_H */

/* Some systems have sys/signal.h */
/* #undef RUDIMENTS_HAVE_SYS_SIGNAL_H */

/* Some systems have sys/socket.h */
/* #undef RUDIMENTS_HAVE_SYS_SOCKET_H */

/* Some systems have sys/stat.h */
#define RUDIMENTS_HAVE_SYS_STAT_H 1

/* Some systems have sys/time.h */
/* #undef RUDIMENTS_HAVE_SYS_TIME_H */

/* Some systems have sys/types.h */
#define RUDIMENTS_HAVE_SYS_TYPES_H 1

/* Some systems have sys/uadmin.h */
/* #undef RUDIMENTS_HAVE_SYS_REBOOT_H */

/* Some systems have sys/uio.h */
/* #undef RUDIMENTS_HAVE_SYS_UIO_H */

/* Some systems have sys/un.h */
/* #undef RUDIMENTS_HAVE_SYS_UN_H */

/* Some systems have sys/utsname.h */
/* #undef RUDIMENTS_HAVE_SYS_UTSNAME_H */

/* Some systems have sys/wait.h */
/* #undef RUDIMENTS_HAVE_SYS_WAIT_H */

/* Some systems dont have S_ISBLK */
/* #undef RUDIMENTS_HAVE_S_ISBLK */

/* Some systems dont have S_ISLNK */
/* #undef RUDIMENTS_HAVE_S_ISLNK */

/* Some systems dont have S_ISSOCK */
/* #undef RUDIMENTS_HAVE_S_ISSOCK */

/* Some systems have tcsetattr */
/* #undef RUDIMENTS_HAVE_TCSETATTR */

/* Some systems have termios.h */
/* #undef RUDIMENTS_HAVE_TERMIOS_H */

/* Some systems have time.h */
#define RUDIMENTS_HAVE_TIME_H 1

/* Some systems have tlhelp32.h */
#define RUDIMENTS_HAVE_TLHELP32_H 1

/* Most systems have true/false */
#define RUDIMENTS_HAVE_TRUE_FALSE 1

/* Some systems have truncate */
/* #undef RUDIMENTS_HAVE_TRUNCATE */

/* Some systems have uadmin with a char * argument */
/* #undef RUDIMENTS_HAVE_UADMIN_CHAR */

/* Some systems have uadmin with a integer argument */
/* #undef RUDIMENTS_HAVE_UADMIN_INT */

/* Some systems have uchar.h */
/* #undef RUDIMENTS_HAVE_UCHAR_H */

/* Some systems have uint16_t */
/* #undef RUDIMENTS_HAVE_UINT16_T */

/* Some systems have uint32_t */
/* #undef RUDIMENTS_HAVE_UINT32_T */

/* Some systems have uint64_t */
/* #undef RUDIMENTS_HAVE_UINT64_T */

/* Some systems have uint8_t */
/* #undef RUDIMENTS_HAVE_UINT8_T */

/* Ultrix style statfs */
/* #undef RUDIMENTS_HAVE_ULTRIX_STATFS */

/* Some systems have umask */
/* #undef RUDIMENTS_HAVE_UMASK */

/* Some systems have uname */
/* #undef RUDIMENTS_HAVE_UNAME */

/* Some systems have __vsnprintf but its not defined */
/* #undef RUDIMENTS_HAVE_UNDEFINED___VSNPRINTF */

/* Some systems have unistd.h */
/* #undef RUDIMENTS_HAVE_UNISTD_H */

/* Some systems have unlink */
#define RUDIMENTS_HAVE_UNLINK 1

/* Some systems have unsetenv */
/* #undef RUDIMENTS_HAVE_UNSETENV */

/* Some systems have utime */
/* #undef RUDIMENTS_HAVE_UTIME */

/* Some systems have utimes with char * argument */
/* #undef RUDIMENTS_HAVE_UTIMES_CHAR */

/* Some systems have utimes with const char * argument */
/* #undef RUDIMENTS_HAVE_UTIMES_CONST_CHAR */

/* Some systems have utime.h */
/* #undef RUDIMENTS_HAVE_UTIME_H */

/* Some systems have vasprintf */
/* #undef RUDIMENTS_HAVE_VASPRINTF */

/* Some systems have vdprintf */
/* #undef RUDIMENTS_HAVE_VDPRINTF */

/* Most systems have vsnprintf */
/* #undef RUDIMENTS_HAVE_VSNPRINTF */

/* Some systems have vsnprintf_s */
/* #undef RUDIMENTS_HAVE_VSNPRINTF_S */

/* GetDiskFreeSpace */
#define RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE 1

/* Some systems have windows.h */
#define RUDIMENTS_HAVE_WINDOWS_H 1

/* Some systems have Sleep */
#define RUDIMENTS_HAVE_WINDOWS_SLEEP 1

/* Some systems have winsock2.h */
#define RUDIMENTS_HAVE_WINSOCK2_H 1

/* Some systems have write */
#define RUDIMENTS_HAVE_WRITE 1

/* Some systems have WSAConnect */
#define RUDIMENTS_HAVE_WSACONNECT 1

/* Some systems have WSADuplicateSocket */
#define RUDIMENTS_HAVE_WSADUPLICATESOCKET 1

/* Some systems have _access_s() */
/* #undef RUDIMENTS_HAVE__ACCESS_S */

/* Some systems have _chsize_s */
/* #undef RUDIMENTS_HAVE__CHSIZE_S */

/* Some systems have _close */
#define RUDIMENTS_HAVE__CLOSE 1

/* Some systems have _dup */
#define RUDIMENTS_HAVE__DUP 1

/* Some systems have _dup2 */
#define RUDIMENTS_HAVE__DUP2 1

/* Some systems have _dupenv_s */
/* #undef RUDIMENTS_HAVE__DUPENV_S */

/* Some systems have _lseek */
#define RUDIMENTS_HAVE__LSEEK 1

/* Some systems have _memccpy */
#define RUDIMENTS_HAVE__MEMCCPY 1

/* Some systems have _open */
#define RUDIMENTS_HAVE__OPEN 1

/* Some systems have _pipe */
#define RUDIMENTS_HAVE__PIPE 1

/* Some systems have _putenv */
#define RUDIMENTS_HAVE__PUTENV 1

/* Some systems have _read */
#define RUDIMENTS_HAVE__READ 1

/* Some systems have _swab */
#define RUDIMENTS_HAVE__SWAB 1

/* Some systems have _unlink */
#define RUDIMENTS_HAVE__UNLINK 1

/* Some systems have _vsnprintf */
#define RUDIMENTS_HAVE__VSNPRINTF 1

/* Some systems have _write */
#define RUDIMENTS_HAVE__WRITE 1

/* Some systems have __bswap64 */
/* #undef RUDIMENTS_HAVE___BSWAP64 */

/* iov_base is char * */
#define RUDIMENTS_IOV_BASE_TYPE char *

/* Some versions of libedit provide history_truncate_file */
/* #undef RUDIMENTS_LIBEDIT_HAS_HISTORY_TRUNCATE_FILE */

/* Some systems have swab but dont define it */
/* #undef RUDIMENTS_MUST_DEFINE_SWAB */

/* Solaris 2.6 has a few missing function prototypes */
/* #undef RUDIMENTS_NEED_XNET_PROTOTYPES */

/* Redfine NULL as 0 */
/* #undef RUDIMENTS_REDEFINE_NULL */

/* setsockopt has a const void * on most systems */
#define RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE const char *

/* Most systems define signal handlers with an integer parameter */
#define RUDIMENTS_SIGNAL_HANDLER_INT 1

/* Use small rather than fast code */
/* #undef RUDIMENTS_SMALL_CODE */

/* getsockopt has a socklen_t * on most systems */
#define RUDIMENTS_SOCKLEN_OR_SIZE_T socklen_t

/* SSL_read/write can use a void * parameter instead of char * */
/* #undef RUDIMENTS_SSL_VOID_PTR */

/* Version */
#define RUDIMENTS_VERSION "@RUDIMENTS_VERSION@"

/* Define to 1 if you have the ANSI C header files. */
/* #undef STDC_HEADERS */

/* Hack for gcc on SCO OSR6 */
/* #undef _CFRONT_3_0 */

/* Syllable OS */
/* #undef _SYLLABLE */

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
