// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixserversocket.h>
#include <rudiments/unixclientsocket.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class unixserversocketprivate {
	friend class unixserversocket;
	private:
		mode_t	_mask;
};

unixserversocket::unixserversocket() : serversocket(), unixsocketutil() {
	pvt=new unixserversocketprivate;
	pvt->_mask=0;
	type("unixserversocket");
}

unixserversocket::unixserversocket(const unixserversocket &u) :
				serversocket(u), unixsocketutil(u) {
	pvt=new unixserversocketprivate;
	pvt->_mask=u.pvt->_mask;
	type("unixserversocket");
}

unixserversocket &unixserversocket::operator=(const unixserversocket &u) {
	if (this!=&u) {
		serversocket::operator=(u);
		unixsocketutil::operator=(u);
		pvt->_mask=u.pvt->_mask;
	}
	return *this;
}

unixserversocket::~unixserversocket() {
	delete pvt;
}

bool unixserversocket::initialize(const char *filename, mode_t mask) {

	unixsocketutil::initialize(filename);
	pvt->_mask=mask;

	// if a null or blank port was specified, return an error
	if (!filename || (filename && !filename[0])) {
		return false;
	}

	// init the socket structure
	file::remove(filename);
	rawbuffer::zero(_sun(),sizeof(sockaddr_un));
	_sun()->sun_family=AF_UNIX;
	charstring::copy(_sun()->sun_path,filename);

	// create the socket
	do {
		fd(::socket(AF_UNIX,SOCK_STREAM,0));
	} while (fd()==-1 && error::getErrorNumber()==EINTR);
	return (fd()!=-1);
}

bool unixserversocket::listen(const char *filename, mode_t mask,
							int32_t backlog) {
	initialize(filename,mask);
	return (bind() && listen(backlog));
}

bool unixserversocket::bind() {

	// set umask and store old umask
	mode_t	oldmask=process::setFileCreationMask(pvt->_mask);

	// bind the socket
	bool	retval=true;
	int32_t	result;
	do {
		result=::bind(fd(),
			reinterpret_cast<struct sockaddr *>(_sun()),
				sizeof(sockaddr_un));
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result==-1) {
		retval=false;
	}

	// restore old umask
	process::setFileCreationMask(oldmask);

	return retval;
}

bool unixserversocket::listen(int32_t backlog) {
	int32_t	result;
	do {
		result=::listen(fd(),backlog);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

filedescriptor *unixserversocket::accept() {

	// initialize a socket address structure
	sockaddr_un	clientsun;
	socklen_t	size=sizeof(clientsun);
	rawbuffer::zero(&clientsun,sizeof(clientsun));

	// accept on the socket
	int32_t	clientsock;
	do {
		clientsock=::accept(fd(),
				reinterpret_cast<struct sockaddr *>(
								&clientsun),
				&size);
	} while (clientsock==-1 && error::getErrorNumber()==EINTR);
	if (clientsock==-1) {
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