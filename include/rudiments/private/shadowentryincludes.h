// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <shadow.h>
#if !defined(HAVE_GETSPNAM_R)
	#include <pthread.h>
#endif
