// FIXME: implement this class

// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHELLCOMMAND_H
#define RUDIMENTS_SHELLCOMMAND_H

#include <rudiments/private/shellcommandincludes.h>

// The shellcommand class provides methods for running commands, reading their
// output or writing input to them.

// wrap:
//	stdlib.h - system()
//	unistd.h - execve()/fexecve(),execv(),execle(),execl(),execvp(),execlp()
//	stdio.h - popen(), pclose()
//	not in many systems -
//		spawn.h - posix spawn interface
//				posix_spawn()
//				posix_spawnp()
//				posix_spawnattr_init()
//				posix_spawnattr_destroy()
//				posix_spawnattr_getsigdefault()
//				posix_spawnattr_setsigdefault()
//				posix_spawnattr_getsigmask()
//				posix_spawnattr_setsigmask()
//				posix_spawnattr_getflags()
//				posix_spawnattr_setflags()
//				posix_spawnattr_getpgroup()
//				posix_spawnattr_setpgroup()
//				posix_spawnattr_getschedpolicy()
//				posix_spawnattr_setschedpolicy()
//				posix_spawnattr_getschedparam()
//				posix_spawnattr_setschedparam()
//				posix_spawn_file_actions_init()
//				posix_spawn_file_actions_destroy()
//				posix_spawn_file_actions_addopen()
//				posix_spawn_file_actions_addclose()
//				posix_spawn_file_actions_adddup2()

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class shellcommandprivate;

class shellcommand {
	public:
		int	run(const char *command);
			// Runs "command".

		int	getStandardInput();
			// If run() succeeded, returns the file descriptor
			// of the standard input of the command or 0 otherwise.

		int	getStandardOutput();
			// If run() succeeded, returns the file descriptor
			// of the standard output of the command or 0 otherwise.

		int	getStandardError();
			// If run() succeeded, returns the file descriptor
			// of the standard error of the command or 0 otherwise.

		int	close();
			// Closes the previously run command.
			//
			// Returns 1 on success and 0 on failure.

	#include <rudiments/private/shellcommand.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
