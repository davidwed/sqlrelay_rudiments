// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/snooze.h>
#include <rudiments/directory.h>
#include <rudiments/userentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/file.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>
#ifdef RUDIMENTS_HAVE_CREATE_PROCESS
	#include <rudiments/environment.h>
	#include <rudiments/bytestring.h>
#endif


#ifndef __USE_XOPEN_EXTENDED
	// for getsid()
	#define __USE_XOPEN_EXTENDED
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

// for exit()
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#ifdef RUDIMENTS_HAVE_TLHELP32_H
	#include <tlhelp32.h>
#endif

// for umask
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif

// for GetCurrentProcessId on windows
#ifdef RUDIMENTS_HAVE_PROCESS_H
	#include <process.h>
#endif

// for waitpid...
#if defined(RUDIMENTS_HAVE_SYS_WAIT_H)
	#include <sys/wait.h>
#endif

signalhandler	process::_deadchildhandler;
signalhandler	process::_shutdownhandler;
signalhandler	process::_crashhandler;

void		(*process::_shutdownfunc)(int32_t);
void		(*process::_crashfunc)(int32_t);

pid_t process::getProcessId() {
	#if defined(RUDIMENTS_HAVE_GETCURRENTPROCESSID)
		return GetCurrentProcessId();
	#elif defined(RUDIMENTS_HAVE_GETPID)
		return getpid();
	#else
		#error no getpid or anything like it
	#endif
}

