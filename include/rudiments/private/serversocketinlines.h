// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

INLINE int serversocket::dontLingerOnClose() {
	return setLingerOnClose(0,1);
}

INLINE int serversocket::reuseAddresses() {
	return setReuseAddresses(1);
}

INLINE int serversocket::dontReuseAddresses() {
	return setReuseAddresses(0);
}
