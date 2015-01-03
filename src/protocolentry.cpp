// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/protocolentry.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#include <rudiments/private/winsock.h>

// for protoent, functions
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#define MAXBUFFER	(32*1024)

class protocolentryprivate {
	friend class protocolentry;
	private:
		protoent	*_pe;
		#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
				defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
			protoent	_pebuffer;
			char		*_buffer;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
	!defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R))
static threadmutex	*_pemutex;
#endif

protocolentry::protocolentry() {
	pvt=new protocolentryprivate;
	winsock::initWinsock();
	pvt->_pe=NULL;
	#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		bytestring::zero(&pvt->_pebuffer,sizeof(pvt->_pebuffer));
		pvt->_buffer=NULL;
	#endif
}

protocolentry::protocolentry(const protocolentry &p) {
	pvt=new protocolentryprivate;
	winsock::initWinsock();
	initialize(p.getName());
}

protocolentry &protocolentry::operator=(const protocolentry &p) {
	if (this!=&p) {
		initialize(p.getName());
	}
	return *this;
}

protocolentry::~protocolentry() {
	#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		delete[] pvt->_buffer;
	#endif
	delete pvt;
}

const char *protocolentry::getName() const {
	return (pvt->_pe)?pvt->_pe->p_name:NULL;
}

const char * const *protocolentry::getAliasList() const {
	return (pvt->_pe)?pvt->_pe->p_aliases:NULL;
}

int32_t protocolentry::getNumber() const {
	return (pvt->_pe)?pvt->_pe->p_proto:-1;
}

bool protocolentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

void protocolentry::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		_pemutex=mtx;
	#endif
}

bool protocolentry::initialize(const char *protocolname) {
	return initialize(protocolname,-1);
}

bool protocolentry::initialize(int32_t number) {
	return initialize(NULL,number);
}

bool protocolentry::initialize(const char *protocolname, int32_t number) {

	pvt->_pe=NULL;
	if (!protocolname && number==-1) {
		return false;
	}

	#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		delete[] pvt->_buffer;
		pvt->_buffer=NULL;
		// getprotobyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int32_t size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R_5) && \
				defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_5)
			if (!((protocolname)
				?(getprotobyname_r(protocolname,
							&pvt->_pebuffer,
							pvt->_buffer,size,
							&pvt->_pe))
				:(getprotobynumber_r(number,
							&pvt->_pebuffer,
							pvt->_buffer,size,
							&pvt->_pe)))) {
				return (pvt->_pe!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R_4) && \
				defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_4)
			if ((protocolname)
				?(pvt->_pe=getprotobyname_r(protocolname,
							&pvt->_pebuffer,
							pvt->_buffer,size))
				:(pvt->_pe=getprotobynumber_r(number,
							&pvt->_pebuffer,
							pvt->_buffer,size))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_pe=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		return (!(_pemutex && !_pemutex->lock())) &&
			((pvt->_pe=((protocolname)
				?getprotobyname(protocolname)
				:getprotobynumber(number)))!=NULL) &&
			!(_pemutex && !_pemutex->unlock());
	#endif
}

int32_t protocolentry::getNumber(const char *protocolname) {
	protocolentry	pe;
	return (pe.initialize(protocolname))?pe.getNumber():-1;
}

char *protocolentry::getName(int32_t number) {
	protocolentry	pe;
	return (pe.initialize(number))?
			charstring::duplicate(pe.getName()):NULL;
}
