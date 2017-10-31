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

		int32_t	errornumber=getErrorNumber();

		for (size_t size=256; size<=1024; size=size+256) {

			char	*buffer=new char[size];

			#if defined(RUDIMENTS_HAVE_STRERROR_S)
			errno_t	result=strerror_s(buffer,size,errornumber);
			if (!result) {
				return buffer;
			} else if (result!=ERANGE) {
				setErrorNumber(errornumber);
				break;
			}
			#elif defined(RUDIMENTS_HAVE_STRERROR_R)

			// There are 2 versions of strerror_r.  The XSI version
			// returns an int (success/fail) and the GNU version
			// returns char * (the error).  Platforms like redhat 9
			// only support the GNU version.
			//
			// It's difficult to autodetect a return value at
			// configure time, so we'll deal with it here.
			//
			// Also, the GNU version returns the error, but doesn't
			// fill the buffer at all!  In that case, the return
			// value is the only thing we can trust to contain the
			// error.
			int	result=reinterpret_cast<int>(
					strerror_r(errornumber,buffer,size));
			if (result!=-1) {
				if (!result) {
					return buffer;
				}
				char	*charresult=
					reinterpret_cast<char *>(result);
				if (charresult!=buffer) {
					delete[] buffer;
				}
				return charresult;
			} else {
				if (getErrorNumber()!=ERANGE) {
					setErrorNumber(errornumber);
					break;
				}
				setErrorNumber(errornumber);
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
