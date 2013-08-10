// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetserversocket.h>
#include <rudiments/inetclientsocket.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_ARPA_INET_H
	#include <arpa/inet.h>
#endif
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif

#ifndef RUDIMENTS_HAVE_IN_ADDR_T
typedef unsigned long	in_addr_t;
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class inetserversocketprivate {
	friend class inetserversocket;
	private:
};

inetserversocket::inetserversocket() : serversocket(), inetsocketutil() {
	pvt=new inetserversocketprivate;
	translateByteOrder();
	type("inetserversocket");
}

inetserversocket::inetserversocket(const inetserversocket &i) :
					serversocket(i), inetsocketutil(i) {
	pvt=new inetserversocketprivate;
	type("inetserversocket");
}

inetserversocket &inetserversocket::operator=(const inetserversocket &i) {
	if (this!=&i) {
		serversocket::operator=(i);
		inetsocketutil::operator=(i);
	}
	return *this;
}

inetserversocket::~inetserversocket() {
	delete pvt;
}

uint16_t inetserversocket::getPort() {
	return *_port();
}

bool inetserversocket::listen(const char *address, uint16_t port,
							int32_t backlog) {
	initialize(address,port);
	reuseAddresses();
	return (bind() && listen(backlog));
}

bool inetserversocket::initialize(const char *address, uint16_t port) {

	inetsocketutil::initialize(address,port);

	// initialize a socket address structure
	rawbuffer::zero(_sin(),sizeof(sockaddr_in));
	_sin()->sin_family=AF_INET;
	_sin()->sin_port=hostToNet(port);

	// if a specific address was passed in, bind to it only,
	// otherwise bind to all addresses
	if (address && address[0] && charstring::compare(address,"0.0.0.0")) {
		#if defined(RUDIMENTS_HAVE_INET_ATON)
			in_addr	ia;
			if (!inet_aton(address,&ia)) {
				return false;
			}
			_sin()->sin_addr.s_addr=ia.s_addr;
		#elif defined(RUDIMENTS_HAVE_INET_ADDR)
			in_addr_t	saddr=inet_addr(address);
			if (saddr==INADDR_NONE) {
				return false;
			}
			_sin()->sin_addr.s_addr=saddr;
		#else
			#error no inet_aton or anything like it
		#endif
	} else {
		_sin()->sin_addr.s_addr=hostToNet((uint32_t)INADDR_ANY);
	}

	// create the socket
	do {
		fd(::socket(AF_INET,SOCK_STREAM,0));
	} while (fd()==-1 && error::getErrorNumber()==EINTR);
	if (fd()==-1) {
		return false;
	}

	// Put the socket in blocking mode.  Most platforms create sockets in
	// blocking mode by default but OpenBSD doesn't appear to (at least in
	// version 4.9) so we'll force it to blocking-mode to be consistent.
	if (!useBlockingMode() &&
			error::getErrorNumber() &&
			error::getErrorNumber()!=ENOTSUP
			#ifdef RUDIMENTS_HAVE_EOPNOTSUPP
			&& error::getErrorNumber()!=EOPNOTSUPP
			#endif
			) {
		close();
		return false;
	}

	return true;
}

bool inetserversocket::bind() {

	// bind the socket
	int32_t	result;
	do {
		result=::bind(fd(),reinterpret_cast<struct sockaddr *>(_sin()),
							sizeof(sockaddr_in));
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result==-1) {
		return false;
	}

	// get the actual port number if an arbitrary port was requested
	if (!*_port()) {

		// initialize a socket address structure
		sockaddr_in	socknamesin;
		socklen_t	size=sizeof(socknamesin);
		rawbuffer::zero(&socknamesin,sizeof(socknamesin));

		int32_t	result;
		do {
			result=getsockname(fd(),
				reinterpret_cast<struct sockaddr *>
							(&socknamesin),
				&size);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (result!=-1) {
			*_port()=static_cast<uint16_t>(
						ntohs(socknamesin.sin_port));
		}
	}
	return true;
}

bool inetserversocket::listen(int32_t backlog) {
	int32_t	result;
	do {
		result=::listen(fd(),backlog);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

filedescriptor *inetserversocket::accept() {

	// initialize a socket address structure
	sockaddr_in	clientsin;
	socklen_t	size=sizeof(clientsin);
	rawbuffer::zero(&clientsin,sizeof(clientsin));

	// accept on the socket
	int32_t	clientsock;
	do {
		clientsock=::accept(fd(),
				reinterpret_cast<struct sockaddr *>(&clientsin),
				&size);
	} while (clientsock==-1 && error::getErrorNumber()==EINTR);
	if (clientsock==-1) {
		return NULL;
	}

	inetclientsocket	*returnsock=new inetclientsocket;
	returnsock->setFileDescriptor(clientsock);

	// set the client socket to the same blocking/non-blocking
	// mode as the server socket
	if (!((isUsingNonBlockingMode())?
			returnsock->useNonBlockingMode():
			returnsock->useBlockingMode()) &&
				error::getErrorNumber() &&
				error::getErrorNumber()!=ENOTSUP
				#ifdef RUDIMENTS_HAVE_EOPNOTSUPP
				&& error::getErrorNumber()!=EOPNOTSUPP
				#endif
				) {
		delete returnsock;
		return NULL;
	}

	// handle SSL-accept if necessary
	#ifdef RUDIMENTS_HAS_SSL
		if (!sslAccept(returnsock)) {
			delete returnsock;
			return NULL;
		}
	#endif

	return returnsock;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
