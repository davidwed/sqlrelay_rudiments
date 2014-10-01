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
