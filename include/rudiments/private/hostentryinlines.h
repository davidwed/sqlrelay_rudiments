// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

inline hostentry::hostentry() {
	he=NULL;
	buffer=NULL;
}

inline hostentry::~hostentry() {
	delete he;
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
