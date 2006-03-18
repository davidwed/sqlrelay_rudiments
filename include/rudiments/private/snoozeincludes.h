// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#include <sys/time.h>

#ifndef HAVE_STRUCT_TIMESPEC
struct timespec {
	time_t	tv_sec;
	long	tv_nsec;
};
#endif
