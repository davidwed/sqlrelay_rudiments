// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/rpcentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

// Some systems (notably cygwin 1.5.7-1) define getrpcbyname and getrpcbynumber
// in their header files but then either don't implement them or don't export
// the symbols.
#if (defined(HAVE_GETRPCBYNAME_R) || defined(HAVE_GETRPCBYNAME)) \
	&& (defined(HAVE_GETRPCBYNUMBER_R) || defined(HAVE_GETRPCBYNUMBER)) \

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__) && \
	(!defined(HAVE_GETRPCBYNAME_R) || !defined(HAVE_GETRPCBYNUMBER_R))
pthread_mutex_t	*rpcentry::remutex;
#endif


rpcentry::rpcentry() {
	re=NULL;
	#if defined(HAVE_GETRPCBYNAME_R) && defined(HAVE_GETRPCBYNUMBER_R)
		rawbuffer::zero(&rebuffer,sizeof(rebuffer));
		buffer=NULL;
	#endif
}

rpcentry::~rpcentry() {
	#if defined(HAVE_GETRPCBYNAME_R) && defined(HAVE_GETRPCBYNUMBER_R)
		delete[] buffer;
	#endif
}

const char *rpcentry::getName() const {
	return re->r_name;
}

int rpcentry::getNumber() const {
	return re->r_number;
}

const char * const *rpcentry::getAliasList() const {
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

	#if defined(HAVE_GETRPCBYNAME_R) && defined(HAVE_GETRPCBYNUMBER_R)
		if (re) {
			re=NULL;
			delete[] buffer;
			buffer=NULL;
		}
		// getrpcbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			#if defined(HAVE_GETRPCBYNAME_R_5) && \
				defined(HAVE_GETRPCBYNUMBER_R_5)
			if (!((rpcname)
				?(getrpcbyname_r(rpcname,&rebuffer,
							buffer,size,&re))
				:(getrpcbynumber_r(number,&rebuffer,
							buffer,size,&re)))) {
				return (re!=NULL);
			}
			#elif defined(HAVE_GETRPCBYNAME_R_4) && \
				defined(HAVE_GETRPCBYNUMBER_R_4)
			if ((rpcname)
				?(re=getrpcbyname_r(rpcname,&rebuffer,
							buffer,size))
				:(re=getrpcbynumber_r(number,&rebuffer,
							buffer,size))) {
				return true;
			}
			#endif
			delete[] buffer;
			buffer=NULL;
			re=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		re=NULL;
		#ifdef RUDIMENTS_HAS_THREADS
		return (!(remutex && pthread_mutex_lock(remutex)) &&
			((re=((rpcname)
				?getrpcbyname(const_cast<char *>(rpcname))
				:getrpcbynumber(number)))!=NULL) &&
			!(remutex && pthread_mutex_unlock(remutex)));
		#else
		return ((re=((rpcname)
				?getrpcbyname(const_cast<char *>(rpcname))
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
			alias[i]=charstring::duplicate(re.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

bool rpcentry::getName(int number, char **name) {
	rpcentry	re;
	if (re.initialize(number)) {
		*name=charstring::duplicate(re.getName());
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
			alias[i]=charstring::duplicate(re.getAliasList()[i]);
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

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
