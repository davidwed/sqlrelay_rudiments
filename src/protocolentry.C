// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/protocolentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

// for protoent, functions
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER	(32*1024)

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class protocolentryprivate {
	friend class protocolentry;
	private:
		protoent	*_pe;
		#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
				defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
			protoent	_pebuffer;
			char		*_buffer;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
	!defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R))
static mutex	*_pemutex;
#endif

protocolentry::protocolentry() {
	pvt=new protocolentryprivate;
	pvt->_pe=NULL;
	#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		rawbuffer::zero(&pvt->_pebuffer,sizeof(pvt->_pebuffer));
		pvt->_buffer=NULL;
	#endif
}

protocolentry::protocolentry(const protocolentry &p) {
	pvt=new protocolentryprivate;
	initialize(p.getName());
}

protocolentry &protocolentry::operator=(const protocolentry &p) {
	if (this!=&p) {
		initialize(p.getName());
	}
	return *this;
}

protocolentry::~protocolentry() {
	#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		delete[] pvt->_buffer;
	#endif
	delete pvt;
}

const char *protocolentry::getName() const {
	return pvt->_pe->p_name;
}

const char * const *protocolentry::getAliasList() const {
	return pvt->_pe->p_aliases;
}

int protocolentry::getNumber() const {
	return pvt->_pe->p_proto;
}

bool protocolentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		return true;
	#else
		return false;
	#endif
}

void protocolentry::setMutex(mutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		_pemutex=mtx;
	#endif
}

bool protocolentry::initialize(const char *protocolname) {
	return initialize(protocolname,0);
}

bool protocolentry::initialize(int number) {
	return initialize(NULL,number);
}

bool protocolentry::initialize(const char *protocolname, int number) {

	#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R)
		if (pvt->_pe) {
			pvt->_pe=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
		// getprotobyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R_5) && \
				defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_5)
			if (!((protocolname)
				?(getprotobyname_r(protocolname,
							&pvt->_pebuffer,
							pvt->_buffer,size,
							&pvt->_pe))
				:(getprotobynumber_r(number,
							&pvt->_pebuffer,
							pvt->_buffer,size,
							&pvt->_pe)))) {
				return (pvt->_pe!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETPROTOBYNAME_R_4) && \
				defined(RUDIMENTS_HAVE_GETPROTOBYNUMBER_R_4)
			if ((protocolname)
				?(pvt->_pe=getprotobyname_r(protocolname,
							&pvt->_pebuffer,
							pvt->_buffer,size))
				:(pvt->_pe=getprotobynumber_r(number,
							&pvt->_pebuffer,
							pvt->_buffer,size))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_pe=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		pvt->_pe=NULL;
		return (!(_pemutex && !_pemutex->lock())) &&
			((pvt->_pe=((protocolname)
				?getprotobyname(protocolname)
				:getprotobynumber(number)))!=NULL) &&
			!(_pemutex && !_pemutex->unlock());
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
			alias[i]=charstring::duplicate(pe.getAliasList()[i]);
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
		*name=charstring::duplicate(pe.getName());
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
			alias[i]=charstring::duplicate(pe.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

void protocolentry::print() const {

	if (!pvt->_pe) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Alias list:\n");
	for (int i=0; getAliasList()[i]; i++) {
		printf("	%s\n",getAliasList()[i]);
	}
	printf("Number: %d\n",getNumber());
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
