// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
	#include <pthread.h>
#endif

#include <netdb.h>
