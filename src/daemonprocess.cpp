// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/daemonprocess.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

// for waitpid...
#if !defined(_WIN32) && defined(RUDIMENTS_HAVE_SYS_WAIT_H)
	#include <sys/wait.h>
#endif

signalhandler	daemonprocess::_deadchildhandler;
signalhandler	daemonprocess::_shutdownhandler;
signalhandler	daemonprocess::_crashhandler;
void		(*daemonprocess::_shutdownfunc)(int32_t);
void		(*daemonprocess::_crashfunc)(int32_t);

void daemonprocess::initialize() {

	// we want daemons to wait for children to die before shutting down,
	// so register some default shutdown/crash handlers that only do that
	_shutdownhandler.setHandler(defaultShutDown);
	_shutdownhandler.handleSignal(SIGINT);
	_shutdownhandler.handleSignal(SIGTERM);

	_crashhandler.setHandler(defaultCrash);
	_crashhandler.handleSignal(SIGSEGV);

	waitForChildren();
}

bool daemonprocess::createPidFile(const char *filename, mode_t permissions) {
	char	*pid=charstring::parseNumber((uint64_t)process::getProcessId());
	bool	retval=(file::createFile(filename,permissions,pid)==
					(ssize_t)charstring::length(pid));
	delete[] pid;
	return retval;
}

int64_t daemonprocess::checkForPidFile(const char *filename) {
	char	*pidstring=file::getContents(filename);
	int64_t	retval=(pidstring && pidstring[0])?
				charstring::toInteger(pidstring):-1;
	delete[] pidstring;
	return retval;
}

void daemonprocess::handleShutDown(void (*shutdownfunction)(int32_t)) {

	_shutdownfunc=shutdownfunction;

	_shutdownhandler.setHandler(shutDown);
	_shutdownhandler.handleSignal(SIGINT);
	_shutdownhandler.handleSignal(SIGTERM);
}

void daemonprocess::handleCrash(void (*crashfunction)(int32_t)) {

	_crashfunc=crashfunction;

	_crashhandler.setHandler(crash);
	_crashhandler.handleSignal(SIGSEGV);
}

#ifndef _WIN32
void daemonprocess::waitForChildren() {
	_deadchildhandler.setHandler(waitForChildrenToExit);
	_deadchildhandler.addFlag(SA_NOCLDSTOP);
	_deadchildhandler.handleSignal(SIGCHLD);
}

void daemonprocess::dontWaitForChildren() {
	_deadchildhandler.setHandler((void (*)(int32_t))SIG_DFL);
	_deadchildhandler.removeAllFlags();
	_deadchildhandler.handleSignal(SIGCHLD);
}
#else
void daemonprocess::waitForChildren() {
	// FIXME: implement this
}
void daemonprocess::dontWaitForChildren() {
	// FIXME: implement this
}
#endif

void daemonprocess::shutDown(int32_t signum) {
	waitForChildren();
	(*_shutdownfunc)(signum);
}

void daemonprocess::crash(int32_t signum) {
	waitForChildren();
	(*_crashfunc)(signum);
}

void daemonprocess::defaultShutDown(int32_t signum) {
	waitForChildren();
	process::exit(0);
}

void daemonprocess::defaultCrash(int32_t signum) {
	waitForChildren();
	process::exit(1);
}

#ifndef _WIN32
void daemonprocess::waitForChildrenToExit(int32_t signum) {

	// Some systems generate a single SIGCHLD even if more than 1 child
	// has entered it's exit state, so we need to loop here and catch
	// all of them.

	// If waitpid() returns 0 then there were no more processes in their
	// exit state, the loop should exit.
	// If it returns > 0 then it successfully waited on a process and it
	// should loop back to wait on another one.
	// If it returns -1 and was interrupted by a signal and should loop
	// back and be restarted.
	// If it returns -1 then there was some other error and the loop should
	// exit.
	for (;;) {
		int32_t pid=waitpid(-1,NULL,WNOHANG);
		if (pid==0 || (pid==-1 && error::getErrorNumber()!=EINTR)) {
			break;
		}
	}

	// FIXME: What if a SIGCHLD gets generated after waitpid() has returned
	// but before the signal handler exits?   Will that SIGCHLD be lost?

	// Since we didn't use the SA_ONESHOT flag when we set up this signal
	// handler, we don't need to reinstall the signal handler here, it will
	// be done automatically.
}
#else
void daemonprocess::waitForChildrenToExit(int32_t signum) {
	// FIXME: implement this...
	// Use ChildStart()
}
#endif
