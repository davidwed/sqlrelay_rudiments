// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

inline hostentry::hostentry() {
	he=NULL;
	buffer=NULL;
}

inline hostentry::~hostentry() {
	delete[] buffer;
}

inline char *hostentry::getName() const {
	return he->h_name;
}

inline char **hostentry::getAliasList() const {
	return he->h_aliases;
}

inline int hostentry::getAddressType() const {
	return he->h_addrtype;
}

inline int hostentry::getAddressLength() const {
	return he->h_length;
}

inline char **hostentry::getAddressList() const {
	return he->h_addr_list;
}

inline int hostentry::getAliasList(const char *hostname, char ***aliaslist) {
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

inline int hostentry::getAddressType(const char *hostname, int *addresstype) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addresstype=he.getAddressType();
		return 1;
	}
	return 0;
}

inline int hostentry::getAddressLength(const char *hostname,
						int *addresslength) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addresslength=he.getAddressLength();
		return 1;
	}
	return 0;
}

inline int hostentry::getAddressList(const char *hostname,
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

inline int hostentry::getAddressString(const char *hostname, int index,
						char **addressstring) {
	hostentry	he;
	if (he.initialize(hostname)) {
		*addressstring=strdup(he.getAddressString(index));
		return 1;
	}
	return 0;
}


inline int hostentry::getName(const char *address, int len, int type,
						char **name) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*name=strdup(he.getName());
		return 1;
	}
	return 0;
}

inline int hostentry::getAliasList(const char *address, int len, int type,
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

inline int hostentry::getAddressList(const char *address, int len, int type,
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

inline int hostentry::getAddressString(const char *address, int len, int type,
						int index,
						char **addressstring) {
	hostentry	he;
	if (he.initialize(address,len,type)) {
		*addressstring=strdup(he.getAddressString(index));
		return 1;
	}
	return 0;
}
