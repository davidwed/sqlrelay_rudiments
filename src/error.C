// Copyright (c) 2005 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/error.h>

// for strerror
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <string.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

void error::clearError() {
	errno=0;
}

void error::setErrorNumber(int err) {
	errno=err;
}

int error::getErrorNumber() {
	return errno;
}

const char *error::getErrorString() {
	return strerror(errno);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
