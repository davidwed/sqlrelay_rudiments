// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ENVIRONMENT_H
#define RUDIMENTS_ENVIRONMENT_H

#include <rudiments/private/environmentincludes.h>

// The environment class provides methods for accessing and manipulating
// environment variables.

class environment {
	public:
		virtual ~environment();

		char	*getValue(const char *variable) const;
			// Returns the value of "variable".
		bool	setValue(const char *variable, const char *value);
			// Sets the value of "variable" to "value",
			// Overwriting any value that "variable" previously
			// had.
			// Returns true on success and false on failure.
		void	remove(const char *variable);
			// Removes "variable" from the environment.

	#include <rudiments/private/environment.h>

};

#endif
