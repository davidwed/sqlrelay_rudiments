// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <rudiments/stdio.h>

#ifdef SIGFPE
void handleSigfpe(int sig) {
	stdoutput.printf("Got a SIGFPE!\n");
}
#endif

void handleAlarm(int sig) {
	stdoutput.printf("alarm!\n");
}

int main(int argc, const char **argv) {

	// this program will ignore all signals except SIGFPE
	signalset	ignoreset;
	ignoreset.addAllSignals();
	#ifdef SIGFPE
	ignoreset.removeSignal(SIGFPE);
	#endif
	ignoreset.removeSignal(SIGALRM);
	signalmanager::ignoreSignals(&ignoreset);

	// when it gets a SIGFPE, it will run the handleSigfpe() function
	#ifdef SIGFPE
	signalhandler	shandler;
	shandler.setHandler(handleSigfpe);
	shandler.handleSignal(SIGFPE);
	#endif

	// handle alarm
	signalhandler	alarmhandler;
	alarmhandler.setHandler(handleAlarm);
	alarmhandler.handleSignal(SIGALRM);

	// Loop forever, each time waiting for a signal not in the ignoreset
	// to be sent. Since SIGFPE is the only signal not in the ignoreset,
	// waitForSignals will fall through only when SIGFPE is received.
	signalmanager::alarm(2);
	for (;;) {
		signalmanager::waitForSignals(&ignoreset);
		signalmanager::alarm(2);
	}
}
