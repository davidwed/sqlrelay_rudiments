// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#ifndef ENABLE_INLINES
	#include <rudiments/private/signalclassesinlines.h>
#endif

#include <stddef.h>


// signalhandler methods
signalhandler::signalhandler() {
	removeAllSignalsFromMask();
	removeAllFlags();
}

signalhandler::signalhandler(int signum, void *handler) {
	removeAllSignalsFromMask();
	removeAllFlags();
	setHandler(handler);
	handleSignal(signum);
}
