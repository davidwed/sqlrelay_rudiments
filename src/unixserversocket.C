// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixserversocket.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/unixserversocketinlines.h>
#endif

#include <sys/stat.h>
#include <sys/socket.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

int	unixserversocket::initialize(const char *filename, mode_t mask) {

	unixsocket::initialize(filename);
	this->mask=mask;

	// if a null or blank port was specified, return an error
	if (!filename || (filename && !filename[0])) {
		return 0;
	}

	// init the socket structure
	unlink(filename);
	memset((void *)&sun,0,sizeof(sun));
	sun.sun_family=AF_UNIX;
	strcpy(sun.sun_path,filename);

	// create the socket
	return (filedescriptor=socket(AF_UNIX,SOCK_STREAM,0))>-1;
}

int	unixserversocket::bind() {

	// set umask and store old umask
	mode_t	oldmask=umask(mask);

	// bind the socket
	int	retval=1;
	if (::bind(filedescriptor,(struct sockaddr *)&sun,sizeof(sun))==-1) {
		retval=0;
	}

	// restore old umask
	umask(oldmask);

	return retval;
}

unixsocket	*unixserversocket::acceptClientConnection() {

	// initialize a socket address structure
	sockaddr_un	clientsun;
	socklen_t	size=sizeof(clientsun);
	memset((void *)&clientsun,0,sizeof(clientsun));

	// accept on the socket
	int		clientsock;
	if ((clientsock=accept(filedescriptor,
					(struct sockaddr *)&clientsun,
					(socklen_t *)&size))==-1) {
		return NULL;
	}

	// return a new unixsocket
	return new unixsocket(clientsock);
}
