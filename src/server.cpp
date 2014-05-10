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

bool server::bind() {
	// by default, just return success
	return true;
}

bool server::listen(int32_t backlog) {
	// by default, just return success
	return true;
}

filedescriptor *server::accept() {
	// create a new filedescriptor and set it to the fd() from this instance
	filedescriptor	*retval=new filedescriptor;
	retval->setFileDescriptor(fd());
	return retval;
}
