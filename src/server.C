// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/server.h>

server::server() : datatransport() {}

server::~server() {}

bool server::bind() {
	return true;
}

bool server::listen(int backlog) {
	return true;
}

bool server::accept() {
	return true;
}
