// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline unixserversocket::unixserversocket() : unixsocket(), serversocket() {
	mask=0;
}

inline int	unixserversocket::listenOnSocket(const char *filename,
						mode_t mask, int backlog) {
	initialize(filename,mask);
	return bind() && listen(backlog);
}
