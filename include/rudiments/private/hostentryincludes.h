// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
	#include <pthread.h>
#endif

#include <netdb.h>
