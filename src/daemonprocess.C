// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/daemonprocess.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/daemonprocessinlines.h>
#endif
#include <rudiments/fileproperties.h>

#include <stdlib.h>

#ifdef __GNUC__
static	signalhandler	*daemonprocess::sighandler;
static	void		(*daemonprocess::shutdownfunc)(int);
static	signalhandler	*daemonprocess::deadchildhandler;
#endif

daemonprocess::daemonprocess() {

	deadchildhandler=new signalhandler();
	sighandler=new signalhandler();

	deadchildhandler->setHandler((void *)waitOnChildren);
	deadchildhandler->addFlag(SA_NOCLDSTOP);
	deadchildhandler->handleSignal(SIGCHLD);
}

daemonprocess::~daemonprocess() {
	waitOnChildren();

	delete deadchildhandler;
	delete sighandler;
}

int	daemonprocess::checkForPidFile(const char *filename) const {

	// open the file, don't need to check for error here, getSize() below
	// will return -1 if the file descriptor is no good
	int	fd=open(filename,O_RDONLY);

	// get the file size
	off_t	size;
	if (fileproperties::getSize(fd,&size)==-1) {
		return -1;
	}

	// get the contents of the file (a pid in string form)
	int	retval=-1;
	char	*pidstring=new char[size+1];
	if (read(fd,pidstring,size)==size) {
		pidstring[size]=(char)NULL;
		retval=atoi(pidstring);
	}
	delete[] pidstring;

	// return the pid
	return retval;
}

void	daemonprocess::detach() const {

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

void	daemonprocess::handleShutDown(void *shutdownfunction) {

	shutdownfunc=(void(*)(int))shutdownfunction;

	sighandler->setHandler((void *)shutDown);
	sighandler->handleSignal(SIGINT);
	sighandler->handleSignal(SIGTERM);
}
