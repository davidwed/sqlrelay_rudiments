// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE rpcentry::rpcentry() {
	re=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R)
		remutex=NULL;
	#endif
}

RUDIMENTS_INLINE rpcentry::~rpcentry() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *rpcentry::getName() const {
	return re->r_name;
}

RUDIMENTS_INLINE int rpcentry::getNumber() const {
	return re->r_number;
}

RUDIMENTS_INLINE char **rpcentry::getAliasList() const {
	return re->r_aliases;
}

RUDIMENTS_INLINE bool rpcentry::needsMutex() {
	#if !defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

RUDIMENTS_INLINE void rpcentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R)
		remutex=mutex;
	#endif
}

RUDIMENTS_INLINE bool rpcentry::initialize(const char *name) {
	return initialize(name,0);
}

RUDIMENTS_INLINE bool rpcentry::initialize(int number) {
	return initialize(NULL,number);
}
