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

class shadowentryprivate {
	friend class shadowentry;
	private:
		spwd	*_sp;
		#if defined(RUDIMENTS_HAVE_GETSPNAM_R)
			spwd	_spbuffer;
			char	*_buffer;
		#endif
};

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_GETSPNAM_R)
static mutex	*_spmutex;
#endif


shadowentry::shadowentry() {
	pvt=new shadowentryprivate;
	pvt->_sp=NULL;
	#ifdef RUDIMENTS_HAVE_GETSPNAM_R
		rawbuffer::zero(&pvt->_spbuffer,sizeof(pvt->_spbuffer));
		pvt->_buffer=NULL;
	#endif
}

shadowentry::shadowentry(const shadowentry &s) {
	pvt=new shadowentryprivate;
	initialize(s.getName());
}

shadowentry &shadowentry::operator=(const shadowentry &s) {
	pvt=new shadowentryprivate;
	if (this!=&s) {
		initialize(s.getName());
	}
	return *this;
}

shadowentry::~shadowentry() {
	#ifdef RUDIMENTS_HAVE_GETSPNAM_R
		delete[] pvt->_buffer;
	#endif
	delete pvt;
}

const char *shadowentry::getName() const {
	return pvt->_sp->sp_namp;
}

const char *shadowentry::getEncryptedPassword() const {
	return pvt->_sp->sp_pwdp;
}

long shadowentry::getLastChangeDate() const {
	return pvt->_sp->sp_lstchg;
}

int shadowentry::getDaysBeforeChangeAllowed() const {
	return pvt->_sp->sp_min;
}

int shadowentry::getDaysBeforeChangeRequired() const {
	return pvt->_sp->sp_max;
}

int shadowentry::getDaysBeforeExpirationWarning() const {
#ifdef RUDIMENTS_HAVE_SP_WARN
	return pvt->_sp->sp_warn;
#else
	return -1;
#endif
}

int shadowentry::getDaysOfInactivityAllowed() const {
#ifdef RUDIMENTS_HAVE_SP_INACT
	return pvt->_sp->sp_inact;
#else
	return -1;
#endif
}

int shadowentry::getExpirationDate() const {
#ifdef RUDIMENTS_HAVE_SP_EXPIRE
	return pvt->_sp->sp_expire;
#else
	return -1;
#endif
}

int shadowentry::getFlag() const {
#ifdef RUDIMENTS_HAVE_SP_FLAG
	return pvt->_sp->sp_flag;
#else
	return -1;
#endif
}

bool shadowentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETSPNAM_R)
		return true;
	#else
		return false;
	#endif
}

void shadowentry::setMutex(mutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETSPNAM_R)
		_spmutex=mtx;
	#endif
}

bool shadowentry::initialize(const char *username) {

	#ifdef RUDIMENTS_HAVE_GETSPNAM_R
		if (pvt->_sp) {
			pvt->_sp=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
		// getspnam_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETSPNAM_R_5)
			if (!getspnam_r(username,&pvt->_spbuffer,
					pvt->_buffer,size,&pvt->_sp)) {
				return (pvt->_sp!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETSPNAM_R_4)
			if ((pvt->_sp=getspnam_r(username,
					&pvt->_spbuffer,pvt->_buffer,size))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_sp=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		pvt->_sp=NULL;
		return (!(_spmutex && !_spmutex->lock()) &&
			((pvt->_sp=getspnam(
				const_cast<char *>(username)))!=NULL) &&
			!(_spmutex && !_spmutex->unlock()));
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
#ifdef RUDIMENTS_HAVE_SP_WARN
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
#ifdef RUDIMENTS_HAVE_SP_INACT
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
#ifdef RUDIMENTS_HAVE_SP_EXPIRE
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
#ifdef RUDIMENTS_HAVE_SP_FLAG
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

	if (!pvt->_sp) {
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
