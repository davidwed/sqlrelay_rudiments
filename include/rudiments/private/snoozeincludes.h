// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

// for timeval
#include <sys/time.h>

// for timespec
#if defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_TIME_H)
	#include <time.h>
#elif defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO)
	#include <sys/siginfo.h>
#elif defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD)
	#include <pthread.h>
#elif !defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SYS_TIME_H)
	struct timespec {
		time_t	tv_sec;
		long	tv_nsec;
	};
#endif
