// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/inetclientsocket.h>
#include <rudiments/hostentry.h>
#include <rudiments/protocolentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/sleep.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

inetclientsocket::inetclientsocket() : clientsocket(), inetsocketutil() {
	translateByteOrder();
}

inetclientsocket::inetclientsocket(const inetclientsocket &i) :
					clientsocket(i), inetsocketutil(i) {}

inetclientsocket &inetclientsocket::operator=(const inetclientsocket &i) {
	if (this!=&i) {
		clientsocket::operator=(i);
		inetsocketutil::operator=(i);
	}
	return *this;
}

inetclientsocket::~inetclientsocket() {}

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
	this->timeoutsec=timeoutsec;
	this->timeoutusec=timeoutusec;
	this->retrywait=retrywait;
	this->retrycount=retrycount;
}

void inetclientsocket::initialize(namevaluepairs *cd) {

	if (cd) {

		char	*host;
		cd->getData("host",&host);
		char	*port;
		cd->getData("port",&port);
		char	*timeoutsec;
		cd->getData("timeoutsec",&timeoutsec);
		char	*timeoutusec;
		cd->getData("timeoutusec",&timeoutusec);
		char	*retrywait;
		cd->getData("retrywait",&retrywait);
		char	*retrycount;
		cd->getData("retrycount",&retrycount);
	
		initialize(host?host:"",
			charstring::toLong(port?port:"0"),
			charstring::toLong(timeoutsec?timeoutsec:"-1"),
			charstring::toLong(timeoutusec?timeoutusec:"-1"),
			charstring::toUnsignedLong(retrywait?retrywait:"0"),
			charstring::toUnsignedLong(retrycount?retrycount:"0"));
	}
}

int inetclientsocket::connect() {

	#ifndef HAVE_GETADDRINFO

		// get the host entry
		hostentry	he;
		if (!he.initialize(address)) {
			return RESULT_ERROR;
		}

		// use tcp protocol
		protocolentry	pe;
		if (!pe.initialize("tcp")) {
			return RESULT_ERROR;
		}

		// set the address type and port to connect to
		rawbuffer::zero(&sin,sizeof(sin));
		sin.sin_family=he.getAddressType();
		sin.sin_port=htons(port);

		// create an inet socket
		if ((fd=::socket(AF_INET,SOCK_STREAM,pe.getNumber()))==-1) {
			fd=-1;
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
		char	*portstr=charstring::parseNumber((long)port);

		// get the address info for the given address/port
		addrinfo	*ai;
		if (getaddrinfo(address,portstr,&hints,&ai)) {
			delete[] portstr;
			return RESULT_ERROR;
		}
		delete[] portstr;

	#endif

	// if create failed, show this error

	// try to connect, over and over for the specified number of times
	for (unsigned long counter=0;
			counter<retrycount || !retrycount; counter++) {

		// wait the specified amount of time between reconnect tries
		// unless we're on the very first try
		if (counter) {
			sleep::macrosleep(retrywait);
		}

		#ifndef HAVE_GETADDRINFO

			// try to connect to each of the addresses
			// that came back from the address lookup
			for (int addressindex=0;
					he.getAddressList()[addressindex];
					addressindex++) {

				// set which host to connect to
				rawbuffer::copy(&sin.sin_addr,
					he.getAddressList()[addressindex],
					he.getAddressLength());
	
				// attempt to connect
				if (clientsocket::connect(
					reinterpret_cast<struct sockaddr *>(
									&sin),
					sizeof(sin),
					timeoutsec,
					timeoutusec)==RESULT_SUCCESS) {
					return RESULT_SUCCESS;
				}
			}

		#else

			// try to connect to each of the addresses
			// that came back from the address lookup
			for (addrinfo *ainfo=ai; ainfo; ainfo=ainfo->ai_next) {

				// create an inet socket
				if ((fd=::socket(ai->ai_family,
						ai->ai_socktype,
						ai->ai_protocol))==-1) {
					continue;
				}

				// attempt to connect to the socket
				if (clientsocket::connect(
					reinterpret_cast<struct sockaddr *>(
								ainfo->ai_addr),
						ainfo->ai_addrlen,
						timeoutsec,
						timeoutusec)==RESULT_SUCCESS) {
					freeaddrinfo(ai);
					return RESULT_SUCCESS;
				} else {
					close();
				}
			}

		#endif
	}

	// if we're here, the connect failed
	#ifdef HAVE_GETADDRINFO
		freeaddrinfo(ai);
	#else
		close();
	#endif

	return RESULT_ERROR;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
