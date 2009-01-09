// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetserversocket.h>
#include <rudiments/inetclientsocket.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <arpa/inet.h>
#include <netdb.h>

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

unsigned short inetserversocket::getPort() {
	return *_port();
}

bool inetserversocket::listen(const char *address, unsigned short port,
								int backlog) {
	initialize(address,port);
	return (bind() && listen(backlog));
}

bool inetserversocket::initialize(const char *address, unsigned short port) {

	inetsocketutil::initialize(address,port);

	// initialize a socket address structure
	rawbuffer::zero(_sin(),sizeof(sockaddr_in));
	_sin()->sin_family=AF_INET;
	_sin()->sin_port=hostToNet(port);

	// if a specific address was passed in, bind to it only,
	// otherwise bind to all addresses
	if (address && address[0] && charstring::compare(address,"0.0.0.0")) {
		in_addr	ia;
		if (!inet_aton(address,&ia)) {
			return false;
		}
		_sin()->sin_addr.s_addr=ia.s_addr;
	} else {
		_sin()->sin_addr.s_addr=hostToNet((uint32_t)INADDR_ANY);
	}

	// create the socket
	do {
		fd(::socket(AF_INET,SOCK_STREAM,0));
	} while (fd()==-1 && error::getErrorNumber()==EINTR);
	return (fd()!=-1);
}

bool inetserversocket::bind() {

	// bind the socket
	int	result;
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

		int	result;
		do {
			result=getsockname(fd(),
				reinterpret_cast<struct sockaddr *>
							(&socknamesin),
				&size);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (result!=-1) {
			*_port()=static_cast<unsigned short int>(
						ntohs(socknamesin.sin_port));
		}
	}
	return true;
}

bool inetserversocket::listen(int backlog) {
	int	result;
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
	int	clientsock;
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
