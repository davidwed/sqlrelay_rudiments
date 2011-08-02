// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#if defined(RUDIMENTS_HAVE_PTHREAD_MUTEX_T)
	// to disable pthread macros on minix, for now
	#define _PTHREAD_PRIVATE
	#include <pthread.h>
#elif defined(RUDIMENTS_HAVE_CREATE_MUTEX)
	#include <windows.h>
	typedef HANDLE pthread_mutex_t *;
#else
	typedef void pthread_mutex_t;
#endif
