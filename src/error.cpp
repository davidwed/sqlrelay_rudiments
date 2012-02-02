// Copyright (c) 2005 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/error.h>

#include <stdio.h>

// to get the more portable XSI-compliant strerror_r rather than the GNU version
#undef __USE_GNU

// for strerror (or similar)
#ifdef RUDIMENTS_HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <string.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAVE_STRERROR_S) || \
	defined(RUDIMENTS_HAVE_STRERROR_R)
static	char	errorbuffer[256];
#endif

void error::clearError() {
	errno=0;
}

void error::setErrorNumber(int32_t err) {
	errno=err;
}

int32_t error::getErrorNumber() {
	return errno;
}

const char *error::getErrorString() {
	// FIXME: strerror_s and strerror_r are intended to be
	// thread safe but this use of them is certainly not
	#if defined(RUDIMENTS_HAVE_STRERROR_S)
		strerror_s(errorbuffer,sizeof(errorbuffer),errno);
		return errorbuffer;
	#elif defined(RUDIMENTS_HAVE_STRERROR_R)
		strerror_r(errno,errorbuffer,sizeof(errorbuffer));
		return errorbuffer;
	#elif defined(RUDIMENTS_HAVE_STRERROR)
		return strerror(errno);
	#else
		#error no strerror or anything like it
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
