// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/daemonprocess.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <stdlib.h>

// for fork/getpid/exit...
#include <unistd.h>

// for wait...
#include <sys/wait.h>

#ifdef __GNUC__
signalhandler	daemonprocess::deadchildhandler;
signalhandler	daemonprocess::shutdownhandler;
signalhandler	daemonprocess::crashhandler;
void		(*daemonprocess::shutdownfunc)(int);
void		(*daemonprocess::crashfunc)(int);
#endif

daemonprocess::daemonprocess() {

	// we want daemons to wait for children to die before shutting down,
	// so register some default shutdown/crash handlers that only do that
	shutdownhandler.setHandler((void *)defaultShutDown);
	shutdownhandler.handleSignal(SIGINT);
	shutdownhandler.handleSignal(SIGTERM);

	crashhandler.setHandler((void *)defaultCrash);
	crashhandler.handleSignal(SIGSEGV);
}

daemonprocess::~daemonprocess() {
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

void daemonprocess::waitForChildren() {
	// It's tempting to pass WNOHANG here, but it's probably not what
	// we really want.  WNOHANG will cause the waitpid() to wait for all
	// children who are currently in their final exit() stage.  But, if
	// the parent calls exit(), and all the children will start dying off,
	// the parent may receive the SIGCHLD from the child before the child
	// is in its final exit() stage and not actually wait for it.
	while(waitpid(-1,NULL,0)>0);
}

void daemonprocess::shutDown() {
	registerWaitForChildren();
	(*shutdownfunc)(0);
}

void daemonprocess::crash() {
	registerWaitForChildren();
	(*crashfunc)(0);
}

void daemonprocess::defaultShutDown() {
	registerWaitForChildren();
	exit(0);
}

void daemonprocess::defaultCrash() {
	registerWaitForChildren();
	exit(1);
}

void daemonprocess::registerWaitForChildren() {
	deadchildhandler.setHandler((void *)waitForChildren);
	deadchildhandler.addFlag(SA_NOCLDSTOP);
	deadchildhandler.handleSignal(SIGCHLD);
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
