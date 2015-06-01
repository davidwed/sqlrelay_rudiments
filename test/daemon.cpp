// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/process.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

// define a function to shut down the process cleanly
void shutDown(int32_t sig) {

	stdoutput.printf("%d: shutting down with signal %d\n",
					process::getProcessId(),sig);

	// clean up
	file::remove("dmn.pid");
	process::exit(0);
}

void crash(int32_t sig) {

	stdoutput.printf("%d: crashing with signal %d\n",
					process::getProcessId(),sig);

	// clean up
	file::remove("dmn.pid");
	process::exit(0);
}

int main(int argc, const char **argv) {

	process::waitForChildren();

	// set up signal handlers for clean shutdown
	process::handleShutDown(shutDown);
	process::handleCrash(crash);

	// change the user/group that the daemon is running as
	process::setUser("nobody");
	process::setGroup("nobody");

	// make sure that only one instance is running
	int64_t	pid=process::checkForPidFile("dmn.pid");
	if (pid>-1) {
		stdoutput.printf("Sorry, an instance of this daemon is already running with process id: %d\n",pid);
		process::exit(0);
	}

	// detach from the controlling terminal
	process::detach();

	// create a pid file which is used to make sure that only one instance
	// is running and can also be used to kill the process
	process::createPidFile("dmn.pid",permissions::ownerReadWrite());

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
