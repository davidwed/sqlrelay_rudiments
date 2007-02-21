// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

void    handleSigusr1() {
        printf("Got a SIGUSR1!\n");
}

int main(int argc, const char **argv) {

        // this program will ignore all signals except SIGUSR1
        signalset       ignoreset;
        ignoreset.addAllSignals();
        ignoreset.removeSignal(SIGUSR1);
        signalmanager::ignoreSignals(ignoreset.getSignalSet());

        // when it gets a SIGUSR1, it will run the handleSigusr1() function
        signalhandler   shandler(SIGUSR1,(void *)&handleSigusr1);

	// Loop forever, each time waiting for a signal not in the ignoreset
	// to be sent. Since SIGUSR1 is the only signal not in the ignoreset,
	// waitForSignals will fall through only when SIGUSR1 is received.
        for (;;) {
                signalmanager::waitForSignals(ignoreset.getSignalSet());
        }
}
