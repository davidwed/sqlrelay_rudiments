// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/inetclientsocket.h>
#include <rudiments/hostentry.h>
#include <rudiments/protocolentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>

// for addrinfo
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif
#ifdef RUDIMENTS_HAVE_WINSOCK2_H
	#include <winsock2.h>
#endif

#include <stdio.h>
#include <stdlib.h>

// Some implementations of pthreads use a macro to redefine connect
// as __pthread_connect.  We don't want that here.
#undef connect

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class inetclientsocketprivate {
	friend class inetclientsocket;
	private:
};

inetclientsocket::inetclientsocket() : clientsocket(), inetsocketutil() {
	pvt=new inetclientsocketprivate;
	translateByteOrder();
	type("inetclientsocket");
}

inetclientsocket::inetclientsocket(const inetclientsocket &i) :
					clientsocket(i), inetsocketutil(i) {
	pvt=new inetclientsocketprivate;
	type("inetclientsocket");
}

inetclientsocket &inetclientsocket::operator=(const inetclientsocket &i) {
	if (this!=&i) {
		clientsocket::operator=(i);
		inetsocketutil::operator=(i);
	}
	return *this;
}

inetclientsocket::~inetclientsocket() {
	delete pvt;
}

int inetclientsocket::connect(const char *host,
						unsigned short port,
						long timeoutsec,
						long timeoutusec,
						unsigned long retrywait,
						unsigned long retrycount) {
	initialize(host,port,timeoutsec,timeoutusec,retrywait,retrycount);
	return connect();
}

void inetclientsocket::initialize(const char *host,
						unsigned short port,
						long timeoutsec,
						long timeoutusec,
						unsigned long retrywait,
						unsigned long retrycount) {
	inetsocketutil::initialize(host,port);
	client::initialize(NULL,timeoutsec,timeoutusec,retrywait,retrycount);
}

void inetclientsocket::initialize(constnamevaluepairs *cd) {

	if (cd) {

		const char	*host;
		cd->getData("host",&host);
		const char	*port;
		cd->getData("port",&port);
		const char	*timeoutsec;
		cd->getData("timeoutsec",&timeoutsec);
		const char	*timeoutusec;
		cd->getData("timeoutusec",&timeoutusec);
		const char	*retrywait;
		cd->getData("retrywait",&retrywait);
		const char	*retrycount;
		cd->getData("retrycount",&retrycount);
	
		initialize(host?host:"",
			charstring::toInteger(port?port:"0"),
			charstring::toInteger(timeoutsec?timeoutsec:"-1"),
			charstring::toInteger(timeoutusec?timeoutusec:"-1"),
			charstring::toUnsignedInteger(retrywait?retrywait:"0"),
			charstring::toUnsignedInteger(retrycount?
							retrycount:"0"));
	}
}

int inetclientsocket::connect() {

	#ifndef RUDIMENTS_HAVE_GETADDRINFO

		// get the host entry
		hostentry	he;
		if (!he.initialize(_address())) {
			return RESULT_ERROR;
		}

		// use tcp protocol
		protocolentry	pe;
		if (!pe.initialize("tcp")) {
			return RESULT_ERROR;
		}

		// set the address type and port to connect to
		rawbuffer::zero(_sin(),sizeof(sockaddr_in));
		_sin()->sin_family=he.getAddressType();
		_sin()->sin_port=hostToNet(*_port());

		// create an inet socket
		do {
			fd(::socket(AF_INET,SOCK_STREAM,pe.getNumber()));
		} while (fd()==-1 && error::getErrorNumber()==EINTR);
		if (fd()==-1) {
			return RESULT_ERROR;
		}

	#else

		// create a hint indicating that SOCK_STREAM should be used
		addrinfo	hints;
		rawbuffer::zero(&hints,sizeof(addrinfo));
		// For now, we're using PF_INET.  Specifying PF_INET prevents
		// IPV6 addresses from working, but that's how this class works
		// if we don't have getaddrinfo anyway.
		// Also, FreeBSD 4.8 appears to have a bug where all addresses
		// returned from getaddrinfo have PF_INET6 for ai_family, even
		// if they're PF_INET addresses.
		hints.ai_family=PF_INET;
		hints.ai_socktype=SOCK_STREAM;

		// get a string representing the port number
		char	*portstr=charstring::parseNumber(*_port());

		// get the address info for the given address/port
		addrinfo	*ai=NULL;
		int		result;
		do {
			result=getaddrinfo(_address(),portstr,&hints,&ai);
		} while (result==EAI_SYSTEM && error::getErrorNumber()==EINTR);
		delete[] portstr;
		if (result) {
			return RESULT_ERROR;
		}

	#endif

	int	retval=RESULT_ERROR;

	// try to connect, over and over for the specified number of times
	for (unsigned long counter=0;
			counter<_retrycount() || !_retrycount(); counter++) {

		// wait the specified amount of time between reconnect tries
		// unless we're on the very first try
		if (counter) {
			snooze::macrosnooze(_retrywait());
		}

		#ifndef RUDIMENTS_HAVE_GETADDRINFO

			// try to connect to each of the addresses
			// that came back from the address lookup
			for (int addressindex=0;
					he.getAddressList()[addressindex];
					addressindex++) {

				// set which host to connect to
				rawbuffer::copy(&_sin()->sin_addr,
					he.getAddressList()[addressindex],
					he.getAddressLength());
	
				// attempt to connect
				retval=clientsocket::connect(
					reinterpret_cast<struct sockaddr *>(
									_sin()),
					sizeof(sockaddr_in),
					_timeoutsec(),
					_timeoutusec());
				if (retval==RESULT_SUCCESS) {
					return RESULT_SUCCESS;
				}
			}

		#else

			// try to connect to each of the addresses
			// that came back from the address lookup
			for (addrinfo *ainfo=ai; ainfo; ainfo=ainfo->ai_next) {

				// create an inet socket
				do {
					fd(::socket(ai->ai_family,
							ai->ai_socktype,
							ai->ai_protocol));
				} while (fd()==-1 &&
					error::getErrorNumber()==EINTR);
				if (fd()==-1) {
					continue;
				}

				// attempt to connect to the socket
				retval=clientsocket::connect(
					reinterpret_cast<struct sockaddr *>(
								ainfo->ai_addr),
						ainfo->ai_addrlen,
						_timeoutsec(),
						_timeoutusec());
				if (retval==RESULT_SUCCESS) {
					freeaddrinfo(ai);
					return RESULT_SUCCESS;
				} else {
					close();
				}
			}

		#endif
	}

	// if we're here, the connect failed
	#ifdef RUDIMENTS_HAVE_GETADDRINFO
		freeaddrinfo(ai);
	#else
		close();
	#endif

	return retval;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
