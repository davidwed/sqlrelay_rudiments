// Copyright (c) 2005 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/error.h>
#include <rudiments/charstring.h>

#include <stdio.h>

// to get the more portable XSI-compliant strerror_r rather than the GNU version
#undef __USE_GNU

// to avoid __THROW errors on some systems (fedora 2)
#undef __REDIRECT

// for strerror (or similar)
#ifdef RUDIMENTS_HAVE_STRINGS_H
	#include <strings.h>
#endif
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
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
		defined(RUDIMENTS_HAVE_XSI_STRERROR_R) || \
		defined(RUDIMENTS_HAVE_GNU_STRERROR_R)

		int32_t	errornumber=getErrorNumber();

		for (size_t size=256; size<=1024; size=size+256) {

			char	*buffer=new char[size];

			#if defined(RUDIMENTS_HAVE_STRERROR_S)
			errno_t	result=strerror_s(buffer,size,errornumber);
			if (!result) {
				return buffer;
			} else if (result!=ERANGE) {
				break;
			}
			setErrorNumber(errornumber);
			#elif defined(RUDIMENTS_HAVE_XSI_STRERROR_R)
			int	result=strerror_r(errornumber,buffer,size);
			if (!result) {
				return buffer;
			} else if (result==EINVAL) {
				// strerror_r on older freebsd, doesn't like it
				// if errornumber is 0.  It returns EINVAL,
				// sets the error number to EINVAL, and
				// doesn't fill the buffer.  Emulate the
				// behavior of strerror(0) by setting the
				// buffer to "Unknown error: 0" and resetting
				// the error number.
				charstring::copy(buffer,"Unknown error: 0");
				setErrorNumber(errornumber);
				return buffer;
			} else if (getErrorNumber()!=ERANGE) {
				break;
			}
			setErrorNumber(errornumber);
			#elif defined(RUDIMENTS_HAVE_GNU_STRERROR_R)
			char	*result=strerror_r(errornumber,buffer,size);
			if (getErrorNumber()==ERANGE) {
				setErrorNumber(errornumber);
			} else {
				return result;
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

void error::clearNativeError() {
	#if defined(RUDIMENTS_HAVE_GETLASTERROR)
		SetLastError(0);
	#else
		clearError();
	#endif
}

void error::setNativeErrorNumber(int32_t err) {
	#if defined(RUDIMENTS_HAVE_GETLASTERROR)
		SetLastError(err);
	#else
		clearError();
	#endif
}

int32_t error::getNativeErrorNumber() {
	#if defined(RUDIMENTS_HAVE_GETLASTERROR)
		return GetLastError();
	#else
		return getErrorNumber();
	#endif
}

char *error::getNativeErrorString() {
	#if defined(RUDIMENTS_HAVE_GETLASTERROR)
		char	*nativeerrorstring=NULL;
		LPVOID	errorstring=NULL;
		DWORD	errornumber=GetLastError();

		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
				FORMAT_MESSAGE_FROM_SYSTEM|
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,errornumber,
				MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				(LPTSTR)&errorstring,0,NULL)) {
			nativeerrorstring=charstring::duplicate(
						(char *)errorstring);
		}
		LocalFree(errorstring);
		return nativeerrorstring;
	#else
		return getErrorString();
	#endif
}
