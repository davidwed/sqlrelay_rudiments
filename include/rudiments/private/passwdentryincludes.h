// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <pwd.h>
#if !defined(HAVE_GETPWNAM_R) || !defined(HAVE_GETPWUID_R)
	#include <pthread.h>
#endif
