// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

void handleSigusr1(int sig) {
	printf("Got a SIGUSR1!\n");
}

int main(int argc, const char **argv) {

	// this program will ignore all signals except SIGUSR1
	signalset	ignoreset;
	ignoreset.addAllSignals();
	ignoreset.removeSignal(SIGUSR1);
	signalmanager::ignoreSignals(&ignoreset);

	// when it gets a SIGUSR1, it will run the handleSigusr1() function
	signalhandler	shandler;
	shandler.setHandler(handleSigusr1);
	shandler.handleSignal(SIGUSR1);

	// Loop forever, each time waiting for a signal not in the ignoreset
	// to be sent. Since SIGUSR1 is the only signal not in the ignoreset,
	// waitForSignals will fall through only when SIGUSR1 is received.
	for (;;) {
		signalmanager::waitForSignals(&ignoreset);
	}
}
