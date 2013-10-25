// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/unixsocketclient.h>
#include <rudiments/permissions.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	inetsocketserver	serversock;
	serversock.listen(NULL,10000,15);

	unixsocketserver	handoffsock;
	handoffsock.listen("/tmp/handoff.socket",0000,15);

	listener	pool;
	pool.addFileDescriptor(&serversock);
	pool.addFileDescriptor(&handoffsock);

	inetsocketclient	*clientsock=NULL;
	unixsocketclient	*handoffclientsock=NULL;

	for (;;) {

		pool.waitForNonBlockingRead(-1,-1);
		filedescriptor	*fd=NULL;
		pool.getReadyList()->getValueByIndex(0,&fd);

		if (fd==&serversock) {
			clientsock=(inetsocketclient *)
					serversock.accept();
		} else if (fd==&handoffsock) {
			handoffclientsock=(unixsocketclient *)
						handoffsock.accept();
		} else {
			stdoutput.printf("error or timeout waiting...\n");
		}

		if (clientsock && handoffclientsock) {
			if (!handoffclientsock->passFileDescriptor(
					clientsock->getFileDescriptor())) {
				stdoutput.printf("pass failed: %d - %s\n",
						error::getErrorNumber(),
						error::getErrorString());
				process::exit(1);
			}
			delete clientsock;
			clientsock=NULL;
		}
	}
}
