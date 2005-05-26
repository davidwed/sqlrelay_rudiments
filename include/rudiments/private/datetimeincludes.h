// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/environment.h>
#ifdef HAVE_GETSYSTEMTIME
	#include <rudiments/rawbuffer.h>
#endif

#ifdef HAVE_WINDOWS_H
	#include <windows.h>
#endif
#include <time.h>
#include <sys/types.h>
#ifdef RUDIMENTS_HAS_THREADS
	#include <rudiments/mutex.h>
#endif
