// Copyright (c) 2002 David Muse
// See the COPYING file for more information

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

RUDIMENTS_INLINE bool hostentry::needsMutex() {
	#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
		return true;
	#else
		return false;
	#endif
}

RUDIMENTS_INLINE void hostentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
		hemutex=mutex;
	#endif
}

RUDIMENTS_INLINE bool hostentry::initialize(const char *hostname) {
	return initialize(hostname,NULL,0,0);
}

RUDIMENTS_INLINE bool hostentry::initialize(const char *address,
							int len, int type) {
	return initialize(NULL,address,len,type);
}
