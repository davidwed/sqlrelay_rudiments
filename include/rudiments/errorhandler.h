// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ERRORHANDLER_H
#define RUDIMENTS_ERRORHANDLER_H

#include <rudiments/private/errorhandlerincludes.h>

// The errorhandler class is a base class for many rudiments classes.

class errorhandler {
	public:
		char	*getError();
			// When a method in a class that inherits from
			// errorhandler returns a return value indicating an
			// error, this method may called to retrieve the
			// error message.

	protected:
		void	clearError();
		void	appendError(const char *string);
		void	appendError(char character);
		void	appendError(long number);
		void	appendError(double number);

	#include <rudiments/private/errorhandler.h>

};

#endif
