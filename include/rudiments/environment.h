// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ENVIRONMENT_H
#define RUDIMENTS_ENVIRONMENT_H

#include <rudiments/private/environmentincludes.h>

// The environment class provides methods for accessing and manipulating
// environment variables.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class environmentprivate;

class DLLSPEC environment {
	public:
		static	const char	*getValue(const char *variable);
					// Returns the value of "variable".
		static	bool		setValue(const char *variable,
						const char *value);
					// Sets the value of "variable" to
					// "value", overwriting any value that
					// "variable" previously had.
					// Returns true on success and false on
					// failure.
		static	bool		remove(const char *variable);
					// Removes "variable" from the
					// environment.

		static	const char * const	*variables();
			// Returns a NULL terminated list of all
			// environment variables.  Each entry in the list is
			// of the form NAME=VALUE.

		static	bool	clear();
				// Clears the environment of all name-value
				// pairs.  After calling this, variables()
				// will return NULL.

		static	void	print();
				// Prints the environment variable list to
				// stdout.

		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex if the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/environment.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
