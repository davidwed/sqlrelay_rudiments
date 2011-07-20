// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/daemonprocess.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

// for umask
#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>

#ifdef RUDIMENTS_HAVE_UNISTD_H
	// for fork/exit...
	#include <unistd.h>
#endif

#ifndef MINGW32
	// for wait...
	#include <sys/wait.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class daemonprocessprivate {
	friend class daemonprocess;
	private:
		static	signalhandler	_deadchildhandler;
		static	signalhandler	_shutdownhandler;
		static	signalhandler	_crashhandler;
		static	void		(*_shutdownfunc)(int);
		static	void		(*_crashfunc)(int);
};

signalhandler	daemonprocessprivate::_deadchildhandler;
signalhandler	daemonprocessprivate::_shutdownhandler;
signalhandler	daemonprocessprivate::_crashhandler;
void		(*daemonprocessprivate::_shutdownfunc)(int);
void		(*daemonprocessprivate::_crashfunc)(int);

daemonprocessprivate	*daemonprocess::pvt=NULL;

void daemonprocess::shutDown(int signum) {
	waitForChildren();
#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
	(*pvt->_shutdownfunc)(signum);
#else
	(*pvt->_shutdownfunc)(0);
#endif
}

void daemonprocess::crash(int signum) {
	waitForChildren();
#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
	(*pvt->_crashfunc)(signum);
#else
	(*pvt->_crashfunc)(0);
#endif
}

void daemonprocess::defaultShutDown(int signum) {
	waitForChildren();
	exit(0);
}

void daemonprocess::defaultCrash(int signum) {
	waitForChildren();
	exit(1);
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

#ifndef MINGW32
bool daemonprocess::detach() const {

	// fork off a child process
	int	result;
	do {
		result=fork();
	} while (result==-1 && error::getErrorNumber()==EINTR);

	if (result==-1) {
		return false;
	}

	// let the parent process exit
	if (result) {
		// cygwin needs a sleep or both processes will exit
		#ifdef __CYGWIN__
		snooze::macrosnooze(1);
		#endif
		_exit(0);
	}
	
	// become process group and session group leader 
	// with no controlling terminal
	setsid();

	// change directory to root to avoid keeping any directories in use
	do {} while (chdir("/")==-1 && error::getErrorNumber()==EINTR);

	// Set umask such that files are created 666 and directories 777.
	// This way we can change them to whatever we like using chmod().
	// We want to avoid inheriting a umask which wouldn't give us write
	// permissions to files we create.
	umask(0);

	return true;
}
#else
bool daemonprocess::detach() const {
	// FIXME: implement this
	error::setErrorNumber(ENOSYS);
	return false;
}
#endif

void daemonprocess::handleShutDown(void (*shutdownfunction)(int)) {

	pvt->_shutdownfunc=shutdownfunction;

	pvt->_shutdownhandler.setHandler(shutDown);
	pvt->_shutdownhandler.handleSignal(SIGINT);
	pvt->_shutdownhandler.handleSignal(SIGTERM);
}

void daemonprocess::handleCrash(void (*crashfunction)(int)) {

	pvt->_crashfunc=crashfunction;

	pvt->_crashhandler.setHandler(crash);
	pvt->_crashhandler.handleSignal(SIGSEGV);
}

#ifndef MINGW32
void daemonprocess::waitForChildrenToExit(int signum) {

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
		int pid=waitpid(-1,NULL,WNOHANG);
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
void daemonprocess::waitForChildrenToExit(int signum) {
	// FIXME: implement this...
	// Use ChildStart()
}
#endif

#ifndef MINGW32
void daemonprocess::waitForChildren() {
	pvt->_deadchildhandler.setHandler(waitForChildrenToExit);
	pvt->_deadchildhandler.addFlag(SA_NOCLDSTOP);
	pvt->_deadchildhandler.handleSignal(SIGCHLD);
}

void daemonprocess::dontWaitForChildren() {
	pvt->_deadchildhandler.setHandler((void (*)(int))SIG_DFL);
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

int daemonprocess::runAsUser(const char *username) const {
	uid_t	userid;
	return (passwdentry::getUserId(username,&userid))?
					runAsUserId(userid):1;
}

int daemonprocess::runAsGroup(const char *groupname) const {
	gid_t	groupid;
	return (groupentry::getGroupId(groupname,&groupid))?
					runAsGroupId(groupid):1;
}

int daemonprocess::runAsUserId(uid_t uid) const {
	return process::setUserId(uid);
}

int daemonprocess::runAsGroupId(gid_t gid) const {
	return process::setGroupId(gid);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
