// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/serviceentry.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/serviceentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R))
pthread_mutex_t	*serviceentry::semutex;
#endif

bool serviceentry::initialize(const char *servicename, int port,
						const char *protocol) {

	if (se) {
		se=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETSERVBYNAME_R) && defined(HAVE_GETSERVBYPORT_R)
		// getservbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((servicename)
				?(getservbyname_r(servicename,protocol,
							&sebuffer,
							buffer,size,&se))
				:(getservbyport_r(htons(port),protocol,
							&sebuffer,
							buffer,size,&se)))) {
				return (se!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			se=NULL;
			if (errno!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		se=NULL;
		return (((semutex)?!pthread_mutex_lock(semutex):true) &&
			((se=((servicename)
				?getservbyname(servicename,protocol)
				:getservbyport(htons(port),protocol)))!=NULL) &&
			((semutex)?!pthread_mutex_unlock(semutex):true));
	#endif
}

void serviceentry::print() const {

	if (!se) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Port: %d\n",getPort());
	printf("Protocol: %s\n",getProtocol());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
}
