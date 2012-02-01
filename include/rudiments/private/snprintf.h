// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>
#include <stdio.h>

#ifndef RUDIMENTS_HAVE_SNPRINTF
	#ifdef RUDIMENTS_HAVE__SNPRINTF
		#define snprintf _snprintf
	#else
		#error no snprintf or anything like it
	#endif
#endif
