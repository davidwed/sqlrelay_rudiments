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
static signalhandler	*daemonprocess::shutdownhandler;
static signalhandler	*daemonprocess::crashhandler;
static void		(*daemonprocess::shutdownfunc)(int);
static void		(*daemonprocess::crashfunc)(int);
static signalhandler	*daemonprocess::deadchildhandler;
#endif

daemonprocess::daemonprocess() {

	deadchildhandler=new signalhandler();
	shutdownhandler=new signalhandler();
	crashhandler=new signalhandler();

	deadchildhandler->setHandler((void *)waitOnChildren);
	deadchildhandler->addFlag(SA_NOCLDSTOP);
	deadchildhandler->handleSignal(SIGCHLD);
}

daemonprocess::~daemonprocess() {
	waitOnChildren();

	delete deadchildhandler;
	delete shutdownhandler;
	delete crashhandler;
}

inline int daemonprocess::runAsUser(const char *username) const {
	passwd	*ent;
#ifdef HAVE_GETPWNAM_R
	char	buffer[1024];
	passwd	pwd;
	if (getpwnam_r((username)?username:"",&pwd,buffer,1024,&ent)) {
		return 0;
	}
#else
	ent=getpwnam((username)?username:"");
#endif
	int	retval=(ent)?runAsUserId(ent->pw_uid):0;
#ifdef HAVE_GETPWNAM_R
	delete ent;
#endif
}

inline int daemonprocess::runAsGroup(const char *groupname) const {
	group	*ent;
#ifdef HAVE_GETGRNAM_R
	char	buffer[1024];
	group	grp;
	if (getgrnam_r((groupname)?groupname:"",&grp,buffer,1024,&ent)) {
		return 0;
	}
#else
	ent=getgrnam((groupname)?groupname:"");
#endif
	int	retval=(ent)?runAsGroupId(ent->gr_gid):0;
#ifdef HAVE_GETGRNAM_R
	delete ent;
#endif
}

int daemonprocess::checkForPidFile(const char *filename) const {

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

	shutdownhandler->setHandler((void *)shutDown);
	shutdownhandler->handleSignal(SIGINT);
	shutdownhandler->handleSignal(SIGTERM);
}

void daemonprocess::handleCrash(void *crashfunction) {

	crashfunc=(void(*)(int))crashfunction;

	crashhandler->setHandler((void *)crash);
	crashhandler->handleSignal(SIGSEGV);
}
