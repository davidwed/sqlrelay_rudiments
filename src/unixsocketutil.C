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

unixsocketutil::unixsocketutil(const unixsocketutil &u) {
	filename=u.filename;
	sockaddrun=u.sockaddrun;
}

unixsocketutil &unixsocketutil::operator=(const unixsocketutil &u) {
	if (this!=&u) {
		filename=u.filename;
		sockaddrun=u.sockaddrun;
	}
	return *this;
}

unixsocketutil::~unixsocketutil() {}

void unixsocketutil::initialize(const char *filename) {
	this->filename=filename;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
