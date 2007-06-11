// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#include <sys/time.h>

#ifdef HAVE_STRUCT_TIMESPEC_IN_SIGINFO
	#include <sys/siginfo.h>
#endif


#if !defined(HAVE_STRUCT_TIMESPEC) && !defined(HAVE_STRUCT_TIMESPEC_IN_SIGINFO)
struct timespec {
	time_t	tv_sec;
	long	tv_nsec;
};
#endif
