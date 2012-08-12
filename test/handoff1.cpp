// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/inetclientsocket.h>
#include <rudiments/unixclientsocket.h>
#include <rudiments/permissions.h>
#include <rudiments/error.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	inetserversocket	serversock;
	serversock.listen(NULL,10000,15);

	unixserversocket	handoffsock;
	handoffsock.listen("/tmp/handoff.socket",0000,15);

	listener	pool;
	pool.addFileDescriptor(&serversock);
	pool.addFileDescriptor(&handoffsock);

	inetclientsocket	*clientsock=NULL;
	unixclientsocket	*handoffclientsock=NULL;

	for (;;) {

		pool.waitForNonBlockingRead(-1,-1);
		filedescriptor	*fd=NULL;
		pool.getReadyList()->getDataByIndex(0,&fd);

		if (fd==&serversock) {
			clientsock=(inetclientsocket *)
					serversock.accept();
		} else if (fd==&handoffsock) {
			handoffclientsock=(unixclientsocket *)
						handoffsock.accept();
		} else {
			printf("error or timeout waiting...\n");
		}

		if (clientsock && handoffclientsock) {
			if (!handoffclientsock->passFileDescriptor(
					clientsock->getFileDescriptor())) {
				printf("pass failed: %d - %s\n",error::getErrorNumber(),error::getErrorString());
			}
			delete clientsock;
			clientsock=NULL;
		}
	}
}
