// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE unixsocket::unixsocket() : datatransport() {
	filename=NULL;
}

RUDIMENTS_INLINE unixsocket::unixsocket(int filedescriptor) :
					datatransport(filedescriptor) {
	filename=NULL;
}

RUDIMENTS_INLINE void unixsocket::initialize(const char *filename) {
	this->filename=(char *)filename;
}
