// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COMMANDLINE_H
#define RUDIMENTS_COMMANDLINE_H

#include <rudiments/private/commandlineincludes.h>

// The commandline class provides methods for parsing command line arguments.

class commandline {
	public:
			commandline(int argc, const char **argv);
		virtual	~commandline();

		char	*value(const char *arg) const;
				// Returns the value of "arg" (which is assumed
				// to be the next item on the command line
				// after "arg").
		int	found(const char *arg) const;
				// Returns 1 if "arg" was found and 0 
				// if it was not found.  This is useful for
				// processing command line switches.

	#include <rudiments/private/commandline.h>

};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/commandlineinlines.h>
#endif

#endif
