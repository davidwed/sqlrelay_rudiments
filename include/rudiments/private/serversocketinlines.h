// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

inline int	serversocket::dontLingerOnClose() {
	return setLingerOnClose(0,1);
}

inline int	serversocket::reuseAddresses() {
	return setReuseAddresses(1);
}

inline int	serversocket::dontReuseAddresses() {
	return setReuseAddresses(0);
}
