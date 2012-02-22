// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKETUTIL_H
#define RUDIMENTS_INETSOCKETUTIL_H

#include <rudiments/private/inetsocketutilincludes.h>

/** The inetsocketutil class is just a base class for other classes that use
 *  inet sockets.  It is unlikely to need to be used directly. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class inetsocketutilprivate;

class RUDIMENTS_DLLSPEC inetsocketutil {
	public:

		/** Creates an instance of the inetsocketutil class. */
		inetsocketutil();

		/** Creates an instance of the inetsocketutil class
		 *  that is a copy of "i". */
		inetsocketutil(const inetsocketutil &i);

		/** Makes this instance of the inetsocketutil class
		 *  identical to "i". */
		inetsocketutil	&operator=(const inetsocketutil &i);

		/** Deletes this instance of the inetsocketutil class. */
		virtual		~inetsocketutil();

		/** Initializes the class to use "address" and "port".
		 *  This is not directly useful within this class, but child
		 *  classes may use these values to decide where to connect to
		 *  or what to listen on. */
		void		initialize(const char *address, uint16_t port);

	#include <rudiments/private/inetsocketutil.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
