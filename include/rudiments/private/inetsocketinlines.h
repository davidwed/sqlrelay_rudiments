// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

inline inetsocket::inetsocket() : transport() {
	initialize(NULL,0);
}

inline inetsocket::inetsocket(int filedescriptor) : transport(filedescriptor) {
	initialize(NULL,0);
}

inline void	inetsocket::initialize(const char *address,
					unsigned short port) {
	this->address=(char *)address;
	this->port=port;
}
