// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE bool serversocket::dontLingerOnClose() {
	return setLingerOnClose(0,1);
}

RUDIMENTS_INLINE bool serversocket::reuseAddresses() {
	return setReuseAddresses(1);
}

RUDIMENTS_INLINE bool serversocket::dontReuseAddresses() {
	return setReuseAddresses(0);
}
