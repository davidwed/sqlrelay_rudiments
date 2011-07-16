// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#include <sys/time.h>

#ifdef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO
	#include <sys/siginfo.h>
#endif

#ifdef RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD
	#include <pthread.h>
#endif


#if !defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC) && \
	!defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO) && \
	!defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD)
struct timespec {
	time_t	tv_sec;
	long	tv_nsec;
};
#endif
