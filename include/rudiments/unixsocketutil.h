// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETUTIL_H
#define RUDIMENTS_UNIXSOCKETUTIL_H

#include <rudiments/private/unixsocketutilincludes.h>

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
		void		initialize(const char *filename);

		/** Converts "filename" to a port number for faking unix
		 *  sockets.  If the filename starts with a number, then
		 *  that number is used.  Otherwise it starts with 10240 and
		 *  adds the ascii values of each character in the name.
		 *  The port number is not guaranteed to be unique for unique
		 *  names unless those names are based on unique numbers. */
		uint16_t	filenameToPort(const char *filename);

	#include <rudiments/private/unixsocketutil.h>
};

#endif
