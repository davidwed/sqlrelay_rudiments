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

		/** Looks for the argument or an abbreviated version of it on
		 *  the command line and returns the value.
		 *
		 *  For example, if "arg" is "name" and "abbr" is "n" then it
		 *  looks for either:
		 *
		 * 	-name value
		 *  or
		 * 	--name=value
		 *  or
		 *  	-n value
		 *  or
		 *  	--n==value
		 *
		 *  on the command line and returns value.
		 *
		 *  If both "arg" and "abbr" are found then the value for
		 *  "arg" is returned. */
		const char	*getValue(const char *arg,
						const char *abbr) const;

		/** Returns true if "-arg", "--arg" or "--arg=value" was found
		 *  on the command line and false if it was not found.
		 *
		 *  This is useful for processing command line switches. */
		bool	found(const char *arg) const;

		/** Returns true if the argument or an abbreviated version of
		 *  it was found on the command line and false if neither were
		 *  found.
		 *
		 *  For example, if "arg" is "recursive" and "abbr" is "r" then
		 *  it returns true if "-recursive", "--recursive", "-r", or
		 *  "--r" was found on the command line.
		 *
		 *  This is useful for processing command line switches. */
		bool	found(const char *arg, const char *abbr) const;

	#include <rudiments/private/commandline.h>

};

#endif
