// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixsocketserver.h>
#include <rudiments/unixsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
	#include <rudiments/inetsocketserver.h>
#endif

#include <rudiments/private/winsock.h>

class unixsocketserverprivate {
	friend class unixsocketserver;
	private:
		mode_t	_mask;
		#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
			inetsocketserver	_iss;
		#endif
};

unixsocketserver::unixsocketserver() : socketserver(), unixsocketutil() {
	pvt=new unixsocketserverprivate;
	pvt->_mask=0;
	#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
		translateByteOrder();
	#endif
	type("unixsocketserver");
}

unixsocketserver::unixsocketserver(const unixsocketserver &u) :
				socketserver(u), unixsocketutil(u) {
	pvt=new unixsocketserverprivate;
	pvt->_mask=u.pvt->_mask;
	type("unixsocketserver");
}

unixsocketserver &unixsocketserver::operator=(const unixsocketserver &u) {
	if (this!=&u) {
		socketserver::operator=(u);
		unixsocketutil::operator=(u);
		pvt->_mask=u.pvt->_mask;
		#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
			pvt->_iss=u.pvt->_iss;
		#endif
	}
	return *this;
}

unixsocketserver::~unixsocketserver() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class, especially on Windows where
	// closesocket() must be called rather than close() to prevent a crash.
	// If close() is called here, it will eventually call this method's
	// lowLevelClose() rather than filedescriptor::lowLevelClose().
	#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
		// However, on windows, we're faking unix sockets using inet
		// sockets, and the actual inet socket will be closed during
		// the delete below, so don't close anything here, just set the
		// filedescriptor to -1 so the filedescriptor destructor
		// doesn't try to close anything when it runs.
		fd(-1);
	#else
		close();
	#endif
	delete pvt;
}

bool unixsocketserver::initialize(const char *filename, mode_t mask) {

	unixsocketutil::initialize(filename);
	pvt->_mask=mask;

	// if a null or blank port was specified, return an error
	if (charstring::isNullOrEmpty(filename)) {
		return false;
	}

#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
	if (pvt->_iss.initialize("127.0.0.1",filenameToPort(filename))) {
		fd(pvt->_iss.getFileDescriptor());
		return true;
	}
	return false;
#else

	// init the socket structure
	file::remove(filename);
	bytestring::zero(_sun(),sizeof(sockaddr_un));
	_sun()->sun_family=AF_UNIX;
	charstring::copy(_sun()->sun_path,filename);

	// create the socket
	error::clearError();
	do {
		fd(::socket(AF_UNIX,SOCK_STREAM,0));
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
#endif
}

bool unixsocketserver::listen(const char *filename, mode_t mask,
							int32_t backlog) {
	initialize(filename,mask);
	return (bind() && listen(backlog));
}

bool unixsocketserver::bind() {

#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
	return pvt->_iss.bind();
#else
	// set umask and store old umask
	mode_t	oldmask=process::setFileCreationMask(pvt->_mask);

	// bind the socket
	bool	retval=true;
	int32_t	result;
	error::clearError();
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
#endif
}

bool unixsocketserver::listen(int32_t backlog) {

#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
	return pvt->_iss.listen(backlog);
#else
	int32_t	result;
	error::clearError();
	do {
		result=::listen(fd(),backlog);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
#endif
}

filedescriptor *unixsocketserver::accept() {

#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
	pvt->_iss.setSecurityContext(getSecurityContext());
	return pvt->_iss.accept();
#else
	// initialize a socket address structure
	sockaddr_un			clientsun;
	RUDIMENTS_SOCKLEN_OR_SIZE_T	size=sizeof(clientsun);
	bytestring::zero(&clientsun,sizeof(clientsun));

	// accept on the socket
	int32_t	clientsock;
	error::clearError();
	do {
		clientsock=::accept(fd(),
				reinterpret_cast<struct sockaddr *>(
								&clientsun),
				&size);
	} while (clientsock==-1 && error::getErrorNumber()==EINTR);
	if (clientsock==-1) {
		return NULL;
	}

	unixsocketclient	*returnsock=new unixsocketclient;
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

	// handle securitycontext-accept if necessary
	if (!securityContextAccept(returnsock)) {
		delete returnsock;
		return NULL;
	}

	return returnsock;
#endif
}

bool unixsocketserver::close() {
	#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
		pvt->_iss.close();
		fd(-1);
	#endif
	return filedescriptor::close();
}
