// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRING_H
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE protocolentry::protocolentry() {
	pe=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYNUMBER_R)
		pemutex=NULL;
	#endif
}

RUDIMENTS_INLINE protocolentry::~protocolentry() {
	delete[] buffer;
}

RUDIMENTS_INLINE char *protocolentry::getName() const {
	return pe->p_name;
}

RUDIMENTS_INLINE char **protocolentry::getAliasList() const {
	return pe->p_aliases;
}

RUDIMENTS_INLINE int protocolentry::getNumber() const {
	return pe->p_proto;
}

RUDIMENTS_INLINE bool protocolentry::getAliasList(const char *protocolname,
							char ***aliaslist) {
	protocolentry	pe;
	if (pe.initialize(protocolname)) {
		int	counter;
		for (counter=0; pe.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(pe.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool protocolentry::getNumber(const char *protocolname,
								int *number) {
	protocolentry	pe;
	if (pe.initialize(protocolname)) {
		*number=pe.getNumber();
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool protocolentry::getName(int number, char **name) {
	protocolentry	pe;
	if (pe.initialize(number)) {
		*name=strdup(pe.getName());
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool protocolentry::getAliasList(int number,
							char ***aliaslist) {
	protocolentry	pe;
	if (pe.initialize(number)) {
		int	counter;
		for (counter=0; pe.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(pe.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

RUDIMENTS_INLINE bool protocolentry::needsMutex() {
	#if !defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

RUDIMENTS_INLINE void protocolentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYNUMBER_R)
		pemutex=mutex;
	#endif
}

RUDIMENTS_INLINE bool protocolentry::initialize(const char *protocolname) {
	return initialize(protocolname,0);
}

RUDIMENTS_INLINE bool protocolentry::initialize(int number) {
	return initialize(NULL,number);
}
