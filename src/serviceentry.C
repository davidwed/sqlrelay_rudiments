// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/serviceentry.h>
#include <rudiments/charstring.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__) && \
	(!defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R))
pthread_mutex_t	*serviceentry::semutex;
#endif


serviceentry::serviceentry() {
	se=NULL;
	buffer=NULL;
}

serviceentry::~serviceentry() {
	delete[] buffer;
}

char *serviceentry::getName() const {
	return se->s_name;
}

int serviceentry::getPort() const {
	return ntohs(se->s_port);
}

char *serviceentry::getProtocol() const {
	return se->s_proto;
}

char **serviceentry::getAliasList() const {
	return se->s_aliases;
}

#ifdef RUDIMENTS_HAS_THREADS
bool serviceentry::needsMutex() {
	#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
		return true;
	#else
		return false;
	#endif
}

void serviceentry::setMutex(pthread_mutex_t *mutex) {
	#if !defined(HAVE_GETSERVBYNAME_R) || !defined(HAVE_GETSERVBYPORT_R)
		semutex=mutex;
	#endif
}
#endif

bool serviceentry::initialize(const char *servicename,
							const char *protocol) {
	return initialize(servicename,0,protocol);
}

bool serviceentry::initialize(int port, const char *protocol) {
	return initialize(NULL,port,protocol);
}

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
			#if defined(HAVE_GETSERVBYNAME_R_6) && \
				defined(HAVE_GETSERVBYPORT_R_6)
			if (!((servicename)
				?(getservbyname_r(servicename,protocol,
							&sebuffer,
							buffer,size,&se))
				:(getservbyport_r(htons(port),protocol,
							&sebuffer,
							buffer,size,&se)))) {
				return (se!=NULL);
			}
			#elif defined(HAVE_GETSERVBYNAME_R_5) && \
				defined(HAVE_GETSERVBYPORT_R_5)
			if ((servicename)
				?(se=getservbyname_r(servicename,protocol,
							&sebuffer,
							buffer,size))
				:(se=getservbyport_r(htons(port),protocol,
							&sebuffer,
							buffer,size))) {
				return true;
			}
			#endif
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
#ifdef RUDIMENTS_HAS_THREADS
		return (!(semutex && pthread_mutex_lock(semutex)) &&
			((se=((servicename)
				?getservbyname(servicename,protocol)
				:getservbyport(htons(port),protocol)))!=NULL) &&
			!(semutex && pthread_mutex_unlock(semutex)));
#else
		return ((se=((servicename)
				?getservbyname(servicename,protocol)
				:getservbyport(htons(port),protocol)))!=NULL);
#endif
	#endif
}

bool serviceentry::getAliasList(const char *servicename,
						const char *protocol,
							char ***aliaslist) {
	serviceentry	se;
	if (se.initialize(servicename,protocol)) {
		int	counter;
		for (counter=0; se.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(se.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

bool serviceentry::getPort(const char *servicename, const char *protocol,
								int *port) {
	serviceentry	se;
	if (se.initialize(servicename,protocol)) {
		*port=se.getPort();
		return true;
	}
	return false;
}

bool serviceentry::getName(int port, const char *protocol, char **name) {
	serviceentry	se;
	if (se.initialize(port,protocol)) {
		*name=charstring::duplicate(se.getName());
		return true;
	}
	return false;
}

bool serviceentry::getAliasList(int port, const char *protocol,
							char ***aliaslist) {
	serviceentry	se;
	if (se.initialize(port,protocol)) {
		int	counter;
		for (counter=0; se.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(se.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
