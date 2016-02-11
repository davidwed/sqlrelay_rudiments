// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/socketclient.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAS_SSL
	// Redhat 6.2 needs _GNU_SOURCE
	#ifndef _GNU_SOURCE
		#define _GNU_SOURCE
	#endif
	#include <openssl/ssl.h>
#endif

#include <stdio.h>

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

class socketclientprivate {
	friend class socketclient;
	private:
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		bool	_nonblockingmode;
		#endif
};

socketclient::socketclient() : client() {
	pvt=new socketclientprivate;
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
	pvt->_nonblockingmode=false;
	#endif
	type("socketclient");
	winsock::initWinsock();
}

socketclient::socketclient(const socketclient &c) : client(c) {
	pvt=new socketclientprivate;
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
	pvt->_nonblockingmode=c.pvt->_nonblockingmode;
	#endif
	type("socketclient");
	winsock::initWinsock();
}

socketclient &socketclient::operator=(const socketclient &c) {
	if (this!=&c) {
		client::operator=(c);
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		pvt->_nonblockingmode=c.pvt->_nonblockingmode;
		#endif
	}
	return *this;
}

socketclient::~socketclient() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class, especially on Windows where
	// closesocket() must be called rather than close() to prevent a crash.
	// If close() is called here, it will eventually call this method's
	// lowLevelClose() rather than filedescriptor::lowLevelClose().
	close();
	delete pvt;
}

bool socketclient::supportsBlockingNonBlockingModes() {
	#if defined(FIONBIO) && !defined(RUDIMENTS_DISABLE_FIONBIO)
		return true;
	#else
		return filedescriptor::supportsBlockingNonBlockingModes();
	#endif
}

