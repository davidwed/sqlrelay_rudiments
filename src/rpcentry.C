// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/rpcentry.h>

// Some systems (notably cygwin 1.5.7-1) define getrpcbyname and getrpcbynumber
// in their header files but then either don't implement them or don't export
// the symbols.
#if (defined(HAVE_GETRPCBYNAME_R) || defined(HAVE_GETRPCBYNAME)) \
	&& (defined(HAVE_GETRPCBYNUMBER_R) || defined(HAVE_GETRPCBYNUMBER)) \

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__) && \
	(!defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R))
pthread_mutex_t	*rpcentry::remutex;
#endif


rpcentry::rpcentry() {
	re=NULL;
	buffer=NULL;
}

rpcentry::~rpcentry() {
	delete[] buffer;
}

char *rpcentry::getName() const {
	return re->r_name;
}

int rpcentry::getNumber() const {
	return re->r_number;
}

char **rpcentry::getAliasList() const {
	return re->r_aliases;
}

#ifdef RUDIMENTS_HAS_THREADS
bool rpcentry::needsMutex() {
	#if !defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

void rpcentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R)
		remutex=mutex;
	#endif
}
#endif

bool rpcentry::initialize(const char *name) {
	return initialize(name,0);
}

bool rpcentry::initialize(int number) {
	return initialize(NULL,number);
}

bool rpcentry::initialize(const char *rpcname, int number) {
	if (re) {
		re=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETRPCBYNAME_R) && defined(HAVE_GETRPCBYNUMBER_R)
		// getrpcbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		int	errnop;
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((rpcname)
				?(getrpcbyname_r(rpcname,&rebuffer,
							buffer,size,&re))
				:(getrpcbynumber_r(number,&rebuffer,
							buffer,size,&re)))) {
				return (re!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			re=NULL;
			if (errnop!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		re=NULL;
#ifdef RUDIMENTS_HAS_THREADS
		return (((remutex)?!pthread_mutex_lock(remutex):true) &&
			((re=((rpcname)
				?getrpcbyname((char *)rpcname)
				:getrpcbynumber(number)))!=NULL) &&
			((remutex)?!pthread_mutex_unlock(remutex):true));
#else
		return ((re=((rpcname)
				?getrpcbyname((char *)rpcname)
				:getrpcbynumber(number)))!=NULL);
#endif
	#endif
}

bool rpcentry::getAliasList(const char *name, char ***aliaslist) {
	rpcentry	re;
	if (re.initialize(name)) {
		int	counter;
		for (counter=0; re.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(re.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

bool rpcentry::getName(int number, char **name) {
	rpcentry	re;
	if (re.initialize(number)) {
		*name=strdup(re.getName());
		return true;
	}
	return false;
}

bool rpcentry::getAliasList(int number, char ***aliaslist) {
	rpcentry	re;
	if (re.initialize(number)) {
		int	counter;
		for (counter=0; re.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=strdup(re.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

void rpcentry::print() const {

	if (!re) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Number: %d\n",getNumber());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
}

#endif
