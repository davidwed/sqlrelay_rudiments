// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixserversocket.h>
#include <rudiments/charstring.h>

#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

unixserversocket::unixserversocket() : serversocket(), unixsocket() {
	mask=0;
}

bool unixserversocket::initialize(const char *filename, mode_t mask) {

	unixsocket::initialize(filename);
	this->mask=mask;

	// if a null or blank port was specified, return an error
	if (!filename || (filename && !filename[0])) {
		return false;
	}

	// init the socket structure
	unlink(filename);
	memset((void *)&sockaddrun,0,sizeof(sockaddrun));
	sockaddrun.sun_family=AF_UNIX;
	charstring::copyInto(sockaddrun.sun_path,filename);

	// create the socket
	return ((fd=::socket(AF_UNIX,SOCK_STREAM,0))>-1);
}

bool unixserversocket::bind() {

	// set umask and store old umask
	mode_t	oldmask=umask(mask);

	// bind the socket
	bool	retval=true;
	if (::bind(fd,(struct sockaddr *)&sockaddrun,sizeof(sockaddrun))==-1) {
		retval=false;
	}

	// restore old umask
	umask(oldmask);

	return retval;
}

unixsocket *unixserversocket::acceptClientConnection() {

	// initialize a socket address structure
	sockaddr_un	clientsun;
	socklen_t	size=sizeof(clientsun);
	memset((void *)&clientsun,0,sizeof(clientsun));

	// accept on the socket
	int		clientsock;
	if ((clientsock=::accept(fd,(struct sockaddr *)&clientsun,
					(socklen_t *)&size))==-1) {
		return NULL;
	}

	// return a new unixsocket
	unixsocket	*returnsock=new unixsocket(clientsock);
	#ifdef RUDIMENTS_HAS_SSL
		if (!sslAccept(returnsock)) {
			delete returnsock;
			return NULL;
		}
	#endif
	return returnsock;
}

bool unixserversocket::listenOnSocket(const char *filename, mode_t mask,
								int backlog) {
	initialize(filename,mask);
	return bind() && listen(backlog);
}
