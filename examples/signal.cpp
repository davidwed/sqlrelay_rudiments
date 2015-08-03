// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <rudiments/stdio.h>

bool	gotsigterm=false;

void handleSigterm(int32_t sig) {
	stdoutput.printf("Got a SIGTERM!\n");
	gotsigterm=true;
}

#ifdef SIGALRM
void handleAlarm(int32_t sig) {
	stdoutput.printf("alarm!\n");
}
#endif

int main(int argc, const char **argv) {

	// ignore all signals except SIGTERM and SIGALRM
	signalset	ignoreset;
	ignoreset.addAllSignals();
	ignoreset.removeSignal(SIGTERM);
	#ifdef SIGALRM
	ignoreset.removeSignal(SIGALRM);
	#endif
	signalmanager::ignoreSignals(&ignoreset);

	// run handleSigterm() when a SIGTERM is received
	signalhandler	termhandler;
	termhandler.setHandler(handleSigterm);
	termhandler.handleSignal(SIGTERM);

	// run handleAlarm() when a SIGALRM is received
	#ifdef SIGALRM
	signalhandler	alarmhandler;
	alarmhandler.setHandler(handleAlarm);
	alarmhandler.handleSignal(SIGALRM);
	#endif

	// Loop forever, waiting to receive a signal that we are not ignoring.
	signalmanager::alarm(2);
	while (!gotsigterm) {
		signalmanager::waitForSignals(&ignoreset);
		signalmanager::alarm(2);
	}
}
