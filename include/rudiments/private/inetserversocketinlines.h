// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline inetserversocket::inetserversocket() : serversocket(), inetsocket() {}

inline int	inetserversocket::listenOnSocket(const char *address,
					unsigned short port, int backlog) {
	initialize(address,port);
	return bind() && listen(backlog);
}

inline int	inetserversocket::lingerOnClose(int timeout) {
	return serversocket::lingerOnClose(timeout);
}

inline int	inetserversocket::dontLingerOnClose() {
	return serversocket::dontLingerOnClose();
}

inline int	inetserversocket::reuseAddresses() {
	return serversocket::reuseAddresses();
}

inline int	inetserversocket::dontReuseAddresses() {
	return serversocket::dontReuseAddresses();
}

inline int	inetserversocket::listen(int backlog) {
	return serversocket::listen(backlog);
}

inline unsigned short	inetserversocket::getPort() {
	return port;
}
