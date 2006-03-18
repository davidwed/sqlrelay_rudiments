// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#include <sys/types.h>

#ifdef MINGW32
	#include <windows.h>
	#include <lm.h>
#else
	#include <pwd.h>
#endif

#ifdef RUDIMENTS_HAS_THREADS
	#include <rudiments/mutex.h>
#endif
