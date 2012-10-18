// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/serversocket.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	#include <sys/socket.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serversocketprivate {
	friend class serversocket;
	private:
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		bool	_nonblockingmode;
		#endif
};

serversocket::serversocket() : server() {
	pvt=new serversocketprivate;
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
	pvt->_nonblockingmode=false;
	#endif
	type("serversocket");
	winsock::initWinsock();
}

serversocket::serversocket(const serversocket &s) : server(s) {
	pvt=new serversocketprivate;
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
	pvt->_nonblockingmode=s.pvt->_nonblockingmode;
	#endif
	type("serversocket");
	winsock::initWinsock();
}

serversocket &serversocket::operator=(const serversocket &s) {
	if (this!=&s) {
		server::operator=(s);
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		pvt->_nonblockingmode=true;
		#endif
	}
	return *this;
}

serversocket::~serversocket() {
	delete pvt;
}

bool serversocket::supportsBlockingNonBlockingModes() {
	#ifdef FIONBIO
		return true;
	#else
		return filedescriptor::supportsBlockingNonBlockingModes();
	#endif
}

bool serversocket::useNonBlockingMode() const {
	// The posix way of setting blocking/non-blocking mode is to use
	// fcntl, which is what the filedescriptor class does, but this doesn't
	// work for sockets on all platforms.  If FIONBIO is defined, then use
	// it with an ioctl instead.
	#ifdef FIONBIO
		int32_t	nonblocking=1;
		bool	retval=(ioCtl(FIONBIO,&nonblocking)!=-1);
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		if (retval) {
			pvt->_nonblockingmode=true;
		}
		#endif
		return retval;
	#else
		return filedescriptor::useNonBlockingMode();
	#endif
}

bool serversocket::useBlockingMode() const {
	// The posix way of setting blocking/non-blocking mode is to use
	// fcntl, which is what the filedescriptor class does, but this doesn't
	// work for sockets on all platforms.  If FIONBIO is defined, then use
	// it with an ioctl instead.
	#ifdef FIONBIO
		int32_t	nonblocking=0;
		bool	retval=(ioCtl(FIONBIO,&nonblocking)!=-1);
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		if (retval) {
			pvt->_nonblockingmode=false;
		}
		#endif
		return retval;
	#else
		return filedescriptor::useBlockingMode();
	#endif
}

bool serversocket::isUsingNonBlockingMode() const {
	// There is no way to determine the blocking mode using ioctl's and
	// FIONBIO.  On posix platforms, independent of whether blocking mode
	// was set using an ioctl or fcntl, you can use an fcntl to get the
	// blocking mode.  On other platforms, you just have to keep track of
	// what mode you set it to and hope that the program only uses methods
	// from this class to set the mode.
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		return pvt->_nonblockingmode;
	#else
		return filedescriptor::isUsingNonBlockingMode();
	#endif
}

int32_t serversocket::ioCtl(int32_t cmd, void *arg) const {
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		int32_t	result;
		do {
			result=ioctlsocket(fd(),cmd,(u_long *)arg);
		} while (getRetryInterruptedIoctl() && result==-1 &&
				error::getErrorNumber()==EINTR);
		return result;
	#else
		return filedescriptor::ioCtl(cmd,arg);
	#endif
}

bool serversocket::lingerOnClose(int32_t timeout) {
	return setLingerOnClose(timeout,1);
}

bool serversocket::dontLingerOnClose() {
	return setLingerOnClose(0,0);
}

bool serversocket::reuseAddresses() {
	return setReuseAddresses(1);
}

bool serversocket::dontReuseAddresses() {
	return setReuseAddresses(0);
}

bool serversocket::setLingerOnClose(int32_t timeout, int32_t onoff) {
	#ifdef SO_LINGER
		struct	linger	ling;
		ling.l_onoff=onoff;
		ling.l_linger=timeout;
		return !setSockOpt(SOL_SOCKET,SO_LINGER,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&ling,
					sizeof(struct linger));
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool serversocket::setReuseAddresses(int32_t onoff) {
	int32_t	value=onoff;
	return !setSockOpt(SOL_SOCKET,SO_REUSEADDR,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&value,
					(socklen_t)sizeof(int));
}

bool serversocket::listen(int32_t backlog) {
	int32_t	result;
	do {
		result=::listen(fd(),backlog);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

#ifdef RUDIMENTS_HAS_SSL
BIO *serversocket::newSSLBIO() const {
	return BIO_new_socket(fd(),BIO_NOCLOSE);
}

bool serversocket::sslAccept(filedescriptor *sock) {
	if (ctx()) {
		sock->setSSLContext(ctx());
		if (!sock->initializeSSL()) {
			return false;
		}
		sslresult(SSL_accept(sock->getSSL()));
		if (sslresult()!=1) {
			return false;
		}
	}
	return true;
}
#endif

ssize_t serversocket::lowLevelRead(void *buf, ssize_t count) const {
	return ::recv(fd(),
			#ifdef RUDIMENTS_HAVE_RECV_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

ssize_t serversocket::lowLevelWrite(const void *buf, ssize_t count) const {
	return ::send(fd(),
			#ifdef RUDIMENTS_HAVE_SEND_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

int32_t serversocket::lowLevelClose() {
	#if defined(RUDIMENTS_HAVE_CLOSESOCKET)
		return closesocket(fd());
	#elif defined(RUDIMENTS_HAVE__CLOSE)
		return _close(fd());
	#elif defined(RUDIMENTS_HAVE_CLOSE)
		return ::close(fd());
	#else
		#error no closesocket, close or anything like it
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
