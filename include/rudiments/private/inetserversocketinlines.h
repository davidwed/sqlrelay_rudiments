// Copyright (c) 2002 David Muse
// See the COPYING file for more information

INLINE inetserversocket::inetserversocket() : serversocket(), inetsocket() {}

INLINE int inetserversocket::listenOnSocket(const char *address,
					unsigned short port, int backlog) {
	initialize(address,port);
	return bind() && listen(backlog);
}

INLINE unsigned short inetserversocket::getPort() {
	return port;
}
