// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/permissions.h>

int	main() {

	inetserversocket	svr;
	svr.listenOnSocket(NULL,11000,15);

	listener		svrp;
	svrp.addFileDescriptor(svr.getFileDescriptor());

	for (;;) {
		svrp.waitForData(-1,-1);
		inetsocket	*clientsock=svr.acceptClientConnection();
		clientsock->write("hello");
		clientsock->close();
		delete clientsock;
	}
}