bool socketclient::useNonBlockingMode() const {
	// The posix way of setting blocking/non-blocking mode is to use
	// fcntl, which is what the filedescriptor class does, but this doesn't
	// work for sockets on all platforms.  If FIONBIO is defined, then use
	// it with an ioctl instead.
	#if defined(FIONBIO) && !defined(RUDIMENTS_DISABLE_FIONBIO)
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

bool socketclient::useBlockingMode() const {
	// The posix way of setting blocking/non-blocking mode is to use
	// fcntl, which is what the filedescriptor class does, but this doesn't
	// work for sockets on all platforms.  If FIONBIO is defined, then use
	// it with an ioctl instead.
	#if defined(FIONBIO) && !defined(RUDIMENTS_DISABLE_FIONBIO)
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

bool socketclient::isUsingNonBlockingMode() const {
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

int32_t socketclient::ioCtl(int32_t cmd, void *arg) const {
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		int32_t	result;
		error::clearError();
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
void *socketclient::newSSLBIO() const {
	return (void *)BIO_new_socket(fd(),BIO_NOCLOSE);
}
#endif

int32_t socketclient::connect(const struct sockaddr *addr,
						socklen_t addrlen,
						int32_t sec, int32_t usec) {

#ifndef RUDIMENTS_HAVE_WSACONNECT

	// variables...
	int32_t		retval;
	bool		wasusingnonblockingmode=false;
	int32_t		error;
	socklen_t	errorsize;
	#define USE_GETSOCKOPT_SO_ERROR 1
	#ifndef USE_GETSOCKOPT_SO_ERROR
	struct sockaddr	peeraddr;
	socklen_t	size;
	int32_t		result;
	char		ch;
	#endif

	// if no timeout was passed in, just do a plain vanilla connect
	if (sec==-1 || usec==-1) {
		retval=(::connect(fd(),
			#ifdef RUDIMENTS_HAVE_CONNECT_WITH_NON_CONST_SOCKADDR
				(struct sockaddr *)addr,
			#else
				addr,
			#endif
				addrlen)==-1)?
				RESULT_ERROR:RESULT_SUCCESS;

		// FIXME: handle errno is EINTR...
		// On non-linux systems the connect is still in progress and we
		// need to use select() to determine when it has
		// succeeded/failed, on linux systems, the connect may be
		// re-called.  Will select() work on linux?

		goto cleanup;
	}

	// if a timeout was passed in then we need to do some more
	// complex stuff...

	// put the socket in non-blocking mode, if necessary
	wasusingnonblockingmode=isUsingNonBlockingMode();
	if (!wasusingnonblockingmode &&
			!useNonBlockingMode() &&
			error::getErrorNumber()
			#ifdef ENOTSUP
			&& error::getErrorNumber()!=ENOTSUP
			#endif
			#ifdef EOPNOTSUPP
			&& error::getErrorNumber()!=EOPNOTSUPP
			#endif
			) {
		retval=RESULT_ERROR;
		goto cleanup;
	}

	// connect...
	if (::connect(fd(),
		#ifdef RUDIMENTS_HAVE_CONNECT_WITH_NON_CONST_SOCKADDR
			(struct sockaddr *)addr,
		#else
			addr,
		#endif
			addrlen)!=-1) {
		retval=RESULT_SUCCESS;
		goto cleanup;
	}

	// FIXME: handle errno is EINTR...
	// if connect() fails and errno is set to one of these values,
	// then the connection is in progress
	if (error::getErrorNumber()!=EINPROGRESS &&
		error::getErrorNumber()!=EWOULDBLOCK) {
		retval=RESULT_ERROR;
		goto cleanup;
	}

	// Wait for the socket to become writable.  If the select()
	// errors or times out then return the error or timeout.
	if ((retval=waitForNonBlockingWrite(sec,usec))<0) {
		retval=RESULT_ERROR;
		goto cleanup;
	}

	// If the select() succeeds then we need to see whether the connection
	// was successful or not, there are 2 ways to do this, the first is
	// more efficient, the second is more portable...

	#ifdef USE_GETSOCKOPT_SO_ERROR

		// Check for an error using getsockopt(,,SO_ERROR,,)
		error=0;
		errorsize=sizeof(error);

		// Some platforms cause getsockopt() to fail and set
		// errno to the error that caused connect() to fail.
		if (getSockOpt(SOL_SOCKET,SO_ERROR,&error,&errorsize)==-1) {
			retval=RESULT_ERROR;
			goto cleanup;
		}

		// Other platforms (correctly) return the error in the
		// buffer that was passed in to getsockopt().
		if (error) {
			error::setErrorNumber(error);
			retval=RESULT_ERROR;
			goto cleanup;
		}

	#else

		// If getpeername() fails then the
		// connect was unsuccessful.
		size=sizeof(peeraddr);
		error::clearError();
		do {
			bytestring::zero(&peeraddr,size);
			result=getpeername(fd(),&peeraddr,&size);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (result==-1) {

			// On some platforms, getpeername() will fail and set
			// errno to the error that caused connect() to fail, on
			// other platforms, it will fail with an ENOTCONN and
			// we need to do a read() (which will fail) to find out
			// what the error that caused connect() to fail was.
			// It's tempting to read 0 bytes, but that will actually
			// return 0 (success) on linux and not set errno.
			if (error::getErrorNumber()==ENOTCONN) {
				::read(fd(),&ch,sizeof(ch));
			}
			retval=RESULT_ERROR;
			goto cleanup;
		}

	#endif

	// if we made it here then everything went well
	retval=RESULT_SUCCESS;

cleanup:

	// save any error that might have occurred, as a
	// successful call to useBlockingMode() will mask it
	int32_t	err=error::getErrorNumber();

	// restore blocking mode, if necessary
	if (!wasusingnonblockingmode &&
			!useBlockingMode() &&
			error::getErrorNumber()
			#ifdef ENOTSUP
			&& error::getErrorNumber()!=ENOTSUP
			#endif
			#ifdef EOPNOTSUPP
			&& error::getErrorNumber()!=EOPNOTSUPP
			#endif
			) {
		return RESULT_ERROR;
	}

	// restore the error
	if (err) {
		error::setErrorNumber(err);
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
			goto wsacleanup;
		}

		// create an event selector for connect events
		er=WSAEventSelect(fd(),ev,FD_CONNECT);
		if (er==SOCKET_ERROR) {
			goto wsacleanup;
		}

		// that event selector will put the socket in
		// non-blocking mode...
		pvt->_nonblockingmode=true;

		// attempt to connect
		er=WSAConnect(fd(),addr,addrlen,NULL,NULL,NULL,NULL);
		if (!er) {
			// the connect succeeded immediately
			retval=RESULT_SUCCESS;
			goto wsacleanup;
		}

		// If we got a "would block" then the connection is in progress
		// and we need to wait.  Otherwise there was some other error
		// and we need to bail.
		if (WSAGetLastError()!=WSAEWOULDBLOCK) {
			goto wsacleanup;
		}

		// wait for the connect to succeed or fail
		milli=(sec*1000)+(usec/1000);
		en=WSAWaitForMultipleEvents(1,&ev,FALSE,milli,FALSE);
		if (en!=WSA_WAIT_EVENT_0) {
			// a timeout occurred
			retval=RESULT_TIMEOUT;
			goto wsacleanup;
		}

		// get the events that occurred
		er=WSAEnumNetworkEvents(fd(),ev,&ne);
		if (er) {
			goto wsacleanup;
		}

		// if there was an error then set errno,
		// otherwise the connect succeeded...
		er=ne.iErrorCode[FD_CONNECT_BIT];
		if (er) {
			error::setErrorNumber(er);
		} else {
			retval=RESULT_SUCCESS;
		}

wsacleanup:
		// clean up
		WSAEventSelect(fd(),ev,0);
		WSACloseEvent(ev);

		// The WSAEventSelect for FD_CONNECT above put the socket in
		// non-blocking mode.  If necessary, we need to set it back to
		// blocking mode here.
		if (!wasusingnonblockingmode &&
				!useBlockingMode() &&
				error::getErrorNumber()
				#ifdef ENOTSUP
				&& error::getErrorNumber()!=ENOTSUP
				#endif
				#ifdef EOPNOTSUPP
				&& error::getErrorNumber()!=EOPNOTSUPP
				#endif
				) {
			return RESULT_ERROR;
		}
	}
	#endif

	// SSL-connect if necessary
	#ifdef RUDIMENTS_HAS_SSL
	if (retval==RESULT_SUCCESS && sslctx()) {
		if (!initializeSSL()) {
			close();
			return RESULT_ERROR;
		}
		sslresult(SSL_connect((SSL *)ssl()));
		if (sslresult()!=1) {
			close();
			return RESULT_ERROR;
		}
	}
	#endif

	// securitycontext-connect if necessary
	if (retval==RESULT_SUCCESS && secctx()) {
		secctx()->setFileDescriptor(this);
		if (!secctx()->connect()) {
			secctx()->setFileDescriptor(NULL);
			close();
			return RESULT_ERROR;
		}
	}

	return retval;
}

ssize_t socketclient::lowLevelRead(void *buf, ssize_t count) {
	return ::recv(fd(),
			#ifdef RUDIMENTS_HAVE_RECV_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

ssize_t socketclient::lowLevelWrite(const void *buf, ssize_t count) const {
	return ::send(fd(),
			#ifdef RUDIMENTS_HAVE_SEND_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

int32_t socketclient::lowLevelClose() {
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
