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
};

serversocket::serversocket() : server() {
	pvt=new serversocketprivate;
	winsock::initWinsock();
	type("serversocket");
}

serversocket::serversocket(const serversocket &s) : server(s) {
	pvt=new serversocketprivate;
	winsock::initWinsock();
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
	return (ioCtl(FIONBIO,&nonblocking)!=-1);
#else
	return false;
#endif
}

bool serversocket::useBlockingMode() const {
#ifdef FIONBIO
	int	nonblocking=0;
	return (ioCtl(FIONBIO,&nonblocking)!=-1);
#else
	return false;
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
	int	value=onoff;
	return !setSockOpt(SOL_SOCKET,SO_REUSEADDR,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&value,
					(socklen_t)sizeof(int));
}

bool serversocket::listen(int32_t backlog) {
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
	#ifdef RUDIMENTS_HAVE_CLOSESOCKET
		return closesocket(fd());
	#else
		return ::close(fd());
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
