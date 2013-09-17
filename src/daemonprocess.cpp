// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/daemonprocess.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/null.h>

// for waitpid...
#if !defined(_WIN32) && defined(RUDIMENTS_HAVE_SYS_WAIT_H)
	#include <sys/wait.h>
#endif

class daemonprocessprivate {
	friend class daemonprocess;
	private:
		static	signalhandler	_deadchildhandler;
		static	signalhandler	_shutdownhandler;
		static	signalhandler	_crashhandler;
		static	void		(*_shutdownfunc)(int32_t);
		static	void		(*_crashfunc)(int32_t);
};

signalhandler	daemonprocessprivate::_deadchildhandler;
signalhandler	daemonprocessprivate::_shutdownhandler;
signalhandler	daemonprocessprivate::_crashhandler;
void		(*daemonprocessprivate::_shutdownfunc)(int32_t);
void		(*daemonprocessprivate::_crashfunc)(int32_t);

daemonprocessprivate	*daemonprocess::pvt=NULL;

void daemonprocess::shutDown(int32_t signum) {
	waitForChildren();
	(*pvt->_shutdownfunc)(signum);
}

void daemonprocess::crash(int32_t signum) {
	waitForChildren();
	(*pvt->_crashfunc)(signum);
}

void daemonprocess::defaultShutDown(int32_t signum) {
	waitForChildren();
	process::exit(0);
}

void daemonprocess::defaultCrash(int32_t signum) {
	waitForChildren();
	process::exit(1);
}

daemonprocess::daemonprocess() {

	// FIXME: initializing static member here?
	// We don't want the signal handlers to be set up unless we call this
	// constructor, but there's got to be a cleaner way.
	pvt=new daemonprocessprivate;

	// we want daemons to wait for children to die before shutting down,
	// so register some default shutdown/crash handlers that only do that
	pvt->_shutdownhandler.setHandler(defaultShutDown);
	pvt->_shutdownhandler.handleSignal(SIGINT);
	pvt->_shutdownhandler.handleSignal(SIGTERM);

	pvt->_crashhandler.setHandler(defaultCrash);
	pvt->_crashhandler.handleSignal(SIGSEGV);

	waitForChildren();
}

daemonprocess::~daemonprocess() {
	delete pvt;
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

	pvt->_shutdownfunc=shutdownfunction;

	pvt->_shutdownhandler.setHandler(shutDown);
	pvt->_shutdownhandler.handleSignal(SIGINT);
	pvt->_shutdownhandler.handleSignal(SIGTERM);
}

void daemonprocess::handleCrash(void (*crashfunction)(int32_t)) {

	pvt->_crashfunc=crashfunction;

	pvt->_crashhandler.setHandler(crash);
	pvt->_crashhandler.handleSignal(SIGSEGV);
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

#ifndef _WIN32
void daemonprocess::waitForChildren() {
	pvt->_deadchildhandler.setHandler(waitForChildrenToExit);
	pvt->_deadchildhandler.addFlag(SA_NOCLDSTOP);
	pvt->_deadchildhandler.handleSignal(SIGCHLD);
}

void daemonprocess::dontWaitForChildren() {
	pvt->_deadchildhandler.setHandler((void (*)(int32_t))SIG_DFL);
	pvt->_deadchildhandler.removeAllFlags();
	pvt->_deadchildhandler.handleSignal(SIGCHLD);
}
#else
void daemonprocess::waitForChildren() {
	// FIXME: implement this
}
void daemonprocess::dontWaitForChildren() {
	// FIXME: implement this
}
#endif
