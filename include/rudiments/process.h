// Copyright (c) 1999-2013 David Muse
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

/** The process class provides methods for accessing information about and
 *  controlling the currently running process. */
class RUDIMENTS_DLLSPEC process {
	public:

		/** Returns the process id of the current process. */
		static	pid_t	getProcessId();

		/** Returns the process id of the parent process
		 *  of the current process. */
		static	pid_t	getParentProcessId();

		/** Returns the process group id of the
		 *  current process. */
		static	pid_t	getProcessGroupId();

		/** Returns the process group id of the
		 *  process pid. */
		static	pid_t	getProcessGroupId(pid_t pid);

		/** Sets the process group id of the current
		 *  process to the current process id. */
		static	bool	setProcessGroupId();

		/** Sets the process group id of the current process to pgid. */
		static	bool	setProcessGroupId(pid_t pgid);

		/** Sets the process group id of the process pid to pgid. */
		static	bool	setProcessGroupId(pid_t pid, pid_t pgid);

		/** Returns the session id of the current process. */
		static	pid_t	getSessionId();

		/** Returns the session id of the process pid. */
		static	pid_t	getSessionId(pid_t pid);

		/** Creates a new session, sets the calling process to be the
		 *  process group leader and detaches from the controlling
		 *  terminal.  Returns the session id on success or -1 if an
		 *  error occurred. */
		static	pid_t	newSession();

		/** Returns the real user id of the current process. */
		static	uid_t	getRealUserId();

		/** Returns the effective user id of the current process. */
		static	uid_t	getEffectiveUserId();

		/** Sets the effective user id of the current process to uid.
		 *  If the effective user id is root, the real and saved user
		 *  id's are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setUserId(uid_t uid);

		/** Sets the effective user id of the current process to the
 		 *  user id of "username".  If the effective user id is root,
 		 *  the real and saved user id's are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setUser(const char *username);

		/** Sets the effective user id of the current process to uid.
		 *  Does not set the real or saved user id's.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setEffectiveUserId(uid_t uid);

		/** Sets the real user id of the current process to uid and the
		 *  effective user id of the current process to euid.  If the
		 *  real user id is set or the effective user id is set to a
		 *  value not equal to the previous real user id, the saved
		 *  user id is set to the new effective user id.
		 * 
		 *  Returns true on success and false on failure. */
		static bool	setRealAndEffectiveUserId(uid_t uid,
								uid_t euid);

		/** Returns the real group id of the current process. */
		static	gid_t	getRealGroupId();

		/** Returns the effective group id of the current process. */
		static	gid_t	getEffectiveGroupId();

		/** Sets the effective group id of the current process to gid.
		 *  If the effective group id is root, the real and saved group
		 *  id's are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setGroupId(gid_t gid);

		/** Sets the effective group id of the current process to the
 		 *  group id of "groupname".  If the effective group id is root,
 		 *  the real and saved group id's are also set.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setGroup(const char *groupname);

		/** Sets the effective group id of the current process to gid.
		 *  Does not set the real or saved group id's.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setEffectiveGroupId(gid_t gid);

		/** Sets the real group id of the current process to gid and
		 *  the effective group id of the current process to egid.  If
		 *  the real group id is set or the effective group id is set
		 *  to a value not equal to the previous real group id, the
		 *  saved group id is set to the new effective group id.
		 *
		 *  Returns true on success and false on failure. */
		static bool	setRealAndEffectiveGroupId(gid_t gid,
								gid_t egid);

		/** Set file/directory creation mode mask to "mask".  Returns
		 *  the mask that was previously in effect. */
		static mode_t	setFileCreationMask(mode_t mask);

		/** Creates a child process.  The child is a duplicate of the
		 *  parent inheriting file descriptors and a copy of the
		 *  parent's address space.  The child does not have access to
		 *  the parent's address space.  In the parent process, the
		 *  process id of the child is returned.  In the child process,
		 *  0 is returned.  -1 is returned if an error occurred and no
		 *  child is forked. */
		static pid_t	fork();

		/** Runs "command" with arguments "args", replacing the current
		 *  running process with this new process.  Whether the new
		 *  process retains the process id of the current process is
		 *  platform-specific.  Note that the first element of array
		 *  "args" should be the name of the command that you want to
		 *  run, typically the same as "command".  "args" should be NULL
		 *  terminated.  Returns false if an error occurred and
		 *  otherwise does not return at all. */
		static bool	exec(const char *command,
					const char * const *args);

		/** Runs "command" with arguments "args" as a new process.
		 *  Use this instead of a combinination of fork() and exec()
		 *  as it is more efficient on some platforms.
		 *  Note that the first element of array "args" should be the
		 *  name of the command that you want to run, typically the
		 *  same as "command".  "args" should be NULL terminated.
		 *  Returns the process id of the child process or -1 if an
		 *  error occurred. */
		static pid_t	spawn(const char *command,
					const char * const *args);

		/** Detaches the current process from the controlling tty,
		 *  creates a new session, changes directory to / and sets the
		 *  file creation mask to 0.  Use this method to "fork your
		 *  process into the background."  Returns true on success and
		 *  false upon failure. */
		static bool	detach();

		/** Terminates the calling process and sets the exit status to
		 *  "status". */
		static void	exit(int32_t status);

		/** Terminates the calling process "immediately" (without
		  * calling any functions registered to run at exit) and sets
		  * the exit status to "status". */
		static void	exitImmediately(int32_t status);

		/** Calls exitOnShutDown(), exitOnCrash() and
		 *  waitForChildren() below. */
		static	void	exitOnCrashOrShutDown();

		/** Checks for filename "filename" and reads the
		 *  process id out of it, if it exists.  Returns
		 *  the process id on success or -1 on failure. */
		static int64_t	checkForPidFile(const char *filename);

		/** Create's file "filename" with permissions
		 *  "permissions" and puts the current process
		 *  id in it.  Note that when you delete this
		 *  file during shutdown you must use the full
		 *  pathname since the process::detach() method
		 *  changes directories to "/".  Returns true on
		 *  success and false on failure. */
		static bool	createPidFile(const char *filename,
						mode_t permissions);

		/** Sets up a default handler that exits cleanly when the
		 *  daemon is killed with SIGINT or SIGTERM signals.
		 *  NOTE: The default handler calls waitForChildren() before
		 *  exiting to prevent zombie processes. */
		static void	exitOnShutDown();

		/** Allows you to designate a function to run when the
		 *  daemon is killed with SIGINT or SIGTERM signals. */
		static	void	handleShutDown(
					void (*shutdownfunction)(int32_t));

		/** Sets up a default handler that exits cleanly if the
		 *  daemon crashes with a SIGSEGV (segmentation fault)
		 *  signal.
		 *  NOTE: The default handler calls waitForChildren() before
		 *  exiting to prevent zombie processes. */
		static void	exitOnCrash();

		/** Allows you to designate a function to run if the
		 *  daemon crashes with a SIGSEGV (segmentation fault)
		 *  signal. */
		static	void	handleCrash(void (*crashfunction)(int32_t));

		/** This method causes the daemon to wait on child processes
		 *  which have exited, preventing so-called "zombie" processes
		 *  from occurring. */
		static	void	waitForChildren();

		/** This method causes the daemon not to wait on child
		 *  processes which have exited.  Ordinarily, you'd want to
		 *  wait on child processes, but this interferes with the
		 *  behavior of WEXITSTATUS() after a call to system() (and
		 *  possibly other calls).  This method allows you to disable
		 *  waiting on child processes. */
		static	void	dontWaitForChildren();

	#include <rudiments/private/process.h>
};

#endif
