// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/unixsocketutil.h>

// for NULL
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

unixsocketutil::unixsocketutil() {
	initialize(NULL);
}

unixsocketutil::~unixsocketutil() {}

void unixsocketutil::initialize(const char *filename) {
	this->filename=filename;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
