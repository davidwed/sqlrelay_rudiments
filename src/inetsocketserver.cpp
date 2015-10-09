// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
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

#ifndef INADDR_NONE
	#define INADDR_NONE ((in_addr_t)-1)
#endif

class inetsocketserverprivate {
	friend class inetsocketserver;
	private:
};

inetsocketserver::inetsocketserver() : socketserver(), inetsocketutil() {
	pvt=new inetsocketserverprivate;
	translateByteOrder();
	type("inetsocketserver");
}

inetsocketserver::inetsocketserver(const inetsocketserver &i) :
					socketserver(i), inetsocketutil(i) {
	pvt=new inetsocketserverprivate;
	translateByteOrder();
	type("inetsocketserver");
}

inetsocketserver &inetsocketserver::operator=(const inetsocketserver &i) {
	if (this!=&i) {
		socketserver::operator=(i);
		inetsocketutil::operator=(i);
		translateByteOrder();
	}
	return *this;
}

inetsocketserver::~inetsocketserver() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class, especially on Windows where
	// closesocket() must be called rather than close() to prevent a crash.
	// If close() is called here, it will eventually call this method's
	// lowLevelClose() rather than filedescriptor::lowLevelClose().
	close();
	delete pvt;
}

uint16_t inetsocketserver::getPort() {
	return *_port();
}

bool inetsocketserver::listen(const char *address, uint16_t port,
							int32_t backlog) {
	initialize(address,port);
	reuseAddresses();
	return (bind() && listen(backlog));
}

bool inetsocketserver::initialize(const char *address, uint16_t port) {

	inetsocketutil::initialize(address,port);

	// initialize a socket address structure
	bytestring::zero(_sin(),sizeof(sockaddr_in));
	_sin()->sin_family=AF_INET;
	_sin()->sin_port=hostToNet(port);

	// if a specific address was passed in, bind to it only,
	// otherwise bind to all addresses
	if (!charstring::isNullOrEmpty(address) &&
			charstring::compare(address,"0.0.0.0")) {
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
	error::clearError();
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
			error::getErrorNumber()
			#ifdef ENOTSUP
			&& error::getErrorNumber()!=ENOTSUP
			#endif
			#ifdef EOPNOTSUPP
			&& error::getErrorNumber()!=EOPNOTSUPP
			#endif
			) {
		close();
		return false;
	}

	return true;
}

bool inetsocketserver::bind() {

	// bind the socket
	int32_t	result;
	error::clearError();
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
		sockaddr_in			socknamesin;
		RUDIMENTS_SOCKLEN_OR_SIZE_T	size=sizeof(socknamesin);
		bytestring::zero(&socknamesin,sizeof(socknamesin));

		int32_t	result;
		error::clearError();
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

bool inetsocketserver::listen(int32_t backlog) {
	int32_t	result;
	error::clearError();
	do {
		result=::listen(fd(),backlog);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

filedescriptor *inetsocketserver::accept() {

	// initialize a socket address structure
	sockaddr_in			clientsin;
	RUDIMENTS_SOCKLEN_OR_SIZE_T	size=sizeof(clientsin);
	bytestring::zero(&clientsin,sizeof(clientsin));

	// accept on the socket
	int32_t	clientsock;
	error::clearError();
	do {
		clientsock=::accept(fd(),
				reinterpret_cast<struct sockaddr *>(&clientsin),
				&size);
	} while (clientsock==-1 && error::getErrorNumber()==EINTR);
	if (clientsock==-1) {
		return NULL;
	}

	inetsocketclient	*returnsock=new inetsocketclient;
	returnsock->setFileDescriptor(clientsock);

	// set the client socket to the same blocking/non-blocking
	// mode as the server socket
	if (!((isUsingNonBlockingMode())?
			returnsock->useNonBlockingMode():
			returnsock->useBlockingMode()) &&
				error::getErrorNumber()
				#ifdef ENOTSUP
				&& error::getErrorNumber()!=ENOTSUP
				#endif
				#ifdef EOPNOTSUPP
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
