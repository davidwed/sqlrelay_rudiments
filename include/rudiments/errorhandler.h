// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <rudiments/private/config.h>

#include <rudiments/stringbuffer.h>

// The errorhandler class is a base class for many rudiments classes.

class errorhandler {
	public:
		char	*getError();
			// When a method in a class that inherits from
			// errorhandler returns a return value indicating an
			// error, this method may called to retrieve the
			// error message.

	#include <rudiments/private/errorhandler.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/errorhandlerinlines.h>
#endif

#endif
