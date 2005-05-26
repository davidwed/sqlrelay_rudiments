// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/shadowentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && \
	defined(__GNUC__) && !defined(HAVE_GETSPNAM_R)
mutex	*shadowentry::spmutex;
#endif


shadowentry::shadowentry() {
	sp=NULL;
	#ifdef HAVE_GETSPNAM_R
		rawbuffer::zero(&spbuffer,sizeof(spbuffer));
		buffer=NULL;
	#endif
}

shadowentry::shadowentry(const shadowentry &s) {
	initialize(s.getName());
}

shadowentry &shadowentry::operator=(const shadowentry &s) {
	if (this!=&s) {
		initialize(s.getName());
	}
	return *this;
}

shadowentry::~shadowentry() {
	#ifdef HAVE_GETSPNAM_R
		delete[] buffer;
	#endif
}

const char *shadowentry::getName() const {
	return sp->sp_namp;
}

const char *shadowentry::getEncryptedPassword() const {
	return sp->sp_pwdp;
}

long shadowentry::getLastChangeDate() const {
	return sp->sp_lstchg;
}

int shadowentry::getDaysBeforeChangeAllowed() const {
	return sp->sp_min;
}

int shadowentry::getDaysBeforeChangeRequired() const {
	return sp->sp_max;
}

int shadowentry::getDaysBeforeExpirationWarning() const {
#ifdef HAVE_SP_WARN
	return sp->sp_warn;
#else
	return -1;
#endif
}

int shadowentry::getDaysOfInactivityAllowed() const {
#ifdef HAVE_SP_INACT
	return sp->sp_inact;
#else
	return -1;
#endif
}

int shadowentry::getExpirationDate() const {
#ifdef HAVE_SP_EXPIRE
	return sp->sp_expire;
#else
	return -1;
#endif
}

int shadowentry::getFlag() const {
#ifdef HAVE_SP_FLAG
	return sp->sp_flag;
#else
	return -1;
#endif
}

#ifdef RUDIMENTS_HAS_THREADS
bool shadowentry::needsMutex() {
	#if !defined(HAVE_GETSPNAM_R)
		return true;
	#else
		return false;
	#endif
}

void shadowentry::setMutex(mutex *mtx) {
	#if !defined(HAVE_GETSPNAM_R)
		spmutex=mtx;
	#endif
}
#endif

bool shadowentry::initialize(const char *username) {

	#ifdef HAVE_GETSPNAM_R
		if (sp) {
			sp=NULL;
			delete[] buffer;
			buffer=NULL;
		}
		// getspnam_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			#if defined(HAVE_GETSPNAM_R_5)
			if (!getspnam_r(username,&spbuffer,buffer,size,&sp)) {
				return (sp!=NULL);
			}
			#elif defined(HAVE_GETSPNAM_R_4)
			if ((sp=getspnam_r(username,&spbuffer,buffer,size))) {
				return true;
			}
			#endif
			delete[] buffer;
			buffer=NULL;
			sp=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		sp=NULL;
		#ifdef RUDIMENTS_HAS_THREADS
		return (!(spmutex && !spmutex->lock()) &&
			((sp=getspnam(const_cast<char *>(username)))!=NULL) &&
			!(spmutex && !spmutex->unlock()));
		#else
		return ((sp=getspnam(const_cast<char *>(username)))!=NULL);
		#endif
	#endif
}

bool shadowentry::getEncryptedPassword(const char *username, char **password) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*password=charstring::duplicate(sp.getEncryptedPassword());
		return true;
	}
	return false;
}

bool shadowentry::getLastChangeDate(const char *username, long *lstchg) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*lstchg=sp.getLastChangeDate();
		return true;
	}
	return false;
}

bool shadowentry::getDaysBeforeChangeAllowed(const char *username, int *min) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*min=sp.getDaysBeforeChangeAllowed();
		return true;
	}
	return false;
}

bool shadowentry::getDaysBeforeChangeRequired(const char *username, int *max) {
	shadowentry	sp;
	if (sp.initialize(username)) {
		*max=sp.getDaysBeforeChangeRequired();
		return true;
	}
	return false;
}

bool shadowentry::getDaysBeforeExpirationWarning(const char *username,
								int *warn) {
#ifdef HAVE_SP_WARN
	shadowentry	sp;
	if (sp.initialize(username)) {
		*warn=sp.getDaysBeforeExpirationWarning();
		return true;
	}
	return false;
#else
	*warn=-1;
	return true;
#endif
}

bool shadowentry::getDaysOfInactivityAllowed(const char *username, int *inact) {
#ifdef HAVE_SP_INACT
	shadowentry	sp;
	if (sp.initialize(username)) {
		*inact=sp.getDaysOfInactivityAllowed();
		return true;
	}
	return false;
#else
	*inact=-1;
	return true;
#endif
}

bool shadowentry::getExpirationDate(const char *username, int *expire) {
#ifdef HAVE_SP_EXPIRE
	shadowentry	sp;
	if (sp.initialize(username)) {
		*expire=sp.getExpirationDate();
		return true;
	}
	return false;
#else
	*expire=-1;
	return true;
#endif
}

bool shadowentry::getFlag(const char *username, int *flag) {
#ifdef HAVE_SP_FLAG
	shadowentry	sp;
	if (sp.initialize(username)) {
		*flag=sp.getFlag();
		return true;
	}
	return false;
#else
	*flag=-1;
	return true;
#endif
}

void shadowentry::print() const {

	if (!sp) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Encrypted Password: %s\n",getEncryptedPassword());
	printf("Last Change: %ld\n",getLastChangeDate());
	printf("Days Before Change Allowed: %d\n",
				getDaysBeforeChangeAllowed());
	printf("Days Before Change Required: %d\n",
				getDaysBeforeChangeRequired());
	printf("Days Before Expiration Warning: %d\n",
				getDaysBeforeExpirationWarning());
	printf("Days Of Inactivity Allowed: %d\n",
				getDaysOfInactivityAllowed());
	printf("Expiration Date: %d\n",getExpirationDate());
	printf("Flag: %d\n",getFlag());
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
