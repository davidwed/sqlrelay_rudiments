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
#if defined(RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT)
	#include <rudiments/sys.h>
#endif
#ifdef RUDIMENTS_HAVE_EXECINFO_H
	#include <execinfo.h>
#endif


#ifndef __USE_XOPEN_EXTENDED
	// for getsid()
	#define __USE_XOPEN_EXTENDED
#endif
#ifdef RUDIMENTS_HAVE_GRP_H
	#include <grp.h>
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
#ifdef RUDIMENTS_HAVE_DBGHELP_H
	#include <dbghelp.h>
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

bool		process::_retry=true;

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

uid_t process::getUserId() {
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
		#if defined(RUDIMENTS_HAVE_SETGROUPS)
			// If we're changing from root to another user, be sure
			// to drop all group permissions first.  This will fail
			// if we're not root, so there's no need to check the
			// result.
			setgroups(0,NULL);
		#endif
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

bool process::setEffectiveUser(const char *username) {
	uid_t	userid=userentry::getUserId(username);
	return (userid!=(uid_t)-1)?setEffectiveUserId(userid):true;
}

bool process::setRealAndEffectiveUserId(uid_t uid, uid_t euid) {
	#if defined(RUDIMENTS_HAVE_SETREUID)
		return !setreuid(uid,euid);
	#else
		return setUserId(uid) && setEffectiveUserId(euid);
	#endif
}

gid_t process::getGroupId() {
	#if defined(RUDIMENTS_HAVE_GETUID)
		return getgid();
	#elif defined(RUDIMENTS_HAVE_GETUSERNAME)
		// windows doesn't have the notion of group-thread association
		// but we can return the primary group of the user associated
		// with the thread
		userentry	uent;
		uent.initialize(getUserId());
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

bool process::setEffectiveGroup(const char *groupname) {
	gid_t	groupid=groupentry::getGroupId(groupname);
	return (groupid!=(gid_t)-1)?setEffectiveGroupId(groupid):true;
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
			if (result==-1 &&
				error::getErrorNumber()==EAGAIN && _retry) {
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

bool process::sendSignal(pid_t processid, int32_t signum) {
	#ifdef RUDIMENTS_HAVE_KILL
		int32_t	result;
		error::clearError();
		do {
			result=kill(processid,signum);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT)

		// decide what access rights we need
		DWORD	accessrights=PROCESS_TERMINATE;
		if (signum!=SIGKILL) {
			accessrights=PROCESS_CREATE_THREAD|
					PROCESS_QUERY_INFORMATION|
					PROCESS_VM_OPERATION|
					PROCESS_VM_WRITE|
					PROCESS_VM_READ;
		}

		// open the target process
		HANDLE	targetprocess=OpenProcess(accessrights,FALSE,processid);
		if (!targetprocess) {
			return false;
		}

		// for SIGKILL we just need to call TerminateProcess
		if (signum==SIGKILL) {
			bool	result=(TerminateProcess(targetprocess,1)!=0);
			CloseHandle(targetprocess);
			return result;
		}

		// For SIGABRT, SIGFPE, SIGILL and SIGSEGV we can trigger the
		// target process' unhandled exception filter by creating a
		// thread aimed at NULL.
		if (signum==SIGABRT || signum==SIGFPE ||
				signum==SIGILL || signum==SIGSEGV) {
			HANDLE	otherthread=
				CreateRemoteThread(targetprocess,
						NULL,0,
						(LPTHREAD_START_ROUTINE)NULL,
						NULL,0,NULL);
			if (otherthread) {
				CloseHandle(otherthread);
			}
			CloseHandle(targetprocess);
			return otherthread!=NULL;
		}

		// For SIGINT/SIGTERM, it gets a lot crazier...

		// Yes, the ridiculousness below is the only "reasonable"
		// way to do this...

		// First... SIGINT is really CTRL-C, but processes created with
		// the CREATE_NEW_PROCESS_GROUP flag don't respond to CTRL-C.
		// All processes respond to CTRL-BREAK though, so we need to
		// use that instead.

		// Ideally for SIGINT/SIGTERM we'd just run
		// GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,processid) but that
		// only works if the calling process is in the same process
		// group as processid (ie. a parent or child of processid).
		// 
		// So, we need to somehow coerce a process or thread in the
		// target process group to run it for us.  We can create a new
		// thread in the target process using CreateRemoteThread.
		// 
		// Ideally we'd just tell it to run
		// GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,0) but
		// CreateRemoteThread only allows you to pass one argument
		// to the function that it runs and we need to pass two.
		//
		// The only "obvious" way to do this is to do define a chunk
		// of memory containing the machine code for a function that
		// runs GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,0) and copy it
		// over to the target process.
 		//
		// Then we can create a thread over there and aim the thread at
		// our function.

		// this only works on x86 and x64, so bail right away if
		// this isn't one of those platforms
		char	*arch=sys::getOperatingSystemArchitecture();
		if (!charstring::compare(arch,"Unknown")) {
			return false;
		}

		// Get the address of GenerateConsoleCtrlEvent in kernel32.dll.
		// kernel32.dll is loaded at the same address for all programs,
		// so the address of this function ought to be the same in
		// the target process as it is here.
		HMODULE	kernel32=GetModuleHandle("Kernel32");
		if (!kernel32) {
			return false;
		}
		FARPROC	funcaddr=GetProcAddress(kernel32,
						"GenerateConsoleCtrlEvent");
		if (!funcaddr) {
			return false;
		}

		// Define a chunk of memory containing the machine code for
		// a function that runs GenerateConsoleCtrlEvent with two
		// parameters (with values of 0).  Eventually this code will
		// be run in the target process...
		//
		// helpful site:
		// https://defuse.ca/online-x86-assembler.htm

		const unsigned char	*updatedmachinecode=NULL;
		size_t			machinecodesize=0;

		// FIXME: use better method of detecting x86 vs. x64
		#ifdef _USE_32BIT_TIME_T

			// for x86:
			const unsigned char	machinecode32[]={
				// load second parameter (0)
				// (we'll overwrite this in a minute)
				0x68,			// push (word)
				0x0,0x0,0x0,0x0,	// 0
				// load first parameter (1)
				0x6A,			// push (byte)
				0x1,			// 1
				// load the absolute address of the function to
				// call (for now use 0, we'll overwrite this in
				// a minute)
				0xB8,			// mov eax
				0x0,0x0,0x0,0x0,	// 0
				// call the function
				0xFF,0xD0,		// call eax
				// return 1
				0xB8,			// mov eax
				0x1,0x0,0x0,0x0,	// 1
				// return
				0xC3			// ret
			};
			size_t		machinecode32size=sizeof(machinecode32);

			// copy the code into a buffer and
			// replace the second parameter and call address
			unsigned char		*updatedmachinecode32=
				(unsigned char *)bytestring::duplicate(
							machinecode32,
							machinecode32size);

			uint32_t	*addr=
				(uint32_t *)(updatedmachinecode32+1);
			*addr=(uint32_t)processid;

			addr=(uint32_t *)(updatedmachinecode32+8);
			*addr=(uint32_t)funcaddr;

			updatedmachinecode=updatedmachinecode32;
			machinecodesize=machinecode32size;

		#else

			// for x64:
			const unsigned char	machinecode64[]={
				// allocate shadow space of 32 bytes on the
				// stack and align it to 16 bytes
				0x48,0x83,0xEC,		// sub rsp
				0x28,			// 0x28
				// load second parameter (0)
				// (we'll overwrite this in a minute)
				0x48,0xC7,0xC2,		// mov rdx
				0x00,0x00,0x00,0x00,	// 0
				// load first parameter (0)
				0x48,0xC7,0xC1,		// mov rcx
				0x01,0x00,0x00,0x00,	// 1
				// load the absolute address of the function to
				// call (for now use 0, we'll overwrite this in
				// a minute)
				0x49,0xBA,		// movabs r10
				0x00,0x00,0x00,0x00,	// 0
				0x00,0x00,0x00,0x00,	// 0 (64-bit)
				// call the function
				0x41,0xFF,0xD2,		// call r10
				// return 1
				0x48,0xC7,0xC0,		// mov rax
				0x01,0x00,0x00,0x00,	// 1
				// deallocate the shadow space
				// and align the stack to 16 bytes
				0x48,0x83,0xC4,		// add rsp
				0x28,			// 0x28
				// return
				0xC3			// retq
			};
			size_t		machinecode64size=sizeof(machinecode64);

			// copy the code into a buffer and
			// replace the second parameter and call address
			unsigned char		*updatedmachinecode64=
				(unsigned char *)bytestring::duplicate(
							machinecode64,
							machinecode64size);

			uint32_t	*addr32=
				(uint32_t *)(updatedmachinecode64+7);
			*addr32=(uint32_t)processid;

			uint64_t	*addr64=
				(uint64_t *)(updatedmachinecode64+20);
			*addr64=(uint64_t)funcaddr;

			updatedmachinecode=updatedmachinecode64;
			machinecodesize=machinecode64size;
		#endif

		// allocate memory in the target process to copy the
		// machine code into
		void	*codetorun=VirtualAllocEx(targetprocess,
						NULL,machinecodesize,
						MEM_COMMIT,
						PAGE_EXECUTE_READWRITE);
		if (!codetorun) {
			CloseHandle(targetprocess);
			return false;
		}

		// copy the machine code over to the target process
		if (!WriteProcessMemory(targetprocess,
						codetorun,
						updatedmachinecode,
						machinecodesize,
						NULL)) {
			CloseHandle(targetprocess);
			return false;
		}

		// create a thread in the target process and aim it at
		// the machine code we passed over there
		HANDLE	otherthread=
			CreateRemoteThread(targetprocess,
					NULL,0,
					(LPTHREAD_START_ROUTINE)codetorun,
					NULL,0,NULL);
		if (!otherthread) {
			CloseHandle(targetprocess);
			return false;
		}

		// wait for the thread to finish running
		WaitForSingleObject(otherthread,INFINITE);

		// clean up
		VirtualFreeEx(targetprocess,codetorun,0,MEM_RELEASE);
		CloseHandle(otherthread);
		CloseHandle(targetprocess);
		
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool process::raiseSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_GENERATECONSOLECTRLEVENT)
		switch (signum) {
			case SIGINT:
			case SIGTERM:
				// SIGINT is really CTRL-C, but processes
				// created with the CREATE_NEW_PROCESS_GROUP
				// flag don't respond to CTRL-C.  All processes
				// respond to CTRL-BREAK though, so we need to
				// use that instead.
				if (GenerateConsoleCtrlEvent(
						CTRL_BREAK_EVENT,0)) {
					return true;
				}
			case SIGKILL:
				if (TerminateProcess(
						INVALID_HANDLE_VALUE,1)) {
					return true;
				}
			case SIGFPE:
				{
					// crash on purpose
					uint16_t	a=1;
					uint16_t	b=0;
					uint16_t	c=a/b;
				}
			case SIGABRT:
			case SIGILL:
			case SIGSEGV:
				{
					// crash on purpose
					void (*f)(void)=NULL;
					f();
				}
		}
		return false;
	#elif defined(RUDIMENTS_HAVE_RAISE)
		int32_t	result;
		error::clearError();
		do {
			result=raise(signum);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool process::atExit(void (*function)(void)) {
	#ifdef RUDIMENTS_HAVE_ATEXIT
		return !atexit(function);
	#else
		return false;
	#endif
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
	do {} while (getChildStateChange(-1,false,true,true,
						NULL,NULL,NULL,NULL)>0);

	// FIXME: What if a SIGCHLD gets generated after waitpid() has returned
	// but before the signal handler exits?   Will that SIGCHLD be lost?

	// Since we didn't use the SA_ONESHOT flag when we set up this signal
	// handler, we don't need to reinstall the signal handler here, it will
	// be done automatically.
}

bool process::wait(pid_t pid) {
	return wait(pid,NULL);
}

bool process::wait(pid_t pid, int32_t *exitstatus) {
#ifdef _WIN32
	HANDLE	h=OpenProcess(SYNCHRONIZE,TRUE,pid);
	if (!h) {
		return false;
	}

	bool	retval=(WaitForSingleObject(h,INFINITE)==WAIT_OBJECT_0);

	if (retval && exitstatus) {
		DWORD	status;
		if (GetExitCodeProcess(h,&status)) {
			*exitstatus=status;
		} else {
			retval=false;
		}
	}

	CloseHandle(h);

	return retval;
#else
	return (getChildStateChange(pid,true,true,true,
					NULL,exitstatus,NULL,NULL)==pid);
#endif
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

void process::retryFailedFork() {
	_retry=true;
}

void process::dontRetryFailedFork() {
	_retry=false;
}

bool process::getRetryFailedFork() {
	return _retry;
}

void process::backtrace(stringbuffer *buffer, uint32_t maxframes) {
	#if defined(RUDIMENTS_HAVE_BACKTRACE)
		unsigned char	**btarray=new unsigned char *[maxframes];
		size_t	btsize=::backtrace((void **)btarray,(int)maxframes);
		char	**btstrings=backtrace_symbols((void **)btarray,btsize);
		for (size_t i=0; i<btsize; i++) {
			buffer->append(btstrings[i])->append('\n');
		}
		delete[] btstrings;
		delete[] btarray;
	#elif defined(RUDIMENTS_HAVE_CAPTURESTACKBACKTRACE)
		HANDLE	process=GetCurrentProcess();
		if (!SymInitialize(process,NULL,TRUE)) {
			return;
		}
		void	**btarray=(void **)malloc(maxframes*sizeof(void *));
		WORD btsize=CaptureStackBackTrace(0,128,btarray,NULL);
		SYMBOL_INFO	*symbolinfo=(SYMBOL_INFO *)
					malloc(sizeof(SYMBOL_INFO)+
						(1024*sizeof(TCHAR)));
		symbolinfo->MaxNameLen=1023;
		symbolinfo->SizeOfStruct=sizeof(SYMBOL_INFO);
		IMAGEHLP_LINE64	*line=(IMAGEHLP_LINE64 *)
					malloc(sizeof(IMAGEHLP_LINE64));
		line->SizeOfStruct=sizeof(IMAGEHLP_LINE64);
		DWORD	displacement;
		for (WORD i=0; i<btsize; i++) {
			if (SymFromAddr(process,(DWORD64)btarray[i],
							NULL,symbolinfo)) {
				buffer->append("\?\?\?(")->
					append(symbolinfo->Name)->
					append(")[0x")->
					append(symbolinfo->Address)->
					append("]\n");
			}
		}
		free(line);
		free(symbolinfo);
		free(btarray);
	#endif
}

void process::backtrace(stringbuffer *buffer) {
	backtrace(buffer,128);
}

void process::backtrace(filedescriptor *fd, uint32_t maxframes) {
	#ifdef RUDIMENTS_HAVE_BACKTRACE
		unsigned char	**btarray=new unsigned char *[maxframes];
		size_t	btsize=::backtrace((void **)btarray,(int)maxframes);
		backtrace_symbols_fd((void **)btarray,btsize,
					fd->getFileDescriptor());
		delete[] btarray;
	#else
		stringbuffer	buf;
		backtrace(&buf,maxframes);
		fd->write(buf.getString(),buf.getStringLength());
	#endif
}

void process::backtrace(filedescriptor *fd) {
	backtrace(fd,128);
}

void process::backtrace(const char *filename, uint32_t maxframes) {
	file	f;
	if (f.open(filename,O_WRONLY|O_APPEND|O_CREAT)) {
		backtrace(&f,maxframes);
	}
}

void process::backtrace(const char *filename) {
	backtrace(filename,128);
}
