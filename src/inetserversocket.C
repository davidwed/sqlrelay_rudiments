// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetserversocket.h>
#include <rudiments/inetclientsocket.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>

#include <arpa/inet.h>
#include <netdb.h>

inetserversocket::inetserversocket() : serversocket(), inetsocketutil() {
	translateByteOrder();
}

inetserversocket::~inetserversocket() {}

unsigned short inetserversocket::getPort() {
	return port;
}

bool inetserversocket::listen(const char *address, unsigned short port,
								int backlog) {
	initialize(address,port);
	return (bind() && listen(backlog));
}

bool inetserversocket::initialize(const char *address, unsigned short port) {

	inetsocketutil::initialize(address,port);

	// initialize a socket address structure
	rawbuffer::zero((void *)&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_port=htons(port);

	// if a specific address was passed in, bind to it only,
	// otherwise bind to all addresses
	if (address && address[0]) {
		sin.sin_addr.s_addr=htonl(inet_addr(address));
	} else {
		sin.sin_addr.s_addr=htonl(INADDR_ANY);
	}

	// create the socket
	return ((fd=::socket(AF_INET,SOCK_STREAM,0))>-1);
}

bool inetserversocket::bind() {

	// bind the socket
	if (::bind(fd,(struct sockaddr *)&sin,sizeof(sin))==-1) {
		return false;
	}

	// get the actual port number if an arbitrary port was requested
	if (!port) {

		// initialize a socket address structure
		sockaddr_in	socknamesin;
		socklen_t	size=sizeof(socknamesin);
		rawbuffer::zero((void *)&socknamesin,sizeof(socknamesin));

		if (getsockname(fd,(struct sockaddr *)&socknamesin,
					(socklen_t *)&size)>-1) {
			port=(unsigned short int)ntohs(socknamesin.sin_port);
		}
	}
	return true;
}

bool inetserversocket::listen(int backlog) {
	return !::listen(fd,backlog);
}

filedescriptor *inetserversocket::accept() {

	// initialize a socket address structure
	sockaddr_in	clientsin;
	socklen_t	size=sizeof(clientsin);
	rawbuffer::zero((void *)&clientsin,sizeof(clientsin));

	// accept on the socket
	int		clientsock;
	if ((clientsock=::accept(fd,(struct sockaddr *)&clientsin,
					(socklen_t *)&size))==-1) {
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

char *inetserversocket::getClientAddress() {

	// initialize a socket address structure
	struct sockaddr_in	clientsin;
	int			size=sizeof(clientsin);
	rawbuffer::zero((void *)&clientsin,sizeof(clientsin));

	// get the peer address
	if (getpeername(fd,(struct sockaddr *)&clientsin,
				(socklen_t *)&size)==-1) {
		return NULL;
	}

	// convert the address to a string and return a copy of it
	return charstring::duplicate(inet_ntoa(clientsin.sin_addr));
}
