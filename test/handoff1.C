// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/permissions.h>
#include <stdio.h>

int main(int argc, const char **argv) {

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

		pool.waitForNonBlockingRead(-1,-1);
		int	fd=-1;
		pool.getReadyList()->getDataByIndex(0,&fd);

		if (fd==serversock.getFileDescriptor()) {
			clientsock=serversock.acceptClientConnection();
		} else if (fd==handoffsock.getFileDescriptor()) {
			handoffclientsock=handoffsock.acceptClientConnection();
		} else {
			printf("error or timeout waiting...\n");
		}

		if (clientsock && handoffclientsock) {
			handoffclientsock->passFileDescriptor(
					clientsock->getFileDescriptor());
			delete clientsock;
			clientsock=NULL;
		}
	}
}
