// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <rudiments/stdio.h>

void handleSigfpe(int sig) {
	stdoutput.printf("Got a SIGFPE!\n");
}

int main(int argc, const char **argv) {

	// this program will ignore all signals except SIGFPE
	signalset	ignoreset;
	ignoreset.addAllSignals();
	ignoreset.removeSignal(SIGFPE);
	signalmanager::ignoreSignals(&ignoreset);

	// when it gets a SIGFPE, it will run the handleSigfpe() function
	signalhandler	shandler;
	shandler.setHandler(handleSigfpe);
	shandler.handleSignal(SIGFPE);

	// Loop forever, each time waiting for a signal not in the ignoreset
	// to be sent. Since SIGFPE is the only signal not in the ignoreset,
	// waitForSignals will fall through only when SIGFPE is received.
	for (;;) {
		signalmanager::waitForSignals(&ignoreset);
	}
}
