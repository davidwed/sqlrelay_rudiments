// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COMMANDLINE_H
#define RUDIMENTS_COMMANDLINE_H

#include <rudiments/private/commandlineincludes.h>

class commandlineprivate;

/** The commandline class provides methods for parsing command line
 *  arguments. */
class RUDIMENTS_DLLSPEC commandline {
	public:

		/** Creates an instance of the commandline class. */
		commandline(int32_t argc, const char **argv);

		/** Deletes this instance of the commandline class. */
		~commandline();

		/** Looks for either:
		 * 	-arg value
		 *  or
		 * 	--arg=value
		 *  on the command line and returns value. */
		const char	*getValue(const char *arg) const;

		/** Returns true if "-arg", "--arg" or "--arg=value" was found
		 *  on the command line and false if it was not found.  This is
		 *  useful for processing command line switches. */
		bool	found(const char *arg) const;

	#include <rudiments/private/commandline.h>

};

#endif
