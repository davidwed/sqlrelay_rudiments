// Copyright (c) 2002 David Muse
// See the COPYING file for more information

INLINE unixsocket::unixsocket() : datatransport() {
	filename=NULL;
}

INLINE unixsocket::unixsocket(int filedescriptor) :
				datatransport(filedescriptor) {
	filename=NULL;
}

INLINE void unixsocket::initialize(const char *filename) {
	this->filename=(char *)filename;
}
