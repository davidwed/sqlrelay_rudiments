// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/shadowentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAVE_GETSPNAM) || defined(RUDIMENTS_HAVE_GETSPNAM_R)
	#define RUDIMENTS_HAVE_SHADOW
#endif

#ifdef RUDIMENTS_HAVE_SHADOW
	// for spwd, functions
	#include <shadow.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#define MAXBUFFER	(32*1024)

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
static threadmutex	*_spmutex;
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
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_WARN)
		return pvt->_sp->sp_warn;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getDaysOfInactivityAllowed() const {
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_INACT)
		return pvt->_sp->sp_inact;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getExpirationDate() const {
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_EXPIRE)
		return pvt->_sp->sp_expire;
	#else
		return -1;
	#endif
}

int32_t shadowentry::getFlag() const {
	#if defined(RUDIMENTS_HAVE_SHADOW) && defined(RUDIMENTS_HAVE_SP_FLAG)
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

void shadowentry::setMutex(threadmutex *mtx) {
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
