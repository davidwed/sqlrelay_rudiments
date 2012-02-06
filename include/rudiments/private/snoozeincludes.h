// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

// for timeval
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif

#ifndef RUDIMENTS_HAVE_STRUCT_TIMEVAL
	struct timeval {
		long	tv_sec;
		long	tv_usec;
	};
#endif

// for timespec
#if defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_TIME_H)
	#include <time.h>
#elif defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO)
	#include <sys/siginfo.h>
#elif defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD)
	// for mingw32
	#include <pthread.h>
#elif !defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SYS_TIME_H)
	struct timespec {
		time_t	tv_sec;
		long	tv_nsec;
	};
#endif
