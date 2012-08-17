// Copyright (c) 2012 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/system.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
