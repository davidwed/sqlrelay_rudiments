// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocketutil.h>

// for NULL
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

inetsocketutil::inetsocketutil() {
	initialize(NULL,0);
}

inetsocketutil::~inetsocketutil() {}

void inetsocketutil::initialize(const char *address, unsigned short port) {
	this->address=address;
	this->port=port;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
