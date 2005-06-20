// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/charstring.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

uint32_t	buffersize;

class myserver : public daemonprocess, public inetserversocket {
	public:
			myserver() : daemonprocess(), inetserversocket() {}
		void	listen();
};


void	myserver::listen() {


	// make sure that only one instance is running
	int	pid=checkForPidFile("/tmp/svr.pidfile");
	if (pid>-1) {
		printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// detach from the controlling terminal
	//detach();


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	createPidFile("/tmp/svr.pidfile",permissions::ownerReadWrite());

	// listen on inet socket port 8000
	if (!inetserversocket::listen(NULL,8000,15)) {
		printf("couldn't listen on port 8000\n");
	}

	// loop...
	for (;;) {

		// accept a client connection
		filedescriptor	*clientsock=accept();
		clientsock->allowShortReads();
		clientsock->setReadBufferSize(buffersize);

		// read from the client
		unsigned char	buffer;
		while (clientsock->read(&buffer)>0) {}

		// close the socket and clean up
		clientsock->close();
		delete clientsock;
	}
}


myserver	*mysvr;

// define a function to shut down the process cleanly
RETSIGTYPE	shutDown() {
	printf("shutting down\n");
	mysvr->close();
	delete mysvr;
	unlink("/tmp/svr.pidfile");
	exit(0);
}


int main(int argc, const char **argv) {

	if (argc<2) {
		printf("usage: inetsvrbench [buffer size]\n");
		exit(1);
	}

	buffersize=charstring::toUnsignedInt32(argv[1]);

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	mysvr->handleShutDown((RETSIGTYPE *)shutDown);
	mysvr->handleCrash((RETSIGTYPE *)shutDown);

	mysvr->listen();
}
