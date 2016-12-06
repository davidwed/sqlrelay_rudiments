// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/charstring.h>
#include <rudiments/directory.h>
#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>
#include "test.cpp"

bool	gotsigterm=false;
void handleSigterm(int32_t sig) {
	//stdoutput.printf("got sigterm\n");
	gotsigterm=true;
}

#ifdef SIGFPE
bool	gotsigfpe=false;
void handleSigfpe(int32_t sig) {
	//stdoutput.printf("got sigfpe\n");
	gotsigfpe=true;
}
#endif

#ifdef SIGALRM
bool	gotsigalrm=false;
void handleSigalrm(int32_t sig) {
	//stdoutput.printf("got sigalrm\n");
	gotsigalrm=true;
}
#endif

int main(int argc, const char **argv) {

// FIXME: it's not clear why this doesn't work on windows,
// signals generally do work...
#ifndef _WIN32
	if (argc==1) {

		header("signal");

		// spawn a child to signal this process
		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/signal");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;
		char	*pidstr=charstring::parseNumber(
					(uint64_t)process::getProcessId());
		const char	*args1[]={"signal",pidstr,NULL};
		process::spawn(cmd.getString(),args1,true);
		delete[] pidstr;


		// this program will ignore all signals except
		// SIGTERM, SIGFPE, and SIGALRM
		signalset	ignoreset;
		ignoreset.addAllSignals();
		ignoreset.removeSignal(SIGTERM);
		#ifdef SIGFPE
		ignoreset.removeSignal(SIGFPE);
		#endif
		#ifdef SIGALRM
		ignoreset.removeSignal(SIGALRM);
		#endif
		signalmanager::ignoreSignals(&ignoreset);

		// when it gets a SIGFPE,
		// it will run the handleSigfpe() function
		#ifdef SIGFPE
		signalhandler	fpehandler;
		fpehandler.setHandler(handleSigfpe);
		fpehandler.handleSignal(SIGFPE);
		#endif

		// when it gets a SIGTERM,
		// it will run the handleSigterm() function
		signalhandler	termhandler;
		termhandler.setHandler(handleSigterm);
		termhandler.handleSignal(SIGTERM);

		// when it gets a SIGALRM,
		// it will run the handleSigalrm() function
		#ifdef SIGALRM
		signalhandler	alarmhandler;
		alarmhandler.setHandler(handleSigalrm);
		alarmhandler.handleSignal(SIGALRM);
		#endif

		// set an alarm
		signalmanager::alarm(3);

		// wait for signals
		if (!signalmanager::waitForSignals(&ignoreset)) {
			snooze::macrosnooze(5);
		}
		test("SIGTERM",gotsigterm);
		#ifdef SIGFPE
		signalmanager::waitForSignals(&ignoreset);
		test("SIGFPE",gotsigfpe);
		#endif
		#ifdef SIGALRM
		signalmanager::waitForSignals(&ignoreset);
		test("SIGALRM",gotsigalrm);
		#endif

	} else {

		// get the parent pid from the command line
		pid_t	pid=charstring::toInteger(argv[1]);

		// send it a SIGTERM
		snooze::macrosnooze(1);
		process::sendSignal(pid,SIGTERM);

		// send it a SIGFPE
		#ifdef SIGFPE
		snooze::macrosnooze(1);
		process::sendSignal(pid,SIGFPE);
		#endif
	}
#endif
}
