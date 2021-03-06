// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"

#ifndef _WIN32
#include <rudiments/signalclasses.h>
void alarmHandler(int32_t signum) {
}
#endif

int main(int argc, const char **argv) {

	header("snooze");

	uint32_t	timeleft;
	test("uninterrupted",snooze::macrosnooze(1,&timeleft));
	test("timeleft",timeleft<=1);

#ifndef _WIN32
	signalhandler	alarmhandler;
	alarmhandler.setHandler(alarmHandler);
	alarmhandler.handleSignal(SIGALRM);

	signalmanager::alarm(1);

	test("interrupted",!snooze::macrosnooze(5,&timeleft));
	test("timeleft",timeleft<5);
#endif
}
