// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/permissions.h>
#include <stdio.h>

int main(int argc, const char **argv) {


	// listen on inet socket port 1800
	inetserversocket	inetsock;
	if (!inetsock.listenOnSocket(NULL,8000,15)) {
		printf("couldn't listen on inet socket\n");
	}


	// listen on unix socket "/tmp/lsnr.socket"
	unixserversocket	unixsock;
	if (!unixsock.listenOnSocket("/tmp/lsnr.socket",0000,15)) {
		printf("couldn't listen on unix socket\n");
	}


	// create a listener and add the 2 sockets to it
	listener	pool;
	pool.addFileDescriptor(inetsock.getFileDescriptor());
	pool.addFileDescriptor(unixsock.getFileDescriptor());


	// loop...
	for (;;) {

		// wait for a client to connect to one of the sockets
		int	fd=pool.waitForNonBlockingRead(-1,-1);


		// figure out which socket the client connected to
		datatransport	*clientsock;
		if (fd==inetsock.getFileDescriptor()) {
			clientsock=inetsock.acceptClientConnection();
			printf("inetsock: ");
		} else if (fd==unixsock.getFileDescriptor()) {
			clientsock=unixsock.acceptClientConnection();
			printf("unixsock: ");
		} else {
			printf("error or timeout waiting...\n");
			continue;
		}


		// read 5 bytes from the client and display it
		char	buffer[6];
		buffer[5]=(char)NULL;
		clientsock->read(buffer,5);
		printf("%s\n",buffer);


		// write "hello" back to the client
		clientsock->write("hello",5);


		// close the socket and clean up
		clientsock->close();
		delete clientsock;
	}
}
