// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/crypt.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>

#ifdef HAVE_CRYPT_R
#define __USE_GNU
#endif
#ifdef HAVE_CRYPT_H
#include <crypt.h>
#else
#include <unistd.h>
#endif
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__)
pthread_mutex_t	*crypt::cryptmutex;
#endif

char *crypt::encrypt(const char *password, const char *salt) {
	#ifdef HAVE_CRYPT_R
		crypt_data	cd;
		rawbuffer::zero(&cd,sizeof(cd));
		char	*encryptedpassword=crypt_r(password,salt,&cd);
		return (encryptedpassword)?
			charstring::duplicate(encryptedpassword):NULL;
	#else
		#ifdef RUDIMENTS_HAS_THREADS
		if (cryptmutex && pthread_mutex_lock(cryptmutex)) {
			return NULL;
		}
		#endif
		char	*encryptedpassword=::crypt(password,salt);
		char	*retval=(encryptedpassword)?
				charstring::duplicate(encryptedpassword):NULL;
		#ifdef RUDIMENTS_HAS_THREADS
		if (cryptmutex) {
			pthread_mutex_unlock(cryptmutex);
		}
		#endif
		return retval;
	#endif
}

#ifdef RUDIMENTS_HAS_THREADS
bool crypt::needsMutex() {
	#if !defined(HAVE_CRYPT_R)
		return true;
	#else
		return false;
	#endif
}

void crypt::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_CRYPT_R)
		cryptmutex=mutex;
	#endif
}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
