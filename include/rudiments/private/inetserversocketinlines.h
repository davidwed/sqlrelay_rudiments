// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE inetserversocket::inetserversocket() :
					serversocket(), inetsocket() {}

RUDIMENTS_INLINE bool inetserversocket::listenOnSocket(const char *address,
							unsigned short port,
							int backlog) {
	initialize(address,port);
	return (bind() && listen(backlog));
}

RUDIMENTS_INLINE unsigned short inetserversocket::getPort() {
	return port;
}
