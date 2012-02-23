// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETUTIL_H
#define RUDIMENTS_UNIXSOCKETUTIL_H

#include <rudiments/private/unixsocketutilincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class unixsocketutilprivate;

/** The unixsocketutil class is just a base class for other classes that use
 *  unix sockets.  It is unlikely to need to be used directly. */
class RUDIMENTS_DLLSPEC unixsocketutil {
	public:

		/** Creates an instance of the unixsocketutil class. */
		unixsocketutil();

		/** Creates an instance of the unixsocketutil class
		 *  that is a copy of "u". */
		unixsocketutil(const unixsocketutil &u);

		/** Makes this instance of the unixsocketutil class
		 *  identical to "u". */
		unixsocketutil	&operator=(const unixsocketutil &u);

		/** Deletes this instance of the unixsocketutill class. */
		virtual		~unixsocketutil();

		/** Initializes the class to use "filename".  This is not
		 *  directly useful within this class but child classes may
		 *  use the value to decide where to connect to or what to
		 *  listen on. */
		void	initialize(const char *filename);

	#include <rudiments/private/unixsocketutil.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
