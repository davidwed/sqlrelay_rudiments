// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/crypt.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_CRYPT_R
	#ifndef __USE_GNU
		#define __USE_GNU
	#endif
#endif
#ifdef RUDIMENTS_HAVE_CRYPT_H
	#include <crypt.h>
#else
	#ifdef RUDIMENTS_HAVE_UNISTD_H
		#include <unistd.h>
	#endif
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_CRYPT_R)
static mutex	*_cryptmutex;
#endif

char *crypt::encrypt(const char *password, const char *salt) {
	#if defined(RUDIMENTS_HAVE_CRYPT_R)
		crypt_data	cd;
		rawbuffer::zero(&cd,sizeof(cd));
		char	*encryptedpassword=crypt_r(password,salt,&cd);
		return (encryptedpassword)?
			charstring::duplicate(encryptedpassword):NULL;
	#elif defined(RUDIMENTS_HAVE_CRYPT)
		if (_cryptmutex && !_cryptmutex->lock()) {
			return NULL;
		}
		char	*encryptedpassword=::crypt(password,salt);
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

void crypt::setMutex(mutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_CRYPT_R)
		_cryptmutex=mtx;
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
