// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/hostentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>

// for ENOMEM
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

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
static mutex	*_hemutex;
#endif


hostentry::hostentry() {
	pvt=new hostentryprivate;
	pvt->_he=NULL;
	#if defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
		rawbuffer::zero(&pvt->_hebuffer,sizeof(pvt->_hebuffer));
		pvt->_buffer=NULL;
	#endif
}

hostentry::hostentry(const hostentry &h) {
	pvt=new hostentryprivate;
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

int hostentry::getAddressType() const {
	return pvt->_he->h_addrtype;
}

int hostentry::getAddressLength() const {
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

void hostentry::setMutex(mutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETHOSTBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETHOSTBYADDR_R)
		_hemutex=mtx;
	#endif
}

bool hostentry::initialize(const char *hostname) {
	return initialize(hostname,NULL,0,0);
}

bool hostentry::initialize(const char *address, int len, int type) {
	return initialize(NULL,address,len,type);
}

bool hostentry::initialize(const char *hostname, const char *address,
							int len, int type) {

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
		int	errnop=0;
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
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

char *hostentry::getAddressString(int index) const {
	size_t	addresslen=(getAddressLength()*4)+1;
	char	*address=new char[addresslen];
	address[0]='\0';
	for (int byte=0; byte<getAddressLength(); byte++) {
		snprintf(address,addresslen,
			"%s%d",address,getAddressList()[index][byte]);
		if (byte<getAddressLength()-1) {
			snprintf(address,addresslen,"%s.",address);
		}
	}
	return address;
}

bool hostentry::getAliasList(const char *hostname, char ***aliaslist) {
	hostentry	he;
	if (he.initialize(hostname)) {
		int	counter;
		for (counter=0; he.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(he.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

bool hostentry::getAddressType(const char *hostname, int *addresstype) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addresstype=he.getAddressType();
		return true;
	}
	return false;
}

bool hostentry::getAddressLength(const char *hostname, int *addresslength) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addresslength=he.getAddressLength();
		return true;
	}
	return false;
}

bool hostentry::getAddressList(const char *hostname, char ***addresslist) {
	hostentry	he;
	if (he.initialize(hostname)) {
		int	counter;
		for (counter=0; he.getAddressList()[counter]; counter++);
		char	**addr=new char *[counter+1];
		addr[counter]=NULL;
		for (int i=0; i<counter; i++) {
			addr[i]=charstring::duplicate(he.getAddressList()[i]);
		}
		*addresslist=addr;
		return true;
	}
	return false;
}

bool hostentry::getAddressString(const char *hostname, int index,
							char **addressstring) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addressstring=he.getAddressString(index);
		return true;
	}
	return false;
}


bool hostentry::getName(const char *address, int len, int type, char **name) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*name=charstring::duplicate(he.getName());
		return true;
	}
	return false;
}

bool hostentry::getAliasList(const char *address, int len, int type,
							char ***aliaslist) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		int	counter;
		for (counter=0; he.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(he.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

bool hostentry::getAddressList(const char *address, int len, int type,
							char ***addresslist) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		int	counter;
		for (counter=0; he.getAddressList()[counter]; counter++);
		char	**addr=new char *[counter+1];
		addr[counter]=NULL;
		for (int i=0; i<counter; i++) {
			addr[i]=charstring::duplicate(he.getAddressList()[i]);
		}
		*addresslist=addr;
		return true;
	}
	return false;
}

bool hostentry::getAddressString(const char *address, int len, int type,
					int index, char **addressstring) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*addressstring=he.getAddressString(index);
		return true;
	}
	return false;
}

void hostentry::print() const {

	if (!pvt->_he) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
	printf("Address type: %d\n",getAddressType());
	printf("Address length: %d\n",getAddressLength());
	printf("Address list:\n");
	for (int i=0; getAddressList()[i]; i++) {
		char	*addr=getAddressString(i);
		printf("	%s\n",addr);
		delete[] addr;
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
