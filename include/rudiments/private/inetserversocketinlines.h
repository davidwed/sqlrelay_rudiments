// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline inetserversocket::inetserversocket() : serversocket(), inetsocket() {}

inline int	inetserversocket::listenOnSocket(const char *address,
					unsigned short port, int backlog) {
	initialize(address,port);
	return bind() && listen(backlog);
}

inline unsigned short	inetserversocket::getPort() {
	return port;
}
