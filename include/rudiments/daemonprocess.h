// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DAEMONPROCESS_H
#define RUDIMENTS_DAEMONPROCESS_H

#include <rudiments/private/daemonprocessincludes.h>

class daemonprocessprivate;

/** The daemonprocess class provides methods that are useful to daemons.
 *
 *  Daemons are long running processes which often detach themselves from
 *  the controlling terminal and run in the background.  They are frequently
 *  started at boot time and run in the background.
 * 
 *  Daemons typically perform "housecleaning" tasks or serve data to client
 *  programs.  See the server class. */
class RUDIMENTS_DLLSPEC daemonprocess {
	public:

		/** Creates an instance of the daemonprocess class. */
		daemonprocess();

		/** Deletes this instance of the daemonprocess class. */
		virtual	~daemonprocess();

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

		/** Allows you to designate a function to run when the
		 *  daemon is killed. */
		static	void	handleShutDown(
					void (*shutdownfunction)(int32_t));

		/** Allows you to designate a function to run if the
		 *  daemon crashes. */
		static	void	handleCrash(void (*crashfunction)(int32_t));

		/** This method causes the daemon to wait on child processes
		 *  which have exited, preventing so-called "zombie" processes
		 *  from occurring.  This method is called in the constructor
		 *  and is thus the default behavior of this class. */
		static	void	waitForChildren();

		/** This method causes the daemon not to wait on child
		 *  processes which have exited.  Ordinarily, you'd want to
		 *  wait on child processes, but this interferes with the
		 *  behavior of WEXITSTATUS() after a call to system() (and
		 *  possibly other calls).  This method allows you to disable
		 *  waiting on child processes. */
		static	void	dontWaitForChildren();

	#include <rudiments/private/daemonprocess.h>

};

#endif
