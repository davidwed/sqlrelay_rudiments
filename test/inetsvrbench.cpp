// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/permissions.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

uint32_t	buffersize;

class myserver : public inetsocketserver {
	public:
			myserver() : inetsocketserver() {}
		void	listen();
};


void myserver::listen() {


	// make sure that only one instance is running
	int	pid=process::checkForPidFile("/tmp/svr.pidfile");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// detach from the controlling terminal
	//detach();


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	process::createPidFile("/tmp/svr.pidfile",
				permissions::ownerReadWrite());

	// listen on inet socket port 8000
	if (!inetsocketserver::listen(NULL,8000,15)) {
		stdoutput.printf("couldn't listen on port 8000\n");
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
void shutDown(int sig) {
	stdoutput.printf("shutting down\n");
	mysvr->close();
	delete mysvr;
	file::remove("/tmp/svr.pidfile");
	process::exit(0);
}


int main(int argc, const char **argv) {

	if (argc<2) {
		stdoutput.printf("usage: inetsvrbench [buffer size]\n");
		process::exit(1);
	}

	buffersize=charstring::toUnsignedInteger(argv[1]);

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	process::waitForChildren();
	process::handleShutDown(shutDown);
	process::handleCrash(shutDown);

	mysvr->listen();
}
