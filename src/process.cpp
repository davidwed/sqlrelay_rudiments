// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/process.h>
#include <rudiments/error.h>
#ifdef __CYGWIN__
	#include <rudiments/snooze.h>
#endif
#include <rudiments/directory.h>

#ifndef __USE_XOPEN_EXTENDED
	// for getsid()
	#define __USE_XOPEN_EXTENDED
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

// for exit()
#include <stdlib.h>

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#ifdef RUDIMENTS_HAVE_TLHELP32_H
	#include <tlhelp32.h>
#endif

// for umask
#include <sys/stat.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

pid_t process::getProcessId() {
	#if defined(RUDIMENTS_HAVE_GETPID)
		return getpid();
	#elif defined(RUDIMENTS_HAVE__GETPID)
		return _getpid();
	#else
		#error no getpid or anything like it
	#endif
}

pid_t process::getParentProcessId() {
	#ifdef RUDIMENTS_HAVE_GETPPID
		return getppid();
	#elif RUDIMENTS_HAVE_PROCESS32FIRST
		HANDLE	snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		if (snap==INVALID_HANDLE_VALUE) {
			return -1;
		}
		PROCESSENTRY32	pe;
		pe.dwSize=sizeof(pe);
		if (!Process32First(snap,&pe)) {
			CloseHandle(snap);
			return -1;
		}
		CloseHandle(snap);
		return pe.th32ParentProcessID;
	#else
		#error no getppid or anything like it
	#endif
}

pid_t process::getProcessGroupId() {
	return getProcessGroupId(0);
}

pid_t process::getProcessGroupId(pid_t pid) {
	#if defined(RUDIMENTS_HAVE_GETPGID)
		return getpgid(pid);
	#else
		// windows and minix don't have the notion of process groups
		error::setErrorNumber(ENOSYS);
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
		// windows and minix don't have the notion of process groups
		error::setErrorNumber(ENOSYS);
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
		// windows and minix don't have the notion of sessions
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

uid_t process::getRealUserId() {
	#if defined(RUDIMENTS_HAVE_GETUID)
		return getuid();
	#elif defined(_WIN32)
		/*
		// get the process handle
		HANDLE	snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		if (snap==INVALID_HANDLE_VALUE) {
			return INVALID_HANDLE_VALUE;
		}
		PROCESSENTRY32	pe;
		pe.dwSize=sizeof(pe);
		if (!Process32First(snap,&pe)) {
			CloseHandle(snap);
			return INVALID_HANDLE_VALUE;
		}
		CloseHandle(snap);
		HANDLE	proc=OpenProcess(PROCESS_QUERY_INFORMATION,
						FALSE,pe.th32ProcessID);
		if (proc==INVALID_HANDLE_VALUE) {
			return INVALID_HANDLE_VALUE;
		}

		// open the process token
		HANDLE	token;
		if (!OpenProcessToken(proc,TOKEN_READ,&token)) {
			CloseHandle(proc);
			return -1;
		}

		// get the buffer size we'll need
		DWORD	size;
		if (!GetTokenInformation(token,TokenUser,NULL,0,&size)) {
			CloseHandle(token);
			CloseHandle(proc);
			return -1;
		}

		// allocate a buffer
		PTOKEN_USER	ptokenuser=
			reinterpret_cast<PTOKEN_USER>(new BYTE(size));

		// populate the buffer
		if (!GetTokenInformation(token,Tokenuser,
					ptokenuser,size,&size)) {
			delete[] ptokenuser;
			CloseHandle(token);
			CloseHandle(proc);
			return -1;
		}

		// ptokenuser.Sid needs to be returned somehow

		// clean up
		delete[] ptokenuser;
		CloseHandle(token);
		CloseHandle(proc);*/
		// Windows user id's aren't numbers, they're undefined, odd,
		// variable length structures.  You can use code like above to
		// get one, but returning it is another matter.  You'd need to
		// figure out it's length, bytewise copy it out and return it's
		// length too.  It doesn't fit well into this paradigm so for
		// now at least we'll call this unsupported.
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no getuid or anything like it
	#endif
}

uid_t process::getEffectiveUserId() {
	#if defined(RUDIMENTS_HAVE_GETEUID)
		return geteuid();
	#elif defined(_WIN32)
		// windows doesn't have the notion of effective user id's
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no geteuid or anything like it
	#endif
}

bool process::setUserId(uid_t uid) {
	#if defined(RUDIMENTS_HAVE_SETUID)
		return !setuid(uid);
	#elif defined(_WIN32)
		// There may be some obsure way to do this under windows,
		// but it would surprise me.  Plus, user id's are odd variable-
		// length structures in windows.  See getUserId above.
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no setuid or anything like it
	#endif
}

#ifdef RUDIMENTS_HAVE_SETEUID_BUT_NOT_DEFINED
extern "C" int seteuid(uid_t euid);
#endif

bool process::setEffectiveUserId(uid_t uid) {
	#if defined(RUDIMENTS_HAVE_SETEUID)
		return !seteuid(uid);
	#elif defined(RUDIMENTS_HAVE_SYSCALL_SETEUID)
		return !syscall(SYS_seteuid,uid);
	#elif defined(_WIN32)
		// windows doesn't have the notion of effective user id's
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no seteuid or anything like it
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
	#elif defined(_WIN32)
		// You could get the group id using code similar to the code
		// commented out for getUserId() above, but use
		// TokenPrimaryGroup and an instance of PTOKEN_GROUPS in
		// GetTokenInformation() but you'd still end up with one of
		// those odd variable-length structures for the group id rather
		// than a number.  For now at least, we'll call this unsupported
		// as well.
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no getgid or anything like it
	#endif
}

gid_t process::getEffectiveGroupId() {
	#if defined(RUDIMENTS_HAVE_GETEGID)
		return getegid();
	#elif defined(_WIN32)
		// windows doesn't have the notion of effective group id's
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no getegid or anything like it
	#endif
}

bool process::setGroupId(gid_t gid) {
	#if defined(RUDIMENTS_HAVE_SETUID)
		return !setgid(gid);
	#elif defined(_WIN32)
		// There may be some obsure way to do this under windows,
		// but it would surprise me.  Plus, group id's are odd variable-
		// length structures in windows.  See getRealGroupId above.
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no setgid or anything like it
	#endif
}

#ifdef RUDIMENTS_HAVE_SETEGID_BUT_NOT_DEFINED
extern "C" int setegid(gid_t egid);
#endif

bool process::setEffectiveGroupId(gid_t gid) {
	#if defined(RUDIMENTS_HAVE_SETEGID)
		return !setegid(gid);
	#elif defined(_WIN32)
		// windows doesn't have the notion of effective group id's
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no setegid or anything like it
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
	return ::umask(mask);
}

pid_t process::fork() {
	#if defined(RUDIMENTS_HAVE_FORK)
		pid_t	result;
		do {
			result=::fork();
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
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
		_exit(0);
	}
	
	#ifdef RUDIMENTS_HAVE_SETSID
		// become process group and session group leader 
		// with no controlling terminal
		setsid();
	#endif

	// change directory to root to avoid keeping any directories in use
	directory::changeDirectory("/");

	// Set umask such that files are created 666 and directories 777.
	// This way we can change them to whatever we like using chmod().
	// We want to avoid inheriting a umask which wouldn't give us write
	// permissions to files we create.
	process::setFileCreationMask(0);

	return true;
}

void process::exit(int32_t status) {
	::exit(status);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
