// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocketutil.h>

// for NULL
#include <stdlib.h>

inetsocketutil::inetsocketutil() {
	initialize(NULL,0);
}

inetsocketutil::~inetsocketutil() {}

void inetsocketutil::initialize(const char *address, unsigned short port) {
	this->address=(char *)address;
	this->port=port;
}
