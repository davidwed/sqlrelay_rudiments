// FIXME: implement this class

// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHELLCOMMAND_H
#define RUDIMENTS_SHELLCOMMAND_H

#include <rudiments/private/shellcommandincludes.h>

// The shellcommand class provides methods for running commands, reading their
// output or writing input to them.

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

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/shellcommandinlines.h>
#endif

#endif
