// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/daemonprocess.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/daemonprocessinlines.h>
#endif
#include <rudiments/file.h>
#include <rudiments/text.h>

#include <stdlib.h>

#ifdef __GNUC__
static signalhandler	daemonprocess::shutdownhandler;
static signalhandler	daemonprocess::crashhandler;
static void		(*daemonprocess::shutdownfunc)(int);
static void		(*daemonprocess::crashfunc)(int);
static signalhandler	daemonprocess::deadchildhandler;
#endif

daemonprocess::daemonprocess() {
	deadchildhandler.setHandler((void *)waitOnChildren);
	deadchildhandler.addFlag(SA_NOCLDSTOP);
	deadchildhandler.handleSignal(SIGCHLD);
}

daemonprocess::~daemonprocess() {
	waitOnChildren();
}

int daemonprocess::createPidFile(const char *filename, mode_t permissions) {
	file	fl;
	char	*pid=text::parseNumber(getpid());
	int	retval=(fl.create(filename,permissions,pid)==strlen(pid));
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
