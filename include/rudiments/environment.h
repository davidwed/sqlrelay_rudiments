// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ENVIRONMENT_H
#define RUDIMENTS_ENVIRONMENT_H

#include <rudiments/private/environmentincludes.h>

class environmentprivate;

/** The environment class provides methods for accessing and manipulating
 *  environment variables. */
class RUDIMENTS_DLLSPEC environment {
	public:
		/** Returns the value of "variable". */
		static	const char	*getValue(const char *variable);

		/** Sets the value of "variable" to
		 *  "value", overwriting any value that
		 *  "variable" previously had.
		 *  Returns true on success and false on
		 *  failure. */
		static	bool		setValue(const char *variable,
						const char *value);

		/** Removes "variable" from the environment. */
		static	bool		remove(const char *variable);

		/** Returns a NULL terminated list of all
		 *  environment variables.  Each entry in the list is
		 *  of the form NAME=VALUE. */
		static	const char * const	*variables();

		/** Clears the environment of all name-value
		 *  pairs.  After calling this, variables()
		 *  will return NULL. */
		static	bool	clear();

		/** Allows you to supply a mutex if the class needs it.
		 *  If your application is not multithreaded, then
		 *  there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/environment.h>
};

#endif
