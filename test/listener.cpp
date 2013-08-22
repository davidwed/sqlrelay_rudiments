// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/unixserversocket.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {


	// listen on inet socket port 1800
	inetserversocket	inetsock;
	if (!inetsock.listen(NULL,8000,15)) {
		stdoutput.printf("couldn't listen on inet socket\n");
	}


	// listen on unix socket "/tmp/lsnr.socket"
	unixserversocket	unixsock;
	if (!unixsock.listen("/tmp/lsnr.socket",0000,15)) {
		stdoutput.printf("couldn't listen on unix socket\n");
	}


	// create a listener and add the 2 sockets to it
	listener	pool;
	pool.addFileDescriptor(&inetsock);
	pool.addFileDescriptor(&unixsock);


	// loop...
	for (;;) {

		// wait for a client to connect to one of the sockets
		pool.waitForNonBlockingRead(-1,-1);
		filedescriptor	*fd=NULL;
		pool.getReadyList()->getDataByIndex(0,&fd);

		// figure out which socket the client connected to
		filedescriptor	*clientsock;
		if (fd==&inetsock) {
			clientsock=inetsock.accept();
			stdoutput.printf("inetsock: ");
		} else if (fd==&unixsock) {
			clientsock=unixsock.accept();
			stdoutput.printf("unixsock: ");
		} else {
			stdoutput.printf("error or timeout waiting...\n");
			continue;
		}


		// read 5 bytes from the client and display it
		char	buffer[6];
		buffer[5]=(char)NULL;
		clientsock->read(buffer,5);
		stdoutput.printf("%s\n",buffer);


		// write "hello" back to the client
		clientsock->write("hello",5);


		// close the socket and clean up
		clientsock->close();
		delete clientsock;
	}
}
