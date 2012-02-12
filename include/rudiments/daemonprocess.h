// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DAEMONPROCESS_H
#define RUDIMENTS_DAEMONPROCESS_H

#include <rudiments/private/daemonprocessincludes.h>

// Daemons are long running processes which often detach themselves from
// the controlling terminal and run in the background.  They are frequently
// started at boot time and run until the machine is shut down.
//
// Daemons typically perform "housecleaning" tasks or serve data to client
// programs.  See the server class.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class daemonprocessprivate;

class DLLSPEC daemonprocess {
	public:
			daemonprocess();
		virtual	~daemonprocess();

		static int64_t	checkForPidFile(const char *filename);
			// Checks for filename "filename" and reads the
			// process id out of it, if it exists.  Returns
			// the process id on success or -1 on failure.
		static bool	createPidFile(const char *filename,
						mode_t permissions);
			// Create's file "filename" with permissions
			// "permissions" and puts the current process
			// id in it.  Note that when you delete this
			// file during shutdown you must use the full
			// pathname since the detach() method below
			// changes directories to "/".  Returns true on
			// success and false on failure.

		bool	detach() const;
			// Detach from the controlling terminal and
			// process and run in the background.  Also
			// change directories to "/" and set the file
			// creation mask such that all files are 
			// created -rw-rw-rw and all directories 
			// drwxrwxrwx.

		// These methods allow the daemon to run as a different
		// user or group than the one that started the process.
		// They have no effect unless the process is started
		// by the root user.
		//
		// These methods return 1 on success, 0 on failure and
		// -1 on error.
		int32_t	runAsUser(const char *username) const;
			// Note that runAsUser() uses the passwdentry
			// class.  If you are using this method in a
			// multithreaded application, you may need to
			// supply the passwdentry classes a mutex.
			// See passwdentry.h for more detail.
		int32_t	runAsGroup(const char *groupname) const;
			// Note that runAsGroup() uses the groupentry
			// class.  If you are using this method in a
			// multithreaded application, you may need to
			// supply the groupentry classes a mutex.
			// See groupentry.h for more detail.
		int32_t	runAsUserId(uid_t uid) const;
		int32_t	runAsGroupId(gid_t gid) const;

		static	void	handleShutDown(
					void (*shutdownfunction)(int32_t));
				// This method allows you to designate a
				// function to run when the daemon is killed.

		static	void	handleCrash(void (*crashfunction)(int32_t));
				// This method allows you to designate a
				// function to run if the daemon crashes.

		static	void	waitForChildren();
				// This method causes the daemon to wait
				// on child processes which have exited,
				// preventing so-called "zombie" processes
				// from occurring.  This method is called
				// in the constructor and is thus the default
				// behavior of this class.
		static	void	dontWaitForChildren();
				// This method causes the daemon not to
				// wait on child processes which have exited.
				// Ordinarily, you'd want to wait on child
				// processes, but this interferes with the
				// behavior of WEXITSTATUS() after a call to
				// system() (and possibly other calls).  This
				// method allows you to disable waiting on
				// child processes.

	#include <rudiments/private/daemonprocess.h>

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
