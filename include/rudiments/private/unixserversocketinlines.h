// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE unixserversocket::unixserversocket() :
					serversocket(), unixsocket() {
	mask=0;
}
