// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/serversocket.h>

#include <sys/types.h>
#include <sys/socket.h>

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
				(void *)&ling,sizeof(struct linger));
}

bool serversocket::setReuseAddresses(int onoff) {
	int	value=onoff;
	return !setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,
			(void *)&value,(socklen_t)sizeof(int));
}

bool serversocket::listen(int backlog) {
	return !::listen(fd,backlog);
}
