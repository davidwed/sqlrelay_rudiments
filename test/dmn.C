// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>

#include <rudiments/private/config.h>

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

daemonprocess	*dmn;

RETSIGTYPE	shutDown() {

	printf("shutting down\n");

	// clean up
	delete dmn;
	unlink("/tmp/dmn.pidfile");
	exit(0);
}

int main(int argv, const char **argc) {

	// launch the listener
	dmn=new daemonprocess();
	dmn->handleShutDown((RETSIGTYPE *)shutDown);

	int	pid=dmn->checkForPidFile("/tmp/dmn.pidfile");
	if (pid>-1) {
		printf("Sorry, an instance of this daemon is already running with process id: %d\n",pid);
		delete dmn;
		exit(0);
	}

	dmn->detach();
	dmn->createPidFile("/tmp/dmn.pidfile",permissions::ownerReadWrite());

	for (;;) {
		printf("looping...\n");
		sleep(1);
	}
}