pid_t process::getParentProcessId() {
	#if defined(RUDIMENTS_HAVE_GETPPID)
		return getppid();
	#elif defined(RUDIMENTS_HAVE_PROCESS32FIRST) && _WIN32_WINNT>=0x0500
		HANDLE	snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		if (snap==INVALID_HANDLE_VALUE) {
			return -1;
		}
		PROCESSENTRY32	pe;
		pe.dwSize=sizeof(pe);
		pid_t	mypid=getProcessId();
		pid_t	ppid=-1;
		if (Process32First(snap,&pe)) {
			do {
				if (pe.th32ProcessID==mypid) {
					ppid=pe.th32ParentProcessID;
					break;
				}
			} while (Process32Next(snap,&pe));
		}
		CloseHandle(snap);
		return ppid;
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

pid_t process::getProcessGroupId() {
	return getProcessGroupId(0);
}

pid_t process::getProcessGroupId(pid_t pid) {
	#if defined(RUDIMENTS_HAVE_GETPGID)
		return getpgid(pid);
	#else
		// minix doesn't have the notion of process groups
		// windows does, but they don't appear to have ID's
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

bool process::setProcessGroupId() {
	return setProcessGroupId(0);
}

bool process::setProcessGroupId(pid_t pgid) {
	return setProcessGroupId(0,pgid);
}

bool process::setProcessGroupId(pid_t pid, pid_t pgid) {
	#if defined(RUDIMENTS_HAVE_SETPGID)
		return !setpgid(pid,pgid);
	#else
		// minix doesn't have the notion of process groups
		// windows does, but they don't appear to have ID's
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

pid_t process::getSessionId() {
	return getSessionId(0);
}

pid_t process::getSessionId(pid_t pid) {
	#if defined(RUDIMENTS_HAVE_GETSID)
		return getsid(pid);
	#else
		// windows doesn't have the notion of sessions
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

pid_t process::newSession() {
	#ifdef RUDIMENTS_HAVE_SETSID
		return setsid();
	#else
		// windows doesn't have the notion of sessions
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

uid_t process::getRealUserId() {
	#if defined(RUDIMENTS_HAVE_GETUID)
		return getuid();
	#elif defined(RUDIMENTS_HAVE_GETUSERNAME)
		DWORD	size=0;
		GetUserName(NULL,&size);
		if (size<1) {
			return (uid_t)-1;
		}
		CHAR	*buffer=new CHAR[size];
		if (GetUserName(buffer,&size)==FALSE) {
			delete[] buffer;
			return (uid_t)-1;
		}
		uid_t	uid=userentry::getUserId(buffer);
		delete[] buffer;
		return uid;
	#else
		#error no getuid or anything like it
	#endif
}

uid_t process::getEffectiveUserId() {
	#if defined(RUDIMENTS_HAVE_GETEUID)
		return geteuid();
	#else
		// windows doesn't have the notion of effective user id's
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

bool process::setUserId(uid_t uid) {
	#if defined(RUDIMENTS_HAVE_SETUID)
		return !setuid(uid);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool process::setUser(const char *username) {
	uid_t	userid=userentry::getUserId(username);
	return (userid!=(uid_t)-1)?setUserId(userid):true;
}

#ifdef RUDIMENTS_HAVE_SETEUID_BUT_NOT_DEFINED
extern "C" int seteuid(uid_t euid);
#endif

bool process::setEffectiveUserId(uid_t uid) {
	#if defined(RUDIMENTS_HAVE_SETEUID)
		return !seteuid(uid);
	#elif defined(RUDIMENTS_HAVE_SYSCALL_SETEUID)
		return !syscall(SYS_seteuid,uid);
	#else
		// windows doesn't have the notion of effective user id's
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool process::setRealAndEffectiveUserId(uid_t uid, uid_t euid) {
	#if defined(RUDIMENTS_HAVE_SETREUID)
		return !setreuid(uid,euid);
	#else
		return setUserId(uid) && setEffectiveUserId(euid);
	#endif
}

gid_t process::getRealGroupId() {
	#if defined(RUDIMENTS_HAVE_GETUID)
		return getgid();
	#elif defined(RUDIMENTS_HAVE_GETUSERNAME)
		// windows doesn't have the notion of group-thread association
		// but we can return the primary group of the user associated
		// with the thread
		userentry	uent;
		uent.initialize(getRealUserId());
		return uent.getPrimaryGroupId();
	#else
		#error no getgid or anything like it
	#endif
}

gid_t process::getEffectiveGroupId() {
	#if defined(RUDIMENTS_HAVE_GETEGID)
		return getegid();
	#else
		// windows doesn't have the notion of effective group id's
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

bool process::setGroupId(gid_t gid) {
	#if defined(RUDIMENTS_HAVE_SETUID)
		return !setgid(gid);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool process::setGroup(const char *groupname) {
	gid_t	groupid=groupentry::getGroupId(groupname);
	return (groupid!=(gid_t)-1)?setGroupId(groupid):true;
}

#ifdef RUDIMENTS_HAVE_SETEGID_BUT_NOT_DEFINED
extern "C" int setegid(gid_t egid);
#endif

bool process::setEffectiveGroupId(gid_t gid) {
	#if defined(RUDIMENTS_HAVE_SETEGID)
		return !setegid(gid);
	#else
		// windows doesn't have the notion of effective group id's
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool process::setRealAndEffectiveGroupId(gid_t gid, gid_t egid) {
	#if defined(RUDIMENTS_HAVE_SETREUID)
		return !setregid(gid,egid);
	#else
		return setGroupId(gid) && setEffectiveGroupId(egid);
	#endif
}

mode_t process::setFileCreationMask(mode_t mask) {
	#if defined(RUDIMENTS_HAVE_UMASK)
		return ::umask(mask);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

pid_t process::fork() {
	#if defined(RUDIMENTS_HAVE_FORK)
		pid_t	result;
		error::clearError();
		do {
			result=::fork();

			if (result==-1 && error::getErrorNumber()==EAGAIN) {
				stdoutput.printf("fork: retry: %s\n",
						error::getErrorString());
				snooze::macrosnooze(1);
				continue;
			}

		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

bool process::supportsFork() {
	#if defined(RUDIMENTS_HAVE_FORK)
		return true;
	#else
		return false;
	#endif
}

bool process::exec(const char *command, const char * const *args) {
	#if defined(RUDIMENTS_HAVE_EXECVP)
		return execvp(command,(char * const *)args)!=-1;
	#elif defined(RUDIMENTS_HAVE_CREATE_PROCESS)
		if (spawn(command,args,false)) {
			exit(0);
		}
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

#if defined(RUDIMENTS_HAVE_CREATE_PROCESS)
static char *quoteArg(const char *arg) {

	// put quotes around args that contain spaces...
	if (!charstring::contains(arg," ")) {
		return charstring::duplicate(arg);
	}

	size_t	quotedarglen=charstring::length(arg);
	char	*quotedarg=new char[charstring::length(arg)+4];

	charstring::copy(quotedarg,"\"");
	charstring::append(quotedarg,arg);

	// if the last character is a backslash, then escape it using another
	// backslash, otherwise windows will think that the trailing \"
	// means "include a literal quote"
	if (arg[quotedarglen-1]=='\\') {
		charstring::append(quotedarg,"\\");
	}

	charstring::append(quotedarg,"\"");

	return quotedarg;
}
#endif

pid_t process::spawn(const char *command,
				const char * const *args,
				bool detached) {
	#if defined(RUDIMENTS_HAVE_FORK) && defined(RUDIMENTS_HAVE_EXECVP)
		pid_t	child=fork();
		if (child==-1 || child>0) {
			return child;
		}
		return exec(command,args);
	#elif defined(RUDIMENTS_HAVE_CREATE_PROCESS)

		// if the command doesn't include a backslash then search
		// the path for it
		char	*fqcommand=NULL;
		if (charstring::contains(command,"\\")) {
			fqcommand=charstring::duplicate(command);
		} else {

			// split the PATH on ;'s
			const char	*path=environment::getValue("PATH");
			char		**dirs=NULL;
			uint64_t	dircount=0;
			charstring::split(path,";",true,&dirs,&dircount);

			// search each directory in the PATH
			size_t	cmdlen=charstring::length(command);
			for (uint64_t i=0; i<dircount; i++) {

				fqcommand=new char[
						charstring::length(dirs[i])+1+
						cmdlen+1];

				charstring::copy(fqcommand,dirs[i]);
				charstring::append(fqcommand,"\\");
				charstring::append(fqcommand,command);

				delete[] dirs[i];

				if (file::exists(fqcommand)) {
					break;
				}

				delete[] fqcommand;
				fqcommand=NULL;
			}
	
			// clean up
			delete[] dirs;
		}

		// get the short form of the fully qualified command
		// (this will work on older versions of windows)
		char	shortcommand[32768];
		GetShortPathName(fqcommand,shortcommand,sizeof(shortcommand));
		delete[] fqcommand;

		// Create the command line from the args...
		// CreateProcess must be able to modify the command line (for
		// some reason, and there's no indication as to whether it could
		// try to use more bytes than originally passed in) and will
		// only accept a command line of 32768 chars or less.
		char	commandline[32768];
		if (args) {
			commandline[0]='\0';
			bool	first=true;
			size_t	totalsize=0;
			for (const char * const *arg=args; *arg; arg++) {
				if (!first) {
					if (totalsize+1<32767) {
						charstring::append(
							commandline," ");
					} else {
						break;
					}
					totalsize=totalsize+1;
				} else {
					first=false;
				}
				char	*qarg=quoteArg(*arg);
				size_t	size=charstring::length(qarg);
				if (totalsize+size<32767) {
					charstring::append(commandline,qarg);
					delete[] qarg;
				} else {
					delete[] qarg;
					break;
				}
				totalsize=totalsize+size;
			}
		}

		// create the new process and return it's pid on success
		STARTUPINFO		si;
		PROCESS_INFORMATION	pi;
		bytestring::zero(&si,sizeof(si));
		bytestring::zero(&pi,sizeof(pi));
		bool	success=(CreateProcess(shortcommand,commandline,
					NULL,NULL,TRUE,
					(detached)?CREATE_NEW_PROCESS_GROUP:0,
					NULL,NULL,&si,&pi)==TRUE);
		if (!success) {
			stdoutput.printf("%s\n",shortcommand);
			stdoutput.printf("error: %s\n",
					error::getNativeErrorString());
		}
		return (success)?pi.dwProcessId:-1;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool process::detach() {

	// fork off a child process
	pid_t	result=fork();
	if (result==-1) {
		return false;
	}

	// let the parent process exit
	if (result) {
		// cygwin needs a sleep or both processes will exit
		#ifdef __CYGWIN__
			snooze::macrosnooze(1);
		#endif
		exitImmediately(0);
	}

	// create a new session with no controlling terminal
	newSession();

	// change directory to root to avoid keeping any directories in use
	directory::changeDirectory("/");

	// Set umask such that files are created 666 and directories 777.  This
	// way we can change them to whatever we like using chmod().  We want to
	// avoid inheriting a umask which wouldn't give us write permissions to
	// files we create.
	setFileCreationMask(0);

	return true;
}

void process::exit(int32_t status) {
	::exit(status);
}

void process::exitImmediately(int32_t status) {
	_exit(status);
}

bool process::createPidFile(const char *filename, mode_t permissions) {
	char	*pid=charstring::parseNumber((uint64_t)process::getProcessId());
	file	pidfile;
	bool	retval=(pidfile.create(filename,permissions) &&
			pidfile.write(pid)==(ssize_t)charstring::length(pid));
	delete[] pid;
	return retval;
}

int64_t process::checkForPidFile(const char *filename) {
	char	*pidstring=file::getContents(filename);
	int64_t	retval=(pidstring && pidstring[0])?
				charstring::toInteger(pidstring):-1;
	delete[] pidstring;
	return retval;
}

void process::exitOnCrashOrShutDown() {
	exitOnShutDown();
	exitOnCrash();
	waitForChildren();
}

void process::exitOnShutDown() {
	_shutdownhandler.setHandler(defaultShutDown);
	_shutdownhandler.handleSignal(SIGINT);
	_shutdownhandler.handleSignal(SIGTERM);
	#ifdef SIGQUIT
	_shutdownhandler.handleSignal(SIGQUIT);
	#endif
	#ifdef SIGHUP
	_shutdownhandler.handleSignal(SIGHUP);
	#endif
}

void process::handleShutDown(void (*shutdownfunction)(int32_t)) {
	_shutdownfunc=shutdownfunction;
	_shutdownhandler.setHandler(shutDown);
	_shutdownhandler.handleSignal(SIGINT);
	_shutdownhandler.handleSignal(SIGTERM);
	#ifdef SIGQUIT
	_shutdownhandler.handleSignal(SIGQUIT);
	#endif
	#ifdef SIGHUP
	_shutdownhandler.handleSignal(SIGHUP);
	#endif
}

void process::exitOnCrash() {
	_crashhandler.setHandler(defaultCrash);
	_crashhandler.handleSignal(SIGABRT);
	_crashhandler.handleSignal(SIGFPE);
	_crashhandler.handleSignal(SIGILL);
	_crashhandler.handleSignal(SIGSEGV);
	#ifdef SIGBUS
	_crashhandler.handleSignal(SIGBUS);
	#endif
	#ifdef SIGIOT
	_crashhandler.handleSignal(SIGIOT);
	#endif
	#ifdef SIGEMT
	_crashhandler.handleSignal(SIGEMT);
	#endif
	#ifdef SIGSYS
	_crashhandler.handleSignal(SIGSYS);
	#endif
}

void process::handleCrash(void (*crashfunction)(int32_t)) {
	_crashfunc=crashfunction;
	_crashhandler.setHandler(crash);
	_crashhandler.handleSignal(SIGABRT);
	_crashhandler.handleSignal(SIGFPE);
	_crashhandler.handleSignal(SIGILL);
	_crashhandler.handleSignal(SIGSEGV);
	#ifdef SIGBUS
	_crashhandler.handleSignal(SIGBUS);
	#endif
	#ifdef SIGIOT
	_crashhandler.handleSignal(SIGIOT);
	#endif
	#ifdef SIGEMT
	_crashhandler.handleSignal(SIGEMT);
	#endif
	#ifdef SIGSYS
	_crashhandler.handleSignal(SIGSYS);
	#endif
}

void process::waitForChildren() {
	#ifdef SIGCHLD
		_deadchildhandler.setHandler(waitForChildrenToExit);
		_deadchildhandler.addFlag(SA_NOCLDSTOP);
		_deadchildhandler.handleSignal(SIGCHLD);
	#endif
}

void process::dontWaitForChildren() {
	#ifdef SIGCHLD
		_deadchildhandler.setHandler((void (*)(int32_t))SIG_DFL);
		_deadchildhandler.removeAllFlags();
		_deadchildhandler.handleSignal(SIGCHLD);
	#endif
}

void process::shutDown(int32_t signum) {
	waitForChildren();
	(*_shutdownfunc)(signum);
}

void process::crash(int32_t signum) {
	waitForChildren();
	(*_crashfunc)(signum);
}

void process::defaultShutDown(int32_t signum) {
	waitForChildren();
	process::exit(0);
}

void process::defaultCrash(int32_t signum) {
	waitForChildren();
	process::exit(1);
}

void process::waitForChildrenToExit(int32_t signum) {

	// Some systems generate a single SIGCHLD even if more than 1 child
	// has entered it's exit state, so we need to loop here and catch
	// all of them.

	// If getChildStateChange returns 0 then there were no more
	// processes in their exit state, the loop should exit.
	// If it returns > 0 then it successfully waited on a process and it
	// should loop back to wait on another one.
	// If it returns -1 then there was some error and the loop should exit.
	int32_t	pid=0;
	do {
		pid=getChildStateChange(-1,false,true,true,NULL,NULL,NULL,NULL);
	} while (pid>0);

	// FIXME: What if a SIGCHLD gets generated after waitpid() has returned
	// but before the signal handler exits?   Will that SIGCHLD be lost?

	// Since we didn't use the SA_ONESHOT flag when we set up this signal
	// handler, we don't need to reinstall the signal handler here, it will
	// be done automatically.
}

pid_t process::getChildStateChange(pid_t pid,
					bool wait,
					bool ignorestop,
					bool ignorecontinue,
					childstatechange *newstate,
					int32_t	*exitstatus,
					int32_t *signum,
					bool *coredump) {
#ifdef _WIN32
	RUDIMENTS_SET_ENOSYS
	return -1;
#else

	// build options
	int32_t	options=0;
	if (!wait) {
		options|=WNOHANG;
	}
	if (!ignorestop) {
		options|=WUNTRACED;
	}
	if (!ignorecontinue) {
		#ifdef WCONTINUED
		options|=WCONTINUED;
		#endif
	}

	// init status
	int32_t	status=0;

	// wait
	int32_t	childpid=-1;
	error::clearError();
	do {
		// Minix 3.1.8 needs the int * cast
		childpid=waitpid(pid,(int *)&status,options);
	} while (childpid==-1 && error::getErrorNumber()==EINTR);

	// set return values
	if (childpid>0 && newstate) {
		if (WIFEXITED(status)) {
			*newstate=EXIT_CHILDSTATECHANGE;
			if (exitstatus) {
				*exitstatus=WEXITSTATUS(status);
			}
		} else if (WIFSIGNALED(status)) {
			*newstate=TERMINATED_CHILDSTATECHANGE;
			if (signum) {
				*signum=WTERMSIG(status);
			}
			#ifdef WCOREDUMP
			if (coredump) {
				*coredump=WCOREDUMP(status);
			}
			#endif
		} else if (WIFSTOPPED(status)) {
			*newstate=STOPPED_CHILDSTATECHANGE;
			if (signum) {
				*signum=WSTOPSIG(status);
			}
		#ifdef WIFCONTINUED
		} else if (WIFCONTINUED(status)) {
			*newstate=CONTINUED_CHILDSTATECHANGE;
		#endif
		}
	}

	return childpid;
#endif
}

bool process::supportsGetChildStateChange() {
	#ifdef _WIN32
		return false;
	#else
		return true;
	#endif
}
