// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixserversocket.h>
#include <rudiments/unixclientsocket.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/file.h>

// need for umask...
#include <sys/stat.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

unixserversocket::unixserversocket() : serversocket(), unixsocketutil() {
	mask=0;
	type="unixserversocket";
}

unixserversocket::unixserversocket(const unixserversocket &u) :
				serversocket(u), unixsocketutil(u) {
	mask=u.mask;
	type="unixserversocket";
}

unixserversocket &unixserversocket::operator=(const unixserversocket &u) {
	if (this!=&u) {
		serversocket::operator=(u);
		unixsocketutil::operator=(u);
		mask=u.mask;
	}
	return *this;
}

unixserversocket::~unixserversocket() {}

bool unixserversocket::initialize(const char *filename, mode_t mask) {

	unixsocketutil::initialize(filename);
	this->mask=mask;

	// if a null or blank port was specified, return an error
	if (!filename || (filename && !filename[0])) {
		return false;
	}

	// init the socket structure
	file::remove(filename);
	rawbuffer::zero(&sockaddrun,sizeof(sockaddrun));
	sockaddrun.sun_family=AF_UNIX;
	charstring::copy(sockaddrun.sun_path,filename);

	// create the socket
	return ((fd=::socket(AF_UNIX,SOCK_STREAM,0))>-1);
}

bool unixserversocket::listen(const char *filename, mode_t mask, int backlog) {
	initialize(filename,mask);
	return (bind() && listen(backlog));
}

bool unixserversocket::bind() {

	// set umask and store old umask
	mode_t	oldmask=umask(mask);

	// bind the socket
	bool	retval=true;
	if (::bind(fd,reinterpret_cast<struct sockaddr *>(&sockaddrun),
						sizeof(sockaddrun))==-1) {
		retval=false;
	}

	// restore old umask
	umask(oldmask);

	return retval;
}

bool unixserversocket::listen(int backlog) {
	return !::listen(fd,backlog);
}

filedescriptor *unixserversocket::accept() {

	// initialize a socket address structure
	sockaddr_un	clientsun;
	socklen_t	size=sizeof(clientsun);
	rawbuffer::zero(&clientsun,sizeof(clientsun));

	// accept on the socket
	int		clientsock;
	if ((clientsock=::accept(fd,
				reinterpret_cast<struct sockaddr *>(&clientsun),
				&size))==-1) {
		return NULL;
	}

	unixclientsocket	*returnsock=new unixclientsocket;
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
