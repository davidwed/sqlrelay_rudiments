// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/serversocket.h>

#include <unistd.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

serversocket::serversocket() : server() {
	type="serversocket";
}

serversocket::serversocket(const serversocket &s) : server(s) {
	type="serversocket";
}

serversocket &serversocket::operator=(const serversocket &s) {
	if (this!=&s) {
		server::operator=(s);
	}
	return *this;
}

serversocket::~serversocket() {}

#ifdef FIONBIO
bool serversocket::useNonBlockingMode() const {
	int	nonblocking=1;
	return (ioctl(FIONBIO,&nonblocking)!=-1);
}

bool serversocket::useBlockingMode() const {
	int	nonblocking=0;
	return (ioctl(FIONBIO,&nonblocking)!=-1);
}
#endif

bool serversocket::dontLingerOnClose() {
	return setLingerOnClose(0,1);
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
	return !setsockopt(fd,SOL_SOCKET,SO_LINGER,
				(SETSOCKOPT_OPTVAL_TYPE)&ling,
					sizeof(struct linger));
}

bool serversocket::setReuseAddresses(int onoff) {
	int	value=onoff;
	return !setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,
				(SETSOCKOPT_OPTVAL_TYPE)&value,
					(socklen_t)sizeof(int));
}

bool serversocket::listen(int backlog) {
	return !::listen(fd,backlog);
}

#ifdef RUDIMENTS_HAS_SSL
BIO *serversocket::newSSLBIO() const {
	return BIO_new_socket(fd,BIO_NOCLOSE);
}

bool serversocket::sslAccept(filedescriptor *sock) {
	if (ctx) {
		sock->setSSLContext(ctx);
		if (!sock->initializeSSL() ||
			(sslresult=SSL_accept(sock->getSSL()))!=1) {
			return false;
		}
	}
	return true;
}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
