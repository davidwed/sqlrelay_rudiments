// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/socket.h>

socket::socket() : filedescriptor(), datatransport() {}

socket::socket(int filedesc) :
	filedescriptor(filedesc), datatransport(filedesc) {}

#ifdef RUDIMENTS_HAS_SSL
BIO *socket::newSSLBIO() const {
	return BIO_new_socket(fd,BIO_NOCLOSE);
}

bool socket::sslConnect(struct sockaddr *sin, socklen_t length) {
	if (::connect(fd,sin,length)!=-1 &&
		(!ctx || (initializeSSL() && SSL_connect(ssl)==1))) {
		return true;
	}
	close();
	return false;
}

bool socket::sslAccept(socket *sock) {
	if (ctx) {
		sock->setSSLContext(ctx);
		if (!sock->initializeSSL() || SSL_accept(sock->getSSL())!=1) {
			return false;
		}
	}
	return true;
}
#endif
