// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/signalclassesinlines.h>
#endif

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <stddef.h>


// signalhandler methods
signalhandler::signalhandler() {
	handlerstruct=new (struct sigaction);
	removeAllSignalsFromMask();
	removeAllFlags();
}

signalhandler::signalhandler(int signum, void *handler) {
	handlerstruct=new (struct sigaction);
	removeAllSignalsFromMask();
	removeAllFlags();
	setHandler(handler);
	handleSignal(signum);
}
