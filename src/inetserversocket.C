// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetserversocket.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif

inetserversocket::inetserversocket() : serversocket(), inetsocket() {}

unsigned short inetserversocket::getPort() {
	return port;
}

bool inetserversocket::listenOnSocket(const char *address, unsigned short port,
								int backlog) {
	initialize(address,port);
	return (bind() && listen(backlog));
}

bool inetserversocket::initialize(const char *address, unsigned short port) {

	inetsocket::initialize(address,port);

	// initialize a socket address structure
	memset((void *)&sin,0,sizeof(sin));
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
		memset((void *)&socknamesin,0,sizeof(socknamesin));

		if (getsockname(fd,(struct sockaddr *)&socknamesin,
					(socklen_t *)&size)>-1) {
			port=(unsigned short int)ntohs(socknamesin.sin_port);
		}
	}
	return true;
}

inetsocket *inetserversocket::acceptClientConnection() {

	// initialize a socket address structure
	sockaddr_in	clientsin;
	socklen_t	size=sizeof(clientsin);
	memset((void *)&clientsin,0,sizeof(clientsin));

	// accept on the socket
	int		clientsock;
	if ((clientsock=::accept(fd,(struct sockaddr *)&clientsin,
					(socklen_t *)&size))==-1) {
		return NULL;
	}

	inetsocket	*returnsock=new inetsocket(clientsock);
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
	memset((void *)&clientsin,0,sizeof(clientsin));

	// get the peer address
	if (getpeername(fd,(struct sockaddr *)&clientsin,
				(socklen_t *)&size)==-1) {
		return NULL;
	}

	// convert the address to a string and return a copy of it
	return strdup(inet_ntoa(clientsin.sin_addr));
}
