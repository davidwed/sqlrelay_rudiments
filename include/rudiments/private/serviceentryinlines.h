// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRING_H
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE serviceentry::serviceentry() {
	se=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
		semutex=NULL;
	#endif
}

RUDIMENTS_INLINE serviceentry::~serviceentry() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *serviceentry::getName() const {
	return se->s_name;
}

RUDIMENTS_INLINE int serviceentry::getPort() const {
	return ntohs(se->s_port);
}

RUDIMENTS_INLINE char *serviceentry::getProtocol() const {
	return se->s_proto;
}

RUDIMENTS_INLINE char **serviceentry::getAliasList() const {
	return se->s_aliases;
}

RUDIMENTS_INLINE bool serviceentry::getAliasList(const char *servicename,
						const char *protocol,
							char ***aliaslist) {
	serviceentry	se;
	if (se.initialize(servicename,protocol)) {
		int	counter;
		for (counter=0; se.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(se.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool serviceentry::getPort(const char *servicename,
						const char *protocol,
							int *port) {
	serviceentry	se;
	if (se.initialize(servicename,protocol)) {
		*port=se.getPort();
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool serviceentry::getName(int port,
						const char *protocol,
							char **name) {
	serviceentry	se;
	if (se.initialize(port,protocol)) {
		*name=strdup(se.getName());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool serviceentry::getAliasList(int port,
						const char *protocol,
							char ***aliaslist) {
	serviceentry	se;
	if (se.initialize(port,protocol)) {
		int	counter;
		for (counter=0; se.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(se.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool serviceentry::needsMutex() {
	#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
		return true;
	#else
		return false;
	#endif
}

RUDIMENTS_INLINE void serviceentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
		semutex=mutex;
	#endif
}

RUDIMENTS_INLINE bool serviceentry::initialize(const char *servicename,
							const char *protocol) {
	return initialize(servicename,0,protocol);
}

RUDIMENTS_INLINE bool serviceentry::initialize(int port, const char *protocol) {
	return initialize(NULL,port,protocol);
}
