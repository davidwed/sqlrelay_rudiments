// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE shadowentry::shadowentry() {
	sp=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETSPNAM_R)
		spmutex=NULL;
	#endif
}

RUDIMENTS_INLINE shadowentry::~shadowentry() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *shadowentry::getName() const {
	return sp->sp_namp;
}

RUDIMENTS_INLINE char *shadowentry::getEncryptedPassword() const {
	return sp->sp_pwdp;
}

RUDIMENTS_INLINE long shadowentry::getLastChangeDate() const {
	return sp->sp_lstchg;
}

RUDIMENTS_INLINE int shadowentry::getDaysBeforeChangeAllowed() const {
	return sp->sp_min;
}

RUDIMENTS_INLINE int shadowentry::getDaysBeforeChangeRequired() const {
	return sp->sp_max;
}

RUDIMENTS_INLINE int shadowentry::getDaysBeforeExpirationWarning() const {
#ifdef HAVE_SP_WARN
	return sp->sp_warn;
#else
	return -1;
#endif
}

RUDIMENTS_INLINE int shadowentry::getDaysOfInactivityAllowed() const {
#ifdef HAVE_SP_INACT
	return sp->sp_inact;
#else
	return -1;
#endif
}

RUDIMENTS_INLINE int shadowentry::getExpirationDate() const {
#ifdef HAVE_SP_EXPIRE
	return sp->sp_expire;
#else
	return -1;
#endif
}

RUDIMENTS_INLINE int shadowentry::getFlag() const {
#ifdef HAVE_SP_FLAG
	return sp->sp_flag;
#else
	return -1;
#endif
}

RUDIMENTS_INLINE bool shadowentry::needsMutex() {
	#if !defined(HAVE_GETSPNAM_R)
		return true;
	#else
		return false;
	#endif
}

RUDIMENTS_INLINE void shadowentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETSPNAM_R)
		spmutex=mutex;
	#endif
}
