// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/permissions.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/snooze.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

class myserver : public inetsocketserver {
	public:
			myserver() : inetsocketserver() {}
		void	listen();
};


void myserver::listen() {


	// make sure that only one instance is running
	int	pid=process::checkForPidFile("svr.pid");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// detach from the controlling terminal
	process::detach();


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	process::createPidFile("svr.pid",
				permissions::ownerReadWrite());


	// listen on inet socket port 8000
	inetsocketserver::listen(NULL,8000,15);


	// accept a client connection
	filedescriptor	*clientsock=accept();


	// read 5 bytes from the client and display it
	char	buffer[6];
	buffer[5]=(char)NULL;
	clientsock->read((char *)buffer,5);
	stdoutput.printf("%s\n",buffer);


	// write "hello" back to the client
	clientsock->write("hello",5);

	// sleep so the client can test it's timeout
	snooze::macrosnooze(5);

	// close the socket and clean up
	clientsock->close();
	delete clientsock;
}


myserver	*mysvr;

// define a function to shut down the process cleanly
void shutDown(int sig) {
	stdoutput.printf("shutting down\n");
	mysvr->close();
	delete mysvr;
	file::remove("svr.pid");
	process::exit(0);
}


int main(int argc, const char **argv) {

	mysvr=new myserver();

	// set up signal handlers for clean shutdown
	process::waitForChildren();
	process::handleShutDown(shutDown);
	process::handleCrash(shutDown);

	mysvr->listen();

	shutDown(0);
}
