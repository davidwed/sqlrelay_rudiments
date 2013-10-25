// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/hostentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/stdio.h>

#include <rudiments/private/winsock.h>

// for ENOMEM
#ifdef RUDIMENTS_HAVE_ERRNO_H
	#include <errno.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif

#define MAXBUFFER	(32*1024)

class hostentryprivate {
	friend class hostentry;
	private:
		hostent	*_he;
		#if defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) && \
				defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
			hostent		_hebuffer;
			char		*_buffer;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) || \
	!defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R))
static threadmutex	*_hemutex;
#endif


hostentry::hostentry() {
	pvt=new hostentryprivate;
	winsock::initWinsock();
	pvt->_he=NULL;
	#if defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
		rawbuffer::zero(&pvt->_hebuffer,sizeof(pvt->_hebuffer));
		pvt->_buffer=NULL;
	#endif
}

hostentry::hostentry(const hostentry &h) {
	pvt=new hostentryprivate;
	winsock::initWinsock();
	initialize(h.getName());
}

hostentry &hostentry::operator=(const hostentry &h) {
	if (this!=&h) {
		initialize(h.getName());
	}
	return *this;
}

hostentry::~hostentry() {
	#if defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
		delete[] pvt->_buffer;
	#endif
	delete pvt;
}

const char *hostentry::getName() const {
	return pvt->_he->h_name;
}

const char * const *hostentry::getAliasList() const {
	return pvt->_he->h_aliases;
}

int32_t hostentry::getAddressType() const {
	return pvt->_he->h_addrtype;
}

int32_t hostentry::getAddressLength() const {
	return pvt->_he->h_length;
}

const char * const *hostentry::getAddressList() const {
	return pvt->_he->h_addr_list;
}

bool hostentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
		return true;
	#else
		return false;
	#endif
}

void hostentry::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
		_hemutex=mtx;
	#endif
}

bool hostentry::initialize(const char *hostname) {
	return initialize(hostname,NULL,0,0);
}

bool hostentry::initialize(const char *address, int32_t len, int32_t type) {
	return initialize(NULL,address,len,type);
}

bool hostentry::initialize(const char *hostname, const char *address,
						int32_t len, int32_t type) {

	if (!hostname && !address) {
		return false;
	}

	#if defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
		if (pvt->_he) {
			pvt->_he=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
		// gethostbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int32_t	errnop=0;
		for (int32_t size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R_6) && \
				defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R_8)
			if (!((hostname)
				?(gethostbyname_r(hostname,
						&pvt->_hebuffer,
						pvt->_buffer,size,
						&pvt->_he,&errnop))
				:(gethostbyaddr_r(address,len,type,
						&pvt->_hebuffer,
						pvt->_buffer,size,
						&pvt->_he,&errnop)))) {
				return (pvt->_he!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R_5) && \
				defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R_7)
			if ((hostname)
				?(pvt->_he=gethostbyname_r(hostname,
						&pvt->_hebuffer,
						pvt->_buffer,size,
						&errnop))
				:(pvt->_he=gethostbyaddr_r(address,len,type,
						&pvt->_hebuffer,
						pvt->_buffer,size,
						&errnop))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_he=NULL;
			if (errnop!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		pvt->_he=NULL;
		return (!(_hemutex && !_hemutex->lock()) &&
			((pvt->_he=((hostname)
				?gethostbyname(hostname)
				:gethostbyaddr(address,len,type)))!=NULL) &&
			!(_hemutex && !_hemutex->unlock()));
	#endif
}

char *hostentry::getAddressString(int32_t index) const {
	size_t	addresslen=(getAddressLength()*4)+1;
	char	*address=new char[addresslen];
	address[0]='\0';
	for (int32_t byte=0; byte<getAddressLength(); byte++) {
		charstring::append(address,
			(int64_t)(unsigned char)getAddressList()[index][byte]);
		if (byte<getAddressLength()-1) {
			charstring::append(address,".");
		}
	}
	return address;
}

bool hostentry::getAddressString(const char *hostname, char **addressstring) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addressstring=he.getAddressString(0);
		return true;
	}
	*addressstring=NULL;
	return false;
}

bool hostentry::getName(const char *address, int32_t len,
						int32_t type, char **name) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*name=charstring::duplicate(he.getName());
		return true;
	}
	*name=NULL;
	return false;
}

bool hostentry::getAddressString(const char *address,
					int32_t len, int32_t type,
					char **addressstring) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*addressstring=he.getAddressString(0);
		return true;
	}
	*addressstring=NULL;
	return false;
}
