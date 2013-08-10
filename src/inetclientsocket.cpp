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

#include <rudiments/private/winsock.h>

// for addrinfo
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

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

int32_t inetclientsocket::connect(const char *host,
						uint16_t port,
						long timeoutsec,
						long timeoutusec,
						unsigned long retrywait,
						unsigned long retrycount) {
	initialize(host,port,timeoutsec,timeoutusec,retrywait,retrycount);
	return connect();
}

void inetclientsocket::initialize(const char *host,
						uint16_t port,
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

int32_t inetclientsocket::connect() {

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

		// Put the socket in blocking mode.  Most platforms create
		// sockets in blocking mode by default but OpenBSD doesn't
		// appear to (at least in version 4.9) so we'll force it to
		// blocking-mode to be consistent.
		if (!useBlockingMode() &&
				error::getErrorNumber() &&
				error::getErrorNumber()!=ENOTSUP
				#ifdef RUDIMENTS_HAVE_EOPNOTSUPP
				&& error::getErrorNumber()!=EOPNOTSUPP
				#endif
				) {
			close();
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
		int32_t		result;
		do {
			error::setErrorNumber(0);
			result=getaddrinfo(_address(),portstr,&hints,&ai);
		} while (result!=0 && error::getErrorNumber()==EINTR);
		// ...In theory, we should only loop back and try again if
		// getaddrinfo() returns EAI_SYSTEM and errno was EINTR.
		// However, apparently, on some systems, if interrupted, it can
		// return something other than EAI_SYSTEM.  So we just check
		// for non-zero.  We also reset the error number each time in
		// case we get a legitimate interrupt on one try and a different
		// error on the next try that doesn't reset errno itself.
		delete[] portstr;
		if (result) {
			return RESULT_ERROR;
		}

	#endif

	int32_t	retval=RESULT_ERROR;

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
			for (int32_t addressindex=0;
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

				// Put the socket in blocking mode.  Most
				// platforms create sockets in blocking mode by
				// default but OpenBSD doesn't appear to (at
				// least in version 4.9) so we'll force it to
				// blocking-mode to be consistent.
				if (!useBlockingMode() &&
					error::getErrorNumber() &&
					error::getErrorNumber()!=ENOTSUP
					#ifdef RUDIMENTS_HAVE_EOPNOTSUPP
					&& error::getErrorNumber()!=EOPNOTSUPP
					#endif
					) {
					freeaddrinfo(ai);
					close();
					return RESULT_ERROR;
				}

				// attempt to connect
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
