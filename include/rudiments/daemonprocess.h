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

class daemonprocess {
	public:
				daemonprocess();
			virtual	~daemonprocess();

			int	checkForPidFile(const char *filename) const;
				// Checks for filename "filename" and reads the
				// process id out of it, if it exists.  Returns
				// the process id on success or 0 on failure.
			int	createPidFile(const char *filename,
						mode_t permissions);
				// Create's file "filename" with permissions
				// "permissions" and puts the current process
				// id in it.  Note that when you delete this
				// file during shutdown you must use the full
				// pathname since the detach() method below
				// changes directories to "/".  Returns 1 on
				// success and 0 on failure.

			void	detach() const;
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
			int	runAsUser(const char *username) const;
				// Note that runAsUser() uses the passwdentry
				// class.  If you are using this method in a
				// multithreaded application, you may need to
				// supply the passwdentry classes a mutex.
				// See passwdentry.h for more detail.
			int	runAsGroup(const char *groupname) const;
				// Note that runAsGroup() uses the groupentry
				// class.  If you are using this method in a
				// multithreaded application, you may need to
				// supply the groupentry classes a mutex.
				// See groupentry.h for more detail.
			int	runAsUserId(uid_t uid) const;
			int	runAsGroupId(gid_t gid) const;

		static	void	handleShutDown(void *shutdownfunction);
				// This method allows you to designate a
				// function to run when the daemon is killed.

		static	void	handleCrash(void *crashfunction);
				// This method allows you to designate a
				// function to run if the daemon crashes.

	#include <rudiments/private/daemonprocess.h>

};

#ifdef ENABLE_INLINES
	#include <rudiments/private/daemonprocessinlines.h>
#endif

#endif
