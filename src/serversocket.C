// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/serversocket.h>

#include <sys/types.h>
#include <sys/socket.h>

int	serversocket::setLingerOnClose(int timeout, int onoff) {
	struct	linger	ling;
	ling.l_onoff=onoff;
	ling.l_linger=timeout;
	return !setsockopt(filedescriptor,SOL_SOCKET,SO_LINGER,
				(void *)&ling,sizeof(struct linger));
}

int	serversocket::setReuseAddresses(int onoff) {
	int	value=onoff;
	return !setsockopt(filedescriptor,SOL_SOCKET,SO_REUSEADDR,
			(void *)&value,(socklen_t)sizeof(int));
}

int	serversocket::listen(int backlog) {
	return !::listen(filedescriptor,backlog);
}
