// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/clientsocket.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <stdio.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifndef EWOULDBLOCK
	#define EWOULDBLOCK	WSAEWOULDBLOCK
#endif

#ifndef EINPROGRESS
	#define EINPROGRESS	WSAEINPROGRESS
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class clientsocketprivate {
	friend class clientsocket;
	private:
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		bool	_nonblockingmode;
		#endif
};

clientsocket::clientsocket() : client() {
	pvt=new clientsocketprivate;
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
	pvt->_nonblockingmode=true;
	#endif
	type("clientsocket");
	winsock::initWinsock();
}

clientsocket::clientsocket(const clientsocket &c) : client(c) {
	pvt=new clientsocketprivate;
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
	pvt->_nonblockingmode=c.pvt->_nonblockingmode;
	#endif
	type("clientsocket");
	winsock::initWinsock();
}

clientsocket &clientsocket::operator=(const clientsocket &c) {
	if (this!=&c) {
		client::operator=(c);
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		pvt->_nonblockingmode=c.pvt->_nonblockingmode;
		#endif
	}
	return *this;
}

clientsocket::~clientsocket() {
	delete pvt;
}

bool clientsocket::supportsBlockingNonBlockingModes() {
	#ifdef FIONBIO
		return true;
	#else
		return filedescriptor::supportsBlockingNonBlockingModes();
	#endif
}

