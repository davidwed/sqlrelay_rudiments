// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <grp.h>
#if !defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R)
	#include <pthread.h>
#endif
