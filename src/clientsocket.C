// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/clientsocket.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class clientsocketprivate {
	friend class clientsocket;
	private:
};

clientsocket::clientsocket() : client() {
	pvt=new clientsocketprivate;
	type("clientsocket");
}

clientsocket::clientsocket(const clientsocket &c) : client(c) {
	pvt=new clientsocketprivate;
	type("clientsocket");
}

clientsocket &clientsocket::operator=(const clientsocket &c) {
	if (this!=&c) {
		client::operator=(c);
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
	return false;
#endif
}

bool clientsocket::useNonBlockingMode() const {
	#ifdef FIONBIO
	int	nonblocking=1;
	return (ioctl(FIONBIO,&nonblocking)!=-1);
	#else
	return false;
	#endif
}

bool clientsocket::useBlockingMode() const {
	#ifdef FIONBIO
	int	nonblocking=0;
	return (ioctl(FIONBIO,&nonblocking)!=-1);
	#else
	return false;
	#endif
}

#ifdef RUDIMENTS_HAS_SSL
BIO *clientsocket::newSSLBIO() const {
	return BIO_new_socket(fd(),BIO_NOCLOSE);
}
#endif

int clientsocket::connect(const struct sockaddr *addr, socklen_t addrlen,
							long sec, long usec) {
	int	retval;
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
				int		error=0;
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
				int	result;
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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
