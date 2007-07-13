// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/serversocket.h>
#include <rudiments/error.h>

#include <sys/ioctl.h>

#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serversocketprivate {
	friend class serversocket;
	private:
};

serversocket::serversocket() : server() {
	pvt=new serversocketprivate;
	type("serversocket");
}

serversocket::serversocket(const serversocket &s) : server(s) {
	pvt=new serversocketprivate;
	type("serversocket");
}

serversocket &serversocket::operator=(const serversocket &s) {
	if (this!=&s) {
		server::operator=(s);
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
	return false;
#endif
}


bool serversocket::useNonBlockingMode() const {
#ifdef FIONBIO
	int	nonblocking=1;
	return (ioctl(FIONBIO,&nonblocking)!=-1);
#else
	return false;
#endif
}

bool serversocket::useBlockingMode() const {
#ifdef FIONBIO
	int	nonblocking=0;
	return (ioctl(FIONBIO,&nonblocking)!=-1);
#else
	return false;
#endif
}

bool serversocket::lingerOnClose(int timeout) {
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

bool serversocket::setLingerOnClose(int timeout, int onoff) {
	struct	linger	ling;
	ling.l_onoff=onoff;
	ling.l_linger=timeout;
	return !setSockOpt(SOL_SOCKET,SO_LINGER,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&ling,
					sizeof(struct linger));
}

bool serversocket::setReuseAddresses(int onoff) {
	int	value=onoff;
	return !setSockOpt(SOL_SOCKET,SO_REUSEADDR,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&value,
					(socklen_t)sizeof(int));
}

bool serversocket::listen(int backlog) {
	int	result;
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

#ifdef RUDIMENTS_NAMESPACE
}
#endif
