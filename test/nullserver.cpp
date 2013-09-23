// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/file.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

// for NULL
#include <stdio.h>

class myserver : public daemonprocess, public inetsocketserver {
	public:
			myserver() : daemonprocess(), inetsocketserver() {}
		void	listen();
};


void	myserver::listen() {


	// make sure that only one instance is running
	int	pid=checkForPidFile("/tmp/svr.pidfile");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this server is already running with process id: %d\n",pid);
		return;
	}


	// detach from the controlling terminal
	//detach();


	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	createPidFile("/tmp/svr.pidfile",permissions::ownerReadWrite());


	// listen on inet socket port 8000
	if (!inetsocketserver::listen(NULL,8000,15)) {
		stdoutput.printf("couldn't listen on port 8000\n");
	}


	// loop...
	for (;;) {

		// accept a client connection
		filedescriptor	*clientsock=accept();

		for (;;) {
			char	*buffer=NULL;
			char	term[]={'T','A','Y',0x1c,0x03,'3'};
			clientsock->read(&buffer,term);
			stdoutput.printf("%s\n",buffer);
			delete[] buffer;

			//char	response[]={0x02,'B','A','D','R','E','S','P','O','N','S','E',0x03};
			//for (int i=0; i<sizeof(response); i++) {
			for (;;) {
				//clientsock->write(response[i]);
				clientsock->write((char)0x05);
				//stdoutput.printf("%c\n",response[i]);
				stdoutput.printf("ENQ\n");
				snooze::macrosnooze(5);
			}
		}

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
