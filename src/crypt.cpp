// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/crypt.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAVE_CRYPT_R) && !defined(__USE_GNU)
	#define __USE_GNU
#endif
#if defined(RUDIMENTS_HAVE_CRYPT_H)
	#include <crypt.h>
#elif defined(RUDIMENTS_HAVE_UNISTD_H)
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

// In some environments the includes above will request that NULL be redefined,
// even if it's already been defined.  In some of those environments it gets
// defined as ((void *)0).  If gcc < 2.8 is used then it will complain if you
// assign const char *a=((void *)0).  This redefines NULL yet again to avoid
// those issues.
#include <rudiments/null.h>

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_CRYPT_R)
static threadmutex	*_cryptmutex;
#endif

char *crypt::encrypt(const char *password, const char *salt) {
	#if defined(RUDIMENTS_HAVE_CRYPT_R)
		crypt_data	cd;
		rawbuffer::zero(&cd,sizeof(cd));
		char	*encryptedpassword=crypt_r(password,(salt)?salt:"",&cd);
		return (encryptedpassword)?
			charstring::duplicate(encryptedpassword):NULL;
	#elif defined(RUDIMENTS_HAVE_CRYPT)
		if (_cryptmutex && !_cryptmutex->lock()) {
			return NULL;
		}
		char	*encryptedpassword=::crypt(password,(salt)?salt:"");
		char	*retval=(encryptedpassword)?
				charstring::duplicate(encryptedpassword):NULL;
		if (_cryptmutex) {
			_cryptmutex->unlock();
		}
		return retval;
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

bool crypt::needsMutex() {
	#if defined(RUDIMENTS_HAVE_CRYPT_R)
		return false;
	#elif !defined(RUDIMENTS_HAVE_CRYPT)
		return true;
	#else
		return false;
	#endif
}

void crypt::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_CRYPT_R)
		_cryptmutex=mtx;
	#endif
}
