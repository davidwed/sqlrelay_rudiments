// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

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

RUDIMENTS_INLINE int shadowentry::getEncryptedPassword(const char *username,
							char **password) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*password=strdup(sp.getEncryptedPassword());
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int shadowentry::getLastChangeDate(
						const char *username,
							long *lstchg) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*lstchg=sp.getLastChangeDate();
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int shadowentry::getDaysBeforeChangeAllowed(
						const char *username,
							int *min) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*min=sp.getDaysBeforeChangeAllowed();
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int shadowentry::getDaysBeforeChangeRequired(
						const char *username,
							int *max) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*max=sp.getDaysBeforeChangeRequired();
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int shadowentry::getDaysBeforeExpirationWarning(
						const char *username,
							int *warn) {
#ifdef HAVE_SP_WARN
	shadowentry	sp;
	if (sp.initialize(username)) {
		*warn=sp.getDaysBeforeExpirationWarning();
		return 1;
	}
	return 0;
#else
	*warn=-1;
	return 1;
#endif
}

RUDIMENTS_INLINE int shadowentry::getDaysOfInactivityAllowed(
						const char *username,
							int *inact) {
#ifdef HAVE_SP_INACT
	shadowentry	sp;
	if (sp.initialize(username)) {
		*inact=sp.getDaysOfInactivityAllowed();
		return 1;
	}
	return 0;
#else
	*inact=-1;
	return 1;
#endif
}

RUDIMENTS_INLINE int shadowentry::getExpirationDate(
						const char *username,
							int *expire) {
#ifdef HAVE_SP_EXPIRE
	shadowentry	sp;
	if (sp.initialize(username)) {
		*expire=sp.getExpirationDate();
		return 1;
	}
	return 0;
#else
	*expire=-1;
	return 1;
#endif
}

RUDIMENTS_INLINE int shadowentry::getFlag(const char *username,
							int *flag) {
#ifdef HAVE_SP_FLAG
	shadowentry	sp;
	if (sp.initialize(username)) {
		*flag=sp.getFlag();
		return 1;
	}
	return 0;
#else
	*flag=-1;
	return 1;
#endif
}

RUDIMENTS_INLINE int shadowentry::needsMutex() {
	#if !defined(HAVE_GETSPNAM_R)
		return 1;
	#else
		return 0;
	#endif
}

RUDIMENTS_INLINE void shadowentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETSPNAM_R)
		spmutex=mutex;
	#endif
}
