// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>
#include <rudiments/inetserversocket.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include <rudiments/null.h>

class myserver : public daemonprocess, public inetserversocket {
	public:
			myserver();
		void	listen();
};

myserver::myserver() : daemonprocess(), inetserversocket() {
}

void myserver::listen() {


	// make sure that only one instance is running
	int	pid=checkForPidFile("/tmp/svr.pidfile");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	createPidFile("/tmp/svr.pidfile",permissions::ownerReadWrite());


	// listen on inet socket port 8000
	if (!inetserversocket::listen(NULL,8000,15)) {
		stdoutput.printf("couldn't listen on port 8000\n");
	}


	// loop...
	for (;;) {

		// accept a client connection
		filedescriptor	*clientsock=accept();

		// read 5 bytes from the client and display it
		char	buffer[6];
		buffer[5]=(char)NULL;
		clientsock->read((char *)buffer,5);
		stdoutput.printf("%s\n",buffer);

		// write "hello" back to the client
		clientsock->write("hello",5);

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

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	mysvr->handleShutDown(shutDown);
	mysvr->handleCrash(shutDown);

	mysvr->listen();
}
