// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/serviceentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/filedescriptor.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#include <rudiments/private/winsock.h>

// for servent, functions
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif
#ifdef RUDIMENTS_HAVE_NETINET_IN_H
	#include <netinet/in.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#define MAXBUFFER	(32*1024)

class serviceentryprivate {
	friend class serviceentry;
	private:
		servent		*_se;
		#if defined(RUDIMENTS_HAVE_GETSERVBYNAME_R) || \
				defined(RUDIMENTS_HAVE_GETSERVBYPORT_R)
			servent		_sebuffer;
			char		*_buffer;
		#endif
};

// LAME: not in the class
#if (!defined(RUDIMENTS_HAVE_GETSERVBYNAME_R) || \
	!defined(RUDIMENTS_HAVE_GETSERVBYPORT_R))
static threadmutex	*_semutex;
#endif


serviceentry::serviceentry() {
	pvt=new serviceentryprivate;
	pvt->_se=NULL;
	#if defined(RUDIMENTS_HAVE_GETSERVBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETSERVBYPORT_R)
		rawbuffer::zero(&pvt->_sebuffer,sizeof(pvt->_sebuffer));
		pvt->_buffer=NULL;
	#endif
}

serviceentry::serviceentry(const serviceentry &s) {
	pvt=new serviceentryprivate;
	initialize(s.getName(),s.getProtocol());
}

serviceentry &serviceentry::operator=(const serviceentry &s) {
	if (this!=&s) {
		initialize(s.getName(),s.getProtocol());
	}
	return *this;
}

serviceentry::~serviceentry() {
	#if defined(RUDIMENTS_HAVE_GETSERVBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETSERVBYPORT_R)
		delete[] pvt->_buffer;
	#endif
	delete pvt;
}

const char *serviceentry::getName() const {
	return pvt->_se->s_name;
}

int32_t serviceentry::getPort() const {
	return filedescriptor::netToHost(
			static_cast<uint16_t>(pvt->_se->s_port));
}

const char *serviceentry::getProtocol() const {
	return pvt->_se->s_proto;
}

const char * const *serviceentry::getAliasList() const {
	return pvt->_se->s_aliases;
}

bool serviceentry::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_GETSERVBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETSERVBYPORT_R)
		return true;
	#else
		return false;
	#endif
}

void serviceentry::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_GETSERVBYNAME_R) || \
		!defined(RUDIMENTS_HAVE_GETSERVBYPORT_R)
		_semutex=mtx;
	#endif
}

bool serviceentry::initialize(const char *servicename, const char *protocol) {
	return initialize(servicename,0,protocol);
}

bool serviceentry::initialize(int32_t port, const char *protocol) {
	return initialize(NULL,port,protocol);
}

bool serviceentry::initialize(const char *servicename, int32_t port,
						const char *protocol) {

	#if defined(RUDIMENTS_HAVE_GETSERVBYNAME_R) && \
		defined(RUDIMENTS_HAVE_GETSERVBYPORT_R)
		if (pvt->_se) {
			pvt->_se=NULL;
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
		}
		// getservbyname_r is goofy.
		// It will retrieve an arbitrarily large amount of data, but
		// requires that you pass it a pre-allocated buffer.  If the
		// buffer is too small, it returns an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int32_t size=1024; size<MAXBUFFER; size=size+1024) {
			pvt->_buffer=new char[size];
			#if defined(RUDIMENTS_HAVE_GETSERVBYNAME_R_6) && \
				defined(RUDIMENTS_HAVE_GETSERVBYPORT_R_6)
			if (!((servicename)
				?(getservbyname_r(servicename,protocol,
							&pvt->_sebuffer,
							pvt->_buffer,size,
							&pvt->_se))
				:(getservbyport_r(
					filedescriptor::hostToNet(
							(uint16_t)port),
							protocol,
							&pvt->_sebuffer,
							pvt->_buffer,size,
							&pvt->_se)))) {
				return (pvt->_se!=NULL);
			}
			#elif defined(RUDIMENTS_HAVE_GETSERVBYNAME_R_5) && \
				defined(RUDIMENTS_HAVE_GETSERVBYPORT_R_5)
			if ((servicename)
				?(pvt->_se=getservbyname_r(servicename,protocol,
							&pvt->_sebuffer,
							pvt->_buffer,size))
				:(pvt->_se=getservbyport_r(
					filedescriptor::hostToNet(
							(uint16_t)port),
							protocol,
							&pvt->_sebuffer,
							pvt->_buffer,size))) {
				return true;
			}
			#endif
			delete[] pvt->_buffer;
			pvt->_buffer=NULL;
			pvt->_se=NULL;
			if (error::getErrorNumber()!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		pvt->_se=NULL;
		return (!(_semutex && !_semutex->lock()) &&
			((pvt->_se=((servicename)
				?getservbyname(servicename,protocol)
				:getservbyport(filedescriptor::hostToNet(
						(uint16_t)port),
						protocol)))!=NULL) &&
			!(_semutex && !_semutex->unlock()));
	#endif
}

bool serviceentry::getAliasList(const char *servicename,
						const char *protocol,
							char ***aliaslist) {
	serviceentry	se;
	if (se.initialize(servicename,protocol)) {
		int32_t	counter;
		for (counter=0; se.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int32_t i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(se.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

bool serviceentry::getPort(const char *servicename, const char *protocol,
								int32_t *port) {
	serviceentry	se;
	if (se.initialize(servicename,protocol)) {
		*port=se.getPort();
		return true;
	}
	return false;
}

bool serviceentry::getName(int32_t port, const char *protocol, char **name) {
	serviceentry	se;
	if (se.initialize(port,protocol)) {
		*name=charstring::duplicate(se.getName());
		return true;
	}
	return false;
}

bool serviceentry::getAliasList(int32_t port, const char *protocol,
							char ***aliaslist) {
	serviceentry	se;
	if (se.initialize(port,protocol)) {
		int32_t	counter;
		for (counter=0; se.getAliasList()[counter]; counter++);
		char	**alias=new char *[counter+1];
		alias[counter]=NULL;
		for (int32_t i=0; i<counter; i++) {
			alias[i]=charstring::duplicate(se.getAliasList()[i]);
		}
		*aliaslist=alias;
		return true;
	}
	return false;
}

void serviceentry::print() const {

	if (!pvt->_se) {
		return;
	}

	stdoutput.printf("Name: %s\n",getName());
	// some compilers complain without this cast
	stdoutput.printf("Port: %d\n",(int)getPort());
	stdoutput.printf("Protocol: %s\n",getProtocol());
	stdoutput.printf("Alias list:\n");
	for (int32_t i=0; getAliasList()[i]; i++) {
		stdoutput.printf("	%s\n",getAliasList()[i]);
	}
}
