// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/shadowentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAVE_GETSPNAM) || defined(RUDIMENTS_HAVE_GETSPNAM_R)
	#define RUDIMENTS_HAVE_SHADOW
#endif

#ifdef RUDIMENTS_HAVE_SHADOW
	// for spwd, functions
	#include <shadow.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#ifdef RUDIMENTS_HAVE_SHADOW
class shadowentryprivate {
	friend class shadowentry;
	private:
		spwd	*_sp;
		#if defined(RUDIMENTS_HAVE_GETSPNAM_R)
			spwd	_spbuffer;
			char	*_buffer;
		#endif
};
#endif

// LAME: not in the class
#if defined(RUDIMENTS_HAVE_SHADOW) && !defined(RUDIMENTS_HAVE_GETSPNAM_R)
static mutex	*_spmutex;
#endif


shadowentry::shadowentry() {
	#ifdef RUDIMENTS_HAVE_SHADOW
		pvt=new shadowentryprivate;
		pvt->_sp=NULL;
		#ifdef RUDIMENTS_HAVE_GETSPNAM_R
			rawbuffer::zero(&pvt->_spbuffer,sizeof(pvt->_spbuffer));
			pvt->_buffer=NULL;
		#endif
	#endif
}

shadowentry::shadowentry(const shadowentry &s) {
	#ifdef RUDIMENTS_HAVE_SHADOW
		pvt=new shadowentryprivate;
		initialize(s.getName());
	#endif
}

shadowentry &shadowentry::operator=(const shadowentry &s) {
	#ifdef RUDIMENTS_HAVE_SHADOW
		pvt=new shadowentryprivate;
	#endif
	if (this!=&s) {
		initialize(s.getName());
	}
	return *this;
}

shadowentry::~shadowentry() {
	#ifdef RUDIMENTS_HAVE_SHADOW
		#ifdef RUDIMENTS_HAVE_GETSPNAM_R
			delete[] pvt->_buffer;
		#endif
		delete pvt;
	#endif
}

const char *shadowentry::getName() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return pvt->_sp->sp_namp;
	#else
		return NULL;
	#endif
}

const char *shadowentry::getEncryptedPassword() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return pvt->_sp->sp_pwdp;
	#else
		return NULL;
	#endif
}

long shadowentry::getLastChangeDate() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return pvt->_sp->sp_lstchg;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getDaysBeforeChangeAllowed() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return pvt->_sp->sp_min;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getDaysBeforeChangeRequired() const {
	#ifdef RUDIMENTS_HAVE_SHADOW
		return pvt->_sp->sp_max;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getDaysBeforeExpirationWarning() const {
	#if defined(RUDIMENTS_HAVE_SP_WARN)
		return pvt->_sp->sp_warn;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getDaysOfInactivityAllowed() const {
	#if defined(RUDIMENTS_HAVE_SP_INACT)
		return pvt->_sp->sp_inact;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getExpirationDate() const {
	#if defined(RUDIMENTS_HAVE_SP_EXPIRE)
		return pvt->_sp->sp_expire;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getFlag() const {
	#if defined(RUDIMENTS_HAVE_SP_FLAG)
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
	#if defined(RUDIMENTS_HAVE_SHADOW) && \
		!defined(RUDIMENTS_HAVE_GETSPNAM_R)
		_spmutex=mtx;
	#endif
}

bool shadowentry::initialize(const char *username) {

	#if defined(RUDIMENTS_HAVE_GETSPNAM_R)
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
		for (int32_t size=1024; size<MAXBUFFER; size=size+1024) {
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
	#elif defined(RUDIMENTS_HAVE_GETSPNAM) 
		pvt->_sp=NULL;
		return (!(_spmutex && !_spmutex->lock()) &&
			((pvt->_sp=getspnam(
				const_cast<char *>(username)))!=NULL) &&
			!(_spmutex && !_spmutex->unlock()));
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool shadowentry::getEncryptedPassword(const char *username, char **password) {
	#ifdef RUDIMENTS_HAVE_SHADOW
		shadowentry	sp;
		if (sp.initialize(username)) {
			*password=charstring::duplicate(
					sp.getEncryptedPassword());
			return true;
		}
	#endif
	return false;
}

bool shadowentry::getLastChangeDate(const char *username, long *lstchg) {
	#ifdef RUDIMENTS_HAVE_SHADOW
		shadowentry	sp;
		if (sp.initialize(username)) {
			*lstchg=sp.getLastChangeDate();
			return true;
		}
	#endif
	return false;
}

bool shadowentry::getDaysBeforeChangeAllowed(const char *username,
							int32_t *min) {
	#ifdef RUDIMENTS_HAVE_SHADOW
		shadowentry	sp;
		if (sp.initialize(username)) {
			*min=sp.getDaysBeforeChangeAllowed();
			return true;
		}
	#endif
	return false;
}

bool shadowentry::getDaysBeforeChangeRequired(const char *username,
							int32_t *max) {
	#ifdef RUDIMENTS_HAVE_SHADOW
		shadowentry	sp;
		if (sp.initialize(username)) {
			*max=sp.getDaysBeforeChangeRequired();
			return true;
		}
	#endif
	return false;
}

bool shadowentry::getDaysBeforeExpirationWarning(const char *username,
							int32_t *warn) {
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

bool shadowentry::getDaysOfInactivityAllowed(const char *username,
							int32_t *inact) {
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

bool shadowentry::getExpirationDate(const char *username, int32_t *expire) {
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

bool shadowentry::getFlag(const char *username, int32_t *flag) {
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

	#ifdef RUDIMENTS_HAVE_SP_WARN
	if (!pvt->_sp) {
		return;
	}
	#endif

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