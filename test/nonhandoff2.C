// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetserversocket.h>
#include <rudiments/permissions.h>

int main(int argc, const char **argv) {

	inetserversocket	svr;
	svr.listenOnSocket(NULL,11000,15);

	for (;;) {
		svr.waitForNonBlockingRead(-1,-1);
		inetsocket	*clientsock=svr.acceptClientConnection();
		clientsock->write("hello");
		clientsock->close();
		delete clientsock;
	}
}
