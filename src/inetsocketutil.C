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

inetsocketutil::inetsocketutil(const inetsocketutil &i) {
	initialize(i.address,i.port);
}

inetsocketutil &inetsocketutil::operator=(const inetsocketutil &i) {
	if (this!=&i) {
		initialize(i.address,i.port);
	}
	return *this;
}

inetsocketutil::~inetsocketutil() {}

void inetsocketutil::initialize(const char *address, unsigned short port) {
	this->address=address;
	this->port=port;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
