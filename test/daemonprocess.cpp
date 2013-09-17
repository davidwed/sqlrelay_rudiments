// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/permissions.h>
#include <rudiments/process.h>
#include <rudiments/file.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

daemonprocess	*dmn;

// define a function to shut down the process cleanly
void shutDown(int sig) {

	stdoutput.printf("%d: shutting down\n",process::getProcessId());

	// clean up
	delete dmn;
	file::remove("/tmp/dmn.pidfile");
	process::exit(0);
}

int main(int argc, const char **argv) {

	dmn=new daemonprocess();

	// set up signal handlers for clean shutdown
	dmn->handleShutDown(shutDown);
	dmn->handleCrash(shutDown);

	// change the user/group that the daemon is running as
	process::setUser("nobody");
	process::setGroup("nobody");

	// make sure that only one instance is running
	int	pid=dmn->checkForPidFile("/tmp/dmn.pidfile");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this daemon is already running with process id: %d\n",pid);
		delete dmn;
		process::exit(0);
	}

	// detach from the controlling terminal
	dmn->detach();

	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	dmn->createPidFile("/tmp/dmn.pidfile",permissions::ownerReadWrite());

	if (!process::fork()) {
		for (;;) {
			stdoutput.printf("%d: child looping...\n",
				process::getProcessId());
			snooze::macrosnooze(1);
		}
	}

	// loop, printing "looping..." once per second
	for (;;) {
		stdoutput.printf("%d: parent looping...\n",
				process::getProcessId());
		snooze::macrosnooze(1);
	}
}
