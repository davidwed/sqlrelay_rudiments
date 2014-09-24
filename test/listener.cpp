// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/listener.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/permissions.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>

int main(int argc, const char **argv) {

	// listen on inet socket port 1800
	inetsocketserver	inetsock;
	bool	inetlisten=inetsock.listen(NULL,8000,15);
	if (!inetlisten) {
		stdoutput.printf("couldn't listen on inet socket\n");
		stdoutput.printf("%s\n",error::getErrorString());
	}


	// listen on unix socket "lsnr.sck"
	unixsocketserver	unixsock;
	bool	unixlisten=unixsock.listen("lsnr.sck",0000,15);
	if (!unixlisten) {
		stdoutput.printf("couldn't listen on unix socket\n");
		stdoutput.printf("%s\n",error::getErrorString());
	}

	// bail if neither socket worked out
	if (!inetlisten && !unixlisten) {
		stdoutput.printf("couldn't listen on either socket\n");
		process::exit(0);
	}

	// create a listener and add the 2 sockets to it
	listener	pool;
	if (inetlisten) {
		pool.addReadFileDescriptor(&inetsock);
	}
	if (unixlisten) {
		pool.addReadFileDescriptor(&unixsock);
	}


	// loop...
	for (;;) {

		// wait for a client to connect to one of the sockets
		int32_t	result=pool.listen(-1,-1);
		if (result==RESULT_ERROR) {
			stdoutput.printf("error waiting...\n%d: %s\n%d: %s\n",
						error::getErrorNumber(),
						error::getErrorString(),
						error::getNativeErrorNumber(),
						error::getNativeErrorString());
			break;
		} else if (result==RESULT_TIMEOUT) {
			stdoutput.printf("timeout waiting...\n");
			break;
		}
		filedescriptor	*fd=pool.getReadReadyList()->
						getFirst()->getValue();

		// figure out which socket the client connected to
		filedescriptor	*clientsock;
		if (fd==&inetsock) {
			clientsock=inetsock.accept();
			stdoutput.printf("inetsock: ");
		} else if (fd==&unixsock) {
			clientsock=unixsock.accept();
			stdoutput.printf("unixsock: ");
		} else {
			stdoutput.printf("client not connected "
					"to either socket...\n");
			snooze::macrosnooze(1);
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
