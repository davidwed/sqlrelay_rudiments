// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

#include <stdlib.h>

// for fork/getpid/exit...
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

// for wait...
#include <sys/wait.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

signalhandler	daemonprocess::deadchildhandler;
signalhandler	daemonprocess::shutdownhandler;
signalhandler	daemonprocess::crashhandler;
void		(*daemonprocess::shutdownfunc)(int);
void		(*daemonprocess::crashfunc)(int);

daemonprocess::daemonprocess() {

	// FIXME: setting static members in the constructor???

	// we want daemons to wait for children to die before shutting down,
	// so register some default shutdown/crash handlers that only do that
	// FIXME: it should be possible to use one of the C++ casting operators
	// here (and elsewhere in this class), but I'm not really sure how...
	shutdownhandler.setHandler((void *)defaultShutDown);
	shutdownhandler.handleSignal(SIGINT);
	shutdownhandler.handleSignal(SIGTERM);

	crashhandler.setHandler((void *)defaultCrash);
	crashhandler.handleSignal(SIGSEGV);

	waitForChildren();
}

daemonprocess::~daemonprocess() {
}

bool daemonprocess::createPidFile(const char *filename, mode_t permissions) {
	char	*pid=charstring::parseNumber((uint64_t)getpid());
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

void daemonprocess::handleShutDown(void *shutdownfunction) {

	shutdownfunc=(void(*)(int))shutdownfunction;

	shutdownhandler.setHandler((void *)shutDown);
	shutdownhandler.handleSignal(SIGINT);
	shutdownhandler.handleSignal(SIGTERM);
}

void daemonprocess::handleCrash(void *crashfunction) {

	crashfunc=(void(*)(int))crashfunction;

	crashhandler.setHandler((void *)crash);
	crashhandler.handleSignal(SIGSEGV);
}

void daemonprocess::waitForChildrenToExit() {

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

void daemonprocess::shutDown() {
	waitForChildren();
	(*shutdownfunc)(0);
}

void daemonprocess::crash() {
	waitForChildren();
	(*crashfunc)(0);
}

void daemonprocess::defaultShutDown() {
	waitForChildren();
	exit(0);
}

void daemonprocess::defaultCrash() {
	waitForChildren();
	exit(1);
}

void daemonprocess::waitForChildren() {
	deadchildhandler.setHandler((void *)waitForChildrenToExit);
	deadchildhandler.addFlag(SA_NOCLDSTOP);
	deadchildhandler.handleSignal(SIGCHLD);
}

void daemonprocess::dontWaitForChildren() {
	deadchildhandler.setHandler((void *)SIG_DFL);
	deadchildhandler.removeAllFlags();
	deadchildhandler.handleSignal(SIGCHLD);
}

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
