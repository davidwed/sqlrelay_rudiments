// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE unixserversocket::unixserversocket() :
					unixsocket(), serversocket() {
	mask=0;
}

RUDIMENTS_INLINE int unixserversocket::listenOnSocket(const char *filename,
							mode_t mask,
							int backlog) {
	initialize(filename,mask);
	return bind() && listen(backlog);
}
