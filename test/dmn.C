// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

daemonprocess	*dmn;

// define a function to shut down the process cleanly
RETSIGTYPE	shutDown() {

	printf("shutting down\n");

	// clean up
	delete dmn;
	unlink("/tmp/dmn.pidfile");
	exit(0);
}

int main(int argv, const char **argc) {

	dmn=new daemonprocess();

	// set up signal handlers for clean shutdown
	dmn->handleShutDown((RETSIGTYPE *)shutDown);
	dmn->handleCrash((RETSIGTYPE *)shutDown);

	// change the user/group that the daemon is running as
	dmn->runAsUser("nobody");
	dmn->runAsGroup("nobody");

	// make sure that only one instance is running
	int	pid=dmn->checkForPidFile("/tmp/dmn.pidfile");
	if (pid>-1) {
		printf("Sorry, an instance of this daemon is already running with process id: %d\n",pid);
		delete dmn;
		exit(0);
	}

	// detach from the controlling terminal
	dmn->detach();

	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	dmn->createPidFile("/tmp/dmn.pidfile",permissions::ownerReadWrite());

	// loop, printing "looping..." once per second
	for (;;) {
		printf("looping...\n");
		sleep(1);
	}
}
