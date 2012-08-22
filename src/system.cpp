// Copyright (c) 2012 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/system.h>
#include <rudiments/error.h>
#include <rudiments/charstring.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	// for getpagesize
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_UTSNAME_H
	// for uname
	#include <sys/utsname.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	// for getloadavg
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAVE_UNAME)
#define	UNAME(part) \
	struct utsname	u; \
	int32_t		result; \
	do { \
		result=uname(&u); \
	} while (result==-1 && error::getErrorNumber()==EINTR); \
	return (result==-1)?NULL:charstring::duplicate(u.part);
#endif

char *system::getSystemName() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		UNAME(sysname);
	#else
		#error no uname or anything like it
	#endif
}

char *system::getOperatingSystemRelease() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		UNAME(release);
	#else
		#error no uname or anything like it
	#endif
}

char *system::getOperatingSystemVersion() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		UNAME(version);
	#else
		#error no uname or anything like it
	#endif
}

char *system::getSystemArchitecture() {
	#if defined(RUDIMENTS_HAVE_UNAME)
		UNAME(machine);
	#else
		#error no uname or anything like it
	#endif
}

int32_t system::getPageSize() {
	#if defined(RUDIMENTS_HAVE_GETPAGESIZE)
		return getpagesize();
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMINFO)
		SYSTEM_INFO	systeminfo;
		GetSystemInfo(&systeminfo);
		return systeminfo.dwPageSize;
	#else
		#error no getpagesize or anything like it
	#endif
}

bool system::getLoadAverages(double *averages, uint32_t count) {
	#if defined(RUDIMENTS_HAVE_GETLOADAVG)
		return !getloadavg(averages,count);
	#else
		error::setError(ENOSYS);
		return false;
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
