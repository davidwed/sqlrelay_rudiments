// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/signalclasses.h>
#include <rudiments/process.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	if (argc<3) {
		stdoutput.printf("usage: kill signal processid\n");
		process::exit(1);
	}

	int32_t	sig=SIGINT;
	if (!charstring::compare(argv[1],"SIGTERM")) {
		sig=SIGTERM;
	} else if (!charstring::compare(argv[1],"SIGABRT")) {
		sig=SIGABRT;
	} else if (!charstring::compare(argv[1],"SIGFPE")) {
		sig=SIGFPE;
	} else if (!charstring::compare(argv[1],"SIGILL")) {
		sig=SIGILL;
	} else if (!charstring::compare(argv[1],"SIGSEGV")) {
		sig=SIGSEGV;
	}

	pid_t	pid=charstring::toInteger(argv[2]);

	if (!signalmanager::sendSignal(pid,sig)) {
stdoutput.printf("%s\n",error::getErrorString());
		process::exit(1);
	}
	process::exit(0);
}
