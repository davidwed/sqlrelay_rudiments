// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/protocolentry.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYNUMBER_R))
pthread_mutex_t	*protocolentry::pemutex;
#endif

protocolentry::protocolentry() {
	pe=NULL;
	buffer=NULL;
	#if !defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYNUMBER_R)
		pemutex=NULL;
	#endif
}

protocolentry::~protocolentry() {
	delete[] buffer;
}

char *protocolentry::getName() const {
	return pe->p_name;
}

char **protocolentry::getAliasList() const {
	return pe->p_aliases;
}

int protocolentry::getNumber() const {
	return pe->p_proto;
}

bool protocolentry::needsMutex() {
	#if !defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

void protocolentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYNUMBER_R)
		pemutex=mutex;
	#endif
}

bool protocolentry::initialize(const char *protocolname) {
	return initialize(protocolname,0);
}

bool protocolentry::initialize(int number) {
	return initialize(NULL,number);
}

bool protocolentry::initialize(const char *protocolname, int number) {
	if (pe) {
		pe=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETPROTOBYNAME_R) && defined(HAVE_GETPROTOBYNUMBER_R)
		// getprotobyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((protocolname)
				?(getprotobyname_r(protocolname,&pebuffer,
							buffer,size,&pe))
				:(getprotobynumber_r(number,&pebuffer,
							buffer,size,&pe)))) {
				return (pe!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			pe=NULL;
			if (errno!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		pe=NULL;
		return (((pemutex)?!pthread_mutex_lock(pemutex):true) &&
			((pe=((protocolname)
				?getprotobyname(protocolname)
				:getprotobynumber(number)))!=NULL) &&
			((pemutex)?!pthread_mutex_unlock(pemutex):true));
	#endif
}

bool protocolentry::getAliasList(const char *protocolname, char ***aliaslist) {
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

bool protocolentry::getNumber(const char *protocolname, int *number) {
	protocolentry	pe;
	if (pe.initialize(protocolname)) {
		*number=pe.getNumber();
		return true;
	}
	return false;
}

bool protocolentry::getName(int number, char **name) {
	protocolentry	pe;
	if (pe.initialize(number)) {
		*name=strdup(pe.getName());
		return true;
	}
	return false;
}

bool protocolentry::getAliasList(int number, char ***aliaslist) {
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

void protocolentry::print() const {

	if (!pe) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
	printf("Number: %d\n",getNumber());
}
