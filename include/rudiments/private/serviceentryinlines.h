// Copyright (c) 2002 David Muse
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

RUDIMENTS_INLINE int serviceentry::getAliasList(const char *servicename,
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
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int serviceentry::getPort(const char *servicename,
						const char *protocol,
							int *port) {
	serviceentry	se;
	if (se.initialize(servicename,protocol)) {
		*port=se.getPort();
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int serviceentry::getName(int port,
						const char *protocol,
							char **name) {
	serviceentry	se;
	if (se.initialize(port,protocol)) {
		*name=strdup(se.getName());
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int serviceentry::getAliasList(int port,
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
		return 1;
	}
	return 0;
}

RUDIMENTS_INLINE int serviceentry::needsMutex() {
	#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
		return 1;
	#else
		return 0;
	#endif
}

RUDIMENTS_INLINE void serviceentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
		semutex=mutex;
	#endif
}

RUDIMENTS_INLINE int serviceentry::initialize(const char *servicename,
							const char *protocol) {
	return initialize(servicename,0,protocol);
}

RUDIMENTS_INLINE int serviceentry::initialize(int port, const char *protocol) {
	return initialize(NULL,port,protocol);
}
