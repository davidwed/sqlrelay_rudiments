// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#ifdef RUDIMENTS_HAVE_PTHREAD_MUTEX_T
	#include <pthread.h>
#endif

#ifdef RUDIMENTS_HAVE_CREATE_MUTEX
	#include <windows.h>
#endif
