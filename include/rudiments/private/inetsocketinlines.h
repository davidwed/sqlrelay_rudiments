// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

INLINE inetsocket::inetsocket() : datatransport() {
	initialize(NULL,0);
}

INLINE inetsocket::inetsocket(int filedescriptor) :
				datatransport(filedescriptor) {
	initialize(NULL,0);
}

INLINE void inetsocket::initialize(const char *address,
					unsigned short port) {
	this->address=(char *)address;
	this->port=port;
}
