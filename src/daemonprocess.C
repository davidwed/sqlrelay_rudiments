// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <stdlib.h>

// for fork/getpid...
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

// for wait...
#ifdef HAVE_SYS_WAIT_H
	#include <sys/wait.h>
#endif

#ifdef __GNUC__
signalhandler	daemonprocess::shutdownhandler;
signalhandler	daemonprocess::crashhandler;
void		(*daemonprocess::shutdownfunc)(int);
void		(*daemonprocess::crashfunc)(int);
signalhandler	daemonprocess::deadchildhandler;
#endif

daemonprocess::daemonprocess() {
	deadchildhandler.setHandler((void *)waitOnChildren);
	deadchildhandler.addFlag(SA_NOCLDSTOP);
	deadchildhandler.handleSignal(SIGCHLD);
}

daemonprocess::~daemonprocess() {
	waitOnChildren();
}

bool daemonprocess::createPidFile(const char *filename, mode_t permissions) {
	file	fl;
	char	*pid=charstring::parseNumber((long)getpid());
	bool	retval=(fl.create(filename,permissions,pid)==
					(ssize_t)charstring::length(pid));
	delete[] pid;
	fl.close();
	return retval;
}

int daemonprocess::checkForPidFile(const char *filename) const {
	char	*pidstring=file::getContents(filename);
	int	retval=(pidstring && pidstring[0])?atoi(pidstring):-1;
	delete[] pidstring;
	return retval;
}

void daemonprocess::detach() const {

	// fork and let the parent process exit
	if (fork()) {
		_exit(0);
	}
	
	// become process group and session group leader 
	// with no controlling terminal
	setsid();

	// change directory to root to avoid keeping any directories in use
	chdir("/");

	// Set umask such that files are created 666 and directories 777.
	// This way we can change them to whatever we like using chmod().
	// We want to avoid inheriting a umask which wouldn't give us write
	// permissions to files we create.
	umask(0);
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

void daemonprocess::waitOnChildren() {
	while(waitpid(-1,NULL,WNOHANG)>0);
}

void daemonprocess::shutDown() {
	(*shutdownfunc)(0);
}

void daemonprocess::crash() {
	(*crashfunc)(0);
}

int daemonprocess::runAsUserId(uid_t uid) const {
	#ifdef HAVE_SETUID
		return !setuid(uid);
	#else
		return 1;
	#endif
}

int daemonprocess::runAsGroupId(gid_t gid) const {
	#ifdef HAVE_SETGID
		return !setgid(gid);
	#else
		return 1;
	#endif
}
