// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/inetsocket.h>

inetsocket::inetsocket() : datatransport() {
	initialize(NULL,0);
}

inetsocket::inetsocket(int filedescriptor) : datatransport(filedescriptor) {
	initialize(NULL,0);
}

void inetsocket::initialize(const char *address, unsigned short port) {
	this->address=(char *)address;
	this->port=port;
}
