// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/permissions.h>
#include <stdio.h>

int	main() {

	inetserversocket	serversock;
	serversock.listenOnSocket(NULL,10000,15);

	unixserversocket	handoffsock;
	handoffsock.listenOnSocket("/tmp/handoff.socket",0000,15);

	listener	pool;
	pool.addFileDescriptor(serversock.getFileDescriptor());
	pool.addFileDescriptor(handoffsock.getFileDescriptor());

	inetsocket	*clientsock=NULL;
	unixsocket	*handoffclientsock=NULL;

	for (;;) {

		int	fd=pool.waitForNonBlockingRead(-1,-1);
		if (fd==serversock.getFileDescriptor()) {
			clientsock=serversock.acceptClientConnection();
		} else {
			handoffclientsock=handoffsock.acceptClientConnection();
		}

		if (clientsock && handoffclientsock) {
			handoffclientsock->passFileDescriptor(
					clientsock->getFileDescriptor());
			delete clientsock;
			clientsock=NULL;
		}
	}
}
