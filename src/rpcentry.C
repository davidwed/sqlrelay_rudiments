// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/rpcentry.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/rpcentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#ifdef HAVE_RPCENT_H
	#include <rpc/rpcent.h>
#endif

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R))
pthread_mutex_t	*rpcentry::remutex;
#endif

int rpcentry::initialize(const char *rpcname, int number) {
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
				return re!=NULL;
			}
			delete[] buffer;
			buffer=NULL;
			re=NULL;
			if (errnop!=ENOMEM) {
				return 0;
			}
		}
		return 0;
	#else
		re=NULL;
		return (((remutex)?!pthread_mutex_lock(remutex):1) &&
			((re=((rpcname)
				?getrpcbyname(rpcname)
				:getrpcbynumber(number)))!=NULL) &&
			((remutex)?!pthread_mutex_unlock(remutex):1));
	#endif
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
