// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline unixsocket::unixsocket() : transport() {
	filename=NULL;
}

inline unixsocket::unixsocket(int filedescriptor) : transport(filedescriptor) {
	filename=NULL;
}

inline void	unixsocket::initialize(const char *filename) {
	this->filename=(char *)filename;
}
