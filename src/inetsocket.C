// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocket.h>
#include <rudiments/charstring.h>

inetsocket::inetsocket() : socket() {
	initialize(NULL,0);
}

inetsocket::~inetsocket() {}

inetsocket::inetsocket(int filedesc) : socket(filedesc) {
	initialize(NULL,0);
}

void inetsocket::initialize(const char *address, unsigned short port) {
	this->address=(char *)address;
	this->port=port;
}
