// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/rpcentry.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/rpcentryinlines.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R))
pthread_mutex_t	*rpcentry::remutex;
#endif

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
		return (((remutex)?!pthread_mutex_lock(remutex):true) &&
			((re=((rpcname)
				?getrpcbyname((char *)rpcname)
				:getrpcbynumber(number)))!=NULL) &&
			((remutex)?!pthread_mutex_unlock(remutex):true));
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
