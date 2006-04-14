// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COMMANDLINE_H
#define RUDIMENTS_COMMANDLINE_H

#include <rudiments/private/commandlineincludes.h>

// The commandline class provides methods for parsing command line arguments.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class commandlineprivate;

class commandline {
	public:
		commandline(int argc, const char **argv);
		~commandline();

		const char	*getValue(const char *arg) const;
				// Looks for either:
				//	-arg value
				// or
				//	--arg=value
				// on the command line and returns value.
				//
				// The "arg" parameter may be specified as
				// any of "arg", "-arg" or "--arg".

		bool		found(const char *arg) const;
				// Returns true if "-arg", "-arg value",
				// "--arg" or "--arg=value" was found on the
				// command line and false if it was not found.
				// This is useful for processing command line
				// switches.
				//
				// The "arg" parameter may be specified as
				// any of "arg", "-arg" or "--arg".

	#include <rudiments/private/commandline.h>

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
