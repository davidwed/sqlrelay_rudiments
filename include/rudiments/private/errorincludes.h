// Copyright (c) 2012 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <errno.h>
#include <rudiments/private/inttypes.h>

#ifdef RUDIMENTS_ENOSYS_NOTIFY

	#include <rudiments/stdio.h>

	#ifdef __GNUC__
		#define RUDIMENTS_CURRENT_FUNCTION __PRETTY_FUNCTION__
	#else
		#define RUDIMENTS_CURRENT_FUNCTION __FUNCTION__
	#endif


	#define RUDIMENTS_SET_ENOSYS \
	{ \
		error::setErrorNumber(ENOSYS); \
		stderror.printf("warning: %s set ENOSYS\n", \
					RUDIMENTS_CURRENT_FUNCTION); \
	}

#else

	#define RUDIMENTS_SET_ENOSYS \
		error::setErrorNumber(ENOSYS);

#endif
