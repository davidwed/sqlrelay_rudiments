// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>

#ifdef HAVE_CRYPT_R
#define __USE_GNU
#endif
#include <crypt.h>

#ifdef RUDIMENTS_HAS_THREADS
	#include <pthread.h>
#endif
