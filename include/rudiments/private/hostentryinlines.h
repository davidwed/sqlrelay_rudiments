// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE hostentry::hostentry() {
	he=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
		hemutex=NULL;
	#endif
}

RUDIMENTS_INLINE hostentry::~hostentry() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *hostentry::getName() const {
	return he->h_name;
}

RUDIMENTS_INLINE char **hostentry::getAliasList() const {
	return he->h_aliases;
}

RUDIMENTS_INLINE int hostentry::getAddressType() const {
	return he->h_addrtype;
}

RUDIMENTS_INLINE int hostentry::getAddressLength() const {
	return he->h_length;
}

RUDIMENTS_INLINE char **hostentry::getAddressList() const {
	return he->h_addr_list;
}

RUDIMENTS_INLINE int hostentry::getAliasList(const char *hostname,
							char ***aliaslist) {
	hostentry	he;
	if (he.initialize(hostname)) {
		int	counter;
		for (counter=0; he.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(he.getAliasList()[i]);
		}
		*aliaslist=alias;
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::getAddressType(const char *hostname,
							int *addresstype) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addresstype=he.getAddressType();
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::getAddressLength(const char *hostname,
							int *addresslength) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addresslength=he.getAddressLength();
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::getAddressList(const char *hostname,
							char ***addresslist) {
	hostentry	he;
	if (he.initialize(hostname)) {
		int	counter;
		for (counter=0; he.getAddressList()[counter]; counter++);
		char	**addr=new char *[counter+1];
		addr[counter]=NULL;
		for (int i=0; i<counter; i++) {
			addr[i]=strdup(he.getAddressList()[i]);
		}
		*addresslist=addr;
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::getAddressString(const char *hostname,
							int index,
							char **addressstring) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addressstring=strdup(he.getAddressString(index));
		return 1;
	}
	return 0;
}


RUDIMENTS_INLINE int hostentry::getName(const char *address,
						int len, int type,
						char **name) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*name=strdup(he.getName());
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::getAliasList(const char *address,
						int len, int type,
						char ***aliaslist) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		int	counter;
		for (counter=0; he.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(he.getAliasList()[i]);
		}
		*aliaslist=alias;
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::getAddressList(const char *address,
						int len, int type,
						char ***addresslist) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		int	counter;
		for (counter=0; he.getAddressList()[counter]; counter++);
		char	**addr=new char *[counter+1];
		addr[counter]=NULL;
		for (int i=0; i<counter; i++) {
			addr[i]=strdup(he.getAddressList()[i]);
		}
		*addresslist=addr;
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::getAddressString(const char *address,
						int len, int type, int index,
						char **addressstring) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*addressstring=strdup(he.getAddressString(index));
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int hostentry::needsMutex() {
	#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
		return 1;
	#else
		return 0;
	#endif
}

RUDIMENTS_INLINE void hostentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
		hemutex=mutex;
	#endif
}

RUDIMENTS_INLINE int hostentry::initialize(const char *hostname) {
	return initialize(hostname,NULL,0,0);
}

RUDIMENTS_INLINE int hostentry::initialize(const char *address,
							int len, int type) {
	return initialize(NULL,address,len,type);
}
