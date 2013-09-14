// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/server.h>

class serverprivate {
	friend class server;
	private:
};

server::server() : filedescriptor() {
	pvt=new serverprivate;
	type("server");
}

server::server(const server &s) : filedescriptor(s) {
	pvt=new serverprivate;
	type("server");
}

server &server::operator=(const server &s) {
	if (this!=&s) {
		filedescriptor::operator=(s);
	}
	return *this;
}

server::~server() {
	delete pvt;
}