bool clientsocket::useNonBlockingMode() const {
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

bool clientsocket::useBlockingMode() const {
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

bool clientsocket::isUsingNonBlockingMode() const {
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

int32_t clientsocket::ioCtl(int32_t cmd, void *arg) const {
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

#ifdef RUDIMENTS_HAS_SSL
BIO *clientsocket::newSSLBIO() const {
	return BIO_new_socket(fd(),BIO_NOCLOSE);
}
#endif

int32_t clientsocket::connect(const struct sockaddr *addr, socklen_t addrlen,
							long sec, long usec) {

#ifndef RUDIMENTS_HAVE_WSACONNECT

	int32_t	retval;
	if (sec==-1 || usec==-1) {

		// if no timeout was passed in, just do a plain vanilla connect
		retval=(::connect(fd(),addr,addrlen)==-1)?
				RESULT_ERROR:RESULT_SUCCESS;

		// FIXME: handle errno is EINTR...
		// on non-linux systems the connect is still in progress and we
		// need to use select() to determine when it has
		// succeeded/failed,
		// on linux systems, the connect may be re-called
		// will select() work on linux?

	} else {

		// if a timeout was passed in then we need to do some more
		// complex stuff...

		// put the socket in non-blocking mode, if necessary
		bool	wasusingnonblockingmode=isUsingNonBlockingMode();
		if (!wasusingnonblockingmode && !useNonBlockingMode()) {
			return RESULT_ERROR;
		}

		// call connect()
		if (::connect(fd(),addr,addrlen)==-1) {

			// FIXME: handle errno is EINTR...
			// if connect() fails and errno is set to one of these
			// values, then the connection is in progress
			if (error::getErrorNumber()==EINPROGRESS ||
				error::getErrorNumber()==EWOULDBLOCK) {

				// Wait for the socket to become writable.  If
				// the select() errors or times out then return
				// the error or timeout.
				// FIXME: use waitForNonBlockingWrite() and
				// add a useListenerInsideConnect() method?
				if ((retval=safeSelect(sec,usec,
							false,true))<0) {
					return retval;
				}

				// If the select() succeeds then we need to see
				// whether the connection was successful or not,
				// there are 2 ways to do this, the first is
				// more efficient, the second is more
				// portable...

			#define USE_GETSOCKOPT_SO_ERROR 1
			#ifdef USE_GETSOCKOPT_SO_ERROR

				// Check for an error using
				// getsockopt(,,SO_ERROR,,)
				int32_t		error=0;
				socklen_t	errorsize=sizeof(error);

				// Some platforms cause getsockopt() to fail
				// and set errno to the error that caused
				// connect() to fail.
				if (getSockOpt(SOL_SOCKET,SO_ERROR,
						&error,&errorsize)==-1) {
					return RESULT_ERROR;
				}

				// Other platforms (correctly) return the error
				// in the buffer that was passed in to
				// getsockopt().
				if (error) {
					error::setErrorNumber(error);
					return RESULT_ERROR;
				}

			#else

				// If getpeername() fails then the
				// connect was unsuccessful.
				struct sockaddr	peeraddr;
				socklen_t	size=sizeof(peeraddr);
				int32_t	result;
				do {
					rawbuffer::zero(&peeraddr,
							sizeof(peeraddr));
					result=getpeername(fd(),
							&peeraddr,&size);
				} while (result==-1 &&
						error::getErrorNumber()==EINTR);
				if (result==-1) {

					// On some platforms, getpeername()
					// will fail and set errno to the
					// error that caused connect() to fail,
					// on other platforms, it will fail
					// with an ENOTCONN and we need to do
					// a read() (which will fail) to find
					// out what the error that caused
					// connect() to fail was.  It's tempting
					// to read 0 bytes, but that will
					// actually return 0 (success) on linux
					// and not set errno.
					if (error::getErrorNumber()==ENOTCONN) {
						char	ch;
						::read(fd(),&ch,sizeof(ch));
					}
					return RESULT_ERROR;
				}

			#endif

				retval=RESULT_SUCCESS;

			} else {
				return RESULT_ERROR;
			}
		} else {
			retval=RESULT_SUCCESS;
		}
		// restore blocking mode, if necessary
		if (!wasusingnonblockingmode && !useBlockingMode()) {
			return RESULT_ERROR;
		}

	}

#else

	int32_t	retval;
	if (sec==-1 || usec==-1) {

		// if no timeout was passed in, just do a plain vanilla connect
		retval=(WSAConnect(fd(),addr,addrlen,
					NULL,NULL,NULL,NULL)==SOCKET_ERROR)?
						RESULT_ERROR:RESULT_SUCCESS;

	} else {

		// if a timeout was passed in then we need to do some more
		// complex stuff...

		// find out if the socket was in non-blocking mode already
		bool	wasusingnonblockingmode=isUsingNonBlockingMode();

		// declare some variables
		WSAEVENT		ev;
		int			er;
		DWORD			milli;
		DWORD			en;
		WSANETWORKEVENTS	ne;

		// initialize return value
		retval=RESULT_ERROR;
		
		// create an event handler
		ev=WSACreateEvent();
		if (ev==WSA_INVALID_EVENT) {
			goto cleanup;
		}

		// create an event selector for connect events
		er=WSAEventSelect(fd(),ev,FD_CONNECT);
		if (er==SOCKET_ERROR) {
			goto cleanup;
		}

		// that event selector will put the socket in
		// non-blocking mode...
		pvt->_nonblockingmode=true;

		// attempt to connect
		er=WSAConnect(fd(),addr,addrlen,NULL,NULL,NULL,NULL);
		if (!er) {
			// the connect succeeded immediately
			retval=RESULT_SUCCESS;
			goto cleanup;
		}

		// If we got a "would block" then the connection is in progress
		// and we need to wait.  Otherwise there was some other error
		// and we need to bail.
		if (WSAGetLastError()!=WSAEWOULDBLOCK) {
			goto cleanup;
		}

		// wait for the connect to succeed or fail
		milli=(sec*1000)+(usec/1000);
		en=WSAWaitForMultipleEvents(1,&ev,FALSE,milli,FALSE);
		if (en!=WSA_WAIT_EVENT_0) {
			// a timeout occurred
			retval=RESULT_TIMEOUT;
			goto cleanup;
		}

		// get the events that occurred
		er=WSAEnumNetworkEvents(fd(),ev,&ne);
		if (er) {
			goto cleanup;
		}

		// if there was an error then set errno,
		// otherwise the connect succeeded...
		er=ne.iErrorCode[FD_CONNECT_BIT];
		if (er) {
			error::setErrorNumber(er);
		} else {
			retval=RESULT_SUCCESS;
		}

cleanup:
		// clean up
		WSAEventSelect(fd(),ev,0);
		WSACloseEvent(ev);

		// The WSAEventSelect for FD_CONNECT above put the socket in
		// non-blocking mode.  If necessary, we need to set it back to
		// blocking mode here.
		if (!wasusingnonblockingmode && !useBlockingMode()) {
			return RESULT_ERROR;
		}
	}
	#endif

	#ifdef RUDIMENTS_HAS_SSL
	if (retval==RESULT_SUCCESS) {
		if (!ctx()) {
			return RESULT_SUCCESS;
		}
		if (initializeSSL()) {
			sslresult(SSL_connect(ssl()));
			if (sslresult()==1) {
				return RESULT_SUCCESS;
			}
		}
		close();
		return RESULT_ERROR;
	}
	#endif

	return retval;
}

ssize_t clientsocket::lowLevelRead(void *buf, ssize_t count) const {
	return ::recv(fd(),
			#ifdef RUDIMENTS_HAVE_RECV_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

ssize_t clientsocket::lowLevelWrite(const void *buf, ssize_t count) const {
	return ::send(fd(),
			#ifdef RUDIMENTS_HAVE_SEND_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

int32_t clientsocket::lowLevelClose() {
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
