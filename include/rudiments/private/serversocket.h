// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <rudiments/transport.h>

class serversocket : virtual public transport {
	public:
		int	lingerOnClose(int timeout);
		int	dontLingerOnClose();
		int	reuseAddresses();
		int	dontReuseAddresses();
		virtual	int	bind()=0;
		virtual	int	listen(int backlog);
	private:
		int	setLingerOnClose(int timeout, int onoff);
		int	setReuseAddresses(int onoff);
};


inline int	serversocket::lingerOnClose(int timeout) {
	return setLingerOnClose(timeout,1);
}

inline int	serversocket::dontLingerOnClose() {
	return setLingerOnClose(0,1);
}

inline int	serversocket::reuseAddresses() {
	return setReuseAddresses(1);
}

inline int	serversocket::dontReuseAddresses() {
	return setReuseAddresses(0);
}

#endif
