// Copyright (c) 2005 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/error.h>
#include <rudiments/charstring.h>

#include <stdio.h>

// to get the more portable XSI-compliant strerror_r rather than the GNU version
#undef __USE_GNU

// for strerror (or similar)
#ifdef RUDIMENTS_HAVE_STRINGS_H
	#include <strings.h>
#endif
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

void error::clearError() {
	errno=0;
}

void error::setErrorNumber(int32_t err) {
	errno=err;
}

int32_t error::getErrorNumber() {
	// This is here for an odd reason.  Apparently, in some debugging
	// environments, if you set a breakpoint here, gdb will segfault if you
	// run "print errno".  You can safely "print errornumber" though.
	int32_t	errornumber=errno;
	return errornumber;
}

char *error::getErrorString() {
	#if defined(RUDIMENTS_HAVE_STRERROR_S) || \
		defined(RUDIMENTS_HAVE_STRERROR_R)

		for (size_t size=256; size<=1024; size=size+256) {

			char	*buffer=new char[size];

			#if defined(RUDIMENTS_HAVE_STRERROR_S)
			errno_t	result=strerror_s(buffer,size,errno);
			if (!result) {
				return buffer;
			} else if (result!=ERANGE) {
				break;
			}
			#elif defined(RUDIMENTS_HAVE_STRERROR_R)
			if (!strerror_r(errno,buffer,size)) {
				return buffer;
			} else if (getErrorNumber()!=ERANGE) {
				break;
			}
			#endif

			delete[] buffer;
		}

		return NULL;
	#elif defined(RUDIMENTS_HAVE_STRERROR)
		return charstring::duplicate(strerror(errno));
	#else
		#error no strerror or anything like it
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
