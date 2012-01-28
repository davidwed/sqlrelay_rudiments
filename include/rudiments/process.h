// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROCESS_H
#define RUDIMENTS_PROCESS_H

#include <rudiments/private/processincludes.h>

// wrap:
//	unistd.h - nice()
//			getgroups()
//			vfork()
//			profil()
//			acct()
//			getdtablesize()
//			brk()/sbrk()
//			syscall()
//	sched.h - functions for manipulating the scheduler
//			sched_setparam(),sched_getparam()
//			sched_setscheduler(),sched_getscheduler()
//			sched_yield(),
//			sched_get_priority_max(),sched_get_priority_min()
//			sched_rr_get_interval()
//			sched_setaffinity(),sched_getaffinity()
//	sys/acct.h - acct() - process accounting on/off
//	sys/ptrace.h - ptrace()
//	sys/resource.h - get/set process priority/resource usage
//				getrlimit(),setrlimit()
//				getrusage()
//				getpriority(),setpriority()
//	sys/times.h - times() - function for getting process times
//	not in many systems - 
//		unistd.h - group_member()
//				getresuid(),getresgid()
//				setresuid(),setresgid()
//		sys/gmon.h and sys/gmon_out.h - monstartup()
//	linux only - 
//		execinfo.h - backtrace(),backtrace_symbols(),
//				backtrace_symbols_fd()
//		sys/prctl.h - prctl()
//	not in bsd - 
//		ucontext.h - user-level context switching
//			getcontext(),setcontext(),swapcontext(),makecontext()

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class process {
	public:

		static	pid_t	getProcessId();
				// returns the process id of the current process
		static	pid_t	getParentProcessId();
				// returns the process id of the parent process
				// of the current process

		static	pid_t	getProcessGroupId();
				// returns the process group id of the
				// current process
		static	pid_t	getProcessGroupId(pid_t pid);
				// returns the process group id of the
				// process pid

		static	bool	setProcessGroupId();
				// sets the process group id of the current
				// process to the current process id
		static	bool	setProcessGroupId(pid_t pgid);
				// sets the process group id of the current
				// process to pgid
		static	bool	setProcessGroupId(pid_t pid, pid_t pgid);
				// sets the process group id of the process
				// pid to pgid

		static	pid_t	getSessionId();
				// returns the session id of the current process
		static	pid_t	getSessionId(pid_t pid);
				// returns the session id of the process pid

		static	uid_t	getRealUserId();
				// returns the real user id of the
				// current process
		static	uid_t	getEffectiveUserId();
				// returns the effective user id of the
				// current process

		static bool	setUserId(uid_t uid);
				// Sets the effective user id of the current
				// process to uid.  If the effective user id
				// is root, the real and saved user id's are
				// also set.
				// Returns true on success and false on failure.
		static bool	setEffectiveUserId(uid_t uid);
				// Sets the effective user id of the current
				// process to uid.  Does not set the real
				// or saved user id's.
				// Returns true on success and false on failure.
		static bool	setRealAndEffectiveUserId(uid_t uid,
								uid_t euid);
				// Sets the real user id of the current process
				// to uid and the effective user id of
				// the current process to euid.
				// If the real user id is set or the effective
				// user id is set to a value not equal to the
				// previous real user id, the saved user id is
				// set to the new effective user id.
				// Returns true on success and false on failure.

		static	gid_t	getRealGroupId();
				// returns the real group id of the
				// current process
		static	gid_t	getEffectiveGroupId();
				// returns the effective group id of the
				// current process

		static bool	setGroupId(gid_t gid);
				// Sets the effective group id of the current
				// process to gid.  If the effective group id
				// is root, the real and saved group id's are
				// also set.
				// Returns true on success and false on failure.
		static bool	setEffectiveGroupId(gid_t gid);
				// Sets the effective group id of the current
				// process to gid.  Does not set the real
				// or saved group id's.
				// Returns true on success and false on failure.
		static bool	setRealAndEffectiveGroupId(gid_t gid,
								gid_t egid);
				// Sets the real group id of the current process
				// to gid and the effective group id of
				// the current process to egid.
				// If the real group id is set or the effective
				// group id is set to a value not equal to the
				// previous real group id, the saved group id is
				// set to the new effective group id.
				// Returns true on success and false on failure.

		static mode_t	setFileCreationMask(mode_t mask);
				// Set file/directory creation mode mask to
				// "mask".  Returns the mask that was previously
				// in effect.

		static pid_t	fork();
				// Creates a child process.  The child is a
				// duplicate of the parent inheriting file
				// descriptors and a copy of the parent's
				// address space.  The child does not have
				// access to the parent's address space.
				// In the parent process, the process id of
				// the child is returned.  In the child
				// process, 0 is returned.  -1 is returned if
				// an error occurred and no child is forked.

		static bool	detach();
				// Detaches the current process from the
				// controlling tty, creates a new session,
				// changes directory to / and sets the file
				// creation mask to 0.
				// Use this method to "fork your process into
				// the background."
				// Returns true on success and false upon
				// failure.

		static void	exit(int32_t status);
				// Terminates the calling process and sets the
				// exit status to "status".
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
