// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/inetsocket.h>

inetsocket::inetsocket() : filedescriptor(), datatransport(), socket() {
	initialize(NULL,0);
}

inetsocket::~inetsocket() {}

inetsocket::inetsocket(int filedesc) :
	filedescriptor(filedesc), datatransport(filedesc), socket(filedesc) {
	initialize(NULL,0);
}

void inetsocket::initialize(const char *address, unsigned short port) {
	this->address=(char *)address;
	this->port=port;
}
