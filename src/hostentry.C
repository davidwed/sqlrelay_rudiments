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

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__) && \
	(!defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R))
pthread_mutex_t	*hostentry::hemutex;
#endif


hostentry::hostentry() {
	he=NULL;
	#if defined(HAVE_GETHOSTBYNAME_R) && defined(HAVE_GETHOSTBYADDR_R)
		rawbuffer::zero(&hebuffer,sizeof(hebuffer));
		buffer=NULL;
	#endif
}

hostentry::hostentry(const hostentry &h) {
	initialize(h.getName());
}

hostentry &hostentry::operator=(const hostentry &h) {
	if (this!=&h) {
		initialize(h.getName());
	}
	return *this;
}

hostentry::~hostentry() {
	#if defined(HAVE_GETHOSTBYNAME_R) && defined(HAVE_GETHOSTBYADDR_R)
		delete[] buffer;
	#endif
}

const char *hostentry::getName() const {
	return he->h_name;
}

const char * const *hostentry::getAliasList() const {
	return he->h_aliases;
}

int hostentry::getAddressType() const {
	return he->h_addrtype;
}

int hostentry::getAddressLength() const {
	return he->h_length;
}

const char * const *hostentry::getAddressList() const {
	return he->h_addr_list;
}

#ifdef RUDIMENTS_HAS_THREADS
bool hostentry::needsMutex() {
	#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
		return true;
	#else
		return false;
	#endif
}

void hostentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
		hemutex=mutex;
	#endif
}
#endif

bool hostentry::initialize(const char *hostname) {
	return initialize(hostname,NULL,0,0);
}

bool hostentry::initialize(const char *address, int len, int type) {
	return initialize(NULL,address,len,type);
}

bool hostentry::initialize(const char *hostname, const char *address,
							int len, int type) {

	#if defined(HAVE_GETHOSTBYNAME_R) && defined(HAVE_GETHOSTBYADDR_R)
		if (he) {
			he=NULL;
			delete[] buffer;
			buffer=NULL;
		}
		// gethostbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int	errnop=0;
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			#if defined(HAVE_GETHOSTBYNAME_R_6) && \
					defined(HAVE_GETHOSTBYADDR_R_8)
			if (!((hostname)
				?(gethostbyname_r(hostname,&hebuffer,
						buffer,size,&he,&errnop))
				:(gethostbyaddr_r(address,len,type,&hebuffer,
						buffer,size,&he,&errnop)))) {
				return (he!=NULL);
			}
			#elif defined(HAVE_GETHOSTBYNAME_R_5) && \
					defined(HAVE_GETHOSTBYADDR_R_7)
			if ((hostname)
				?(he=gethostbyname_r(hostname,&hebuffer,
						buffer,size,&errnop))
				:(he=gethostbyaddr_r(address,len,type,&hebuffer,
						buffer,size,&errnop))) {
				return true;
			}
			#endif
			delete[] buffer;
			buffer=NULL;
			he=NULL;
			if (errnop!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		he=NULL;
		#ifdef RUDIMENTS_HAS_THREADS
		return (!(hemutex && pthread_mutex_lock(hemutex)) &&
			((he=((hostname)
				?gethostbyname(hostname)
				:gethostbyaddr(address,len,type)))!=NULL) &&
			!(hemutex && pthread_mutex_unlock(hemutex)));
		#else
		return ((he=((hostname)
				?gethostbyname(hostname)
				:gethostbyaddr(address,len,type)))!=NULL);
		#endif
	#endif
}

char *hostentry::getAddressString(int index) const {
	char	*address=new char[(getAddressLength()*4)+1];
	address[0]='\0';
	for (int byte=0; byte<getAddressLength(); byte++) {
		sprintf(address,"%s%d",address,getAddressList()[index][byte]);
		if (byte<getAddressLength()-1) {
			sprintf(address,"%s.",address);
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

	if (!he) {
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
