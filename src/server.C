// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/server.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

server::server() : filedescriptor() {
	type="server";
}

server::server(const server &s) : filedescriptor(s) {
	type="server";
}

server &server::operator=(const server &s) {
	if (this!=&s) {
		filedescriptor::operator=(s);
	}
	return *this;
}

server::~server() {}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
