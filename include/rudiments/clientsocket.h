// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENTSOCKET_H
#define RUDIMENTS_CLIENTSOCKET_H

#include <rudiments/private/clientsocketincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class clientsocketprivate;

/** The clientsocket class provides a base class for classes that implement
 *  the client side of a client-server communication model over a socket. */
class RUDIMENTS_DLLSPEC clientsocket : public client {
	public:

		/** Creates an instance of the clientsocket class. */
		clientsocket();

		/** Creates an instance of the clientsocket class
		 *  that is a copy of "c". */
		clientsocket(const clientsocket &c);

		/** Makes this instance of the clientsocket class
		 *  idenitcal to "c". */
		clientsocket	&operator=(const clientsocket &c);

		/** Deletes this instance of the clientsocket class. */
		virtual	~clientsocket();

		/** Returns true if the client socket supports
		 *  blocking/nonblocking modes and false otherwise. */
		virtual bool	supportsBlockingNonBlockingModes();

		/** Puts the client socket in non-blocking mode.  Returns true
		 *  on success and false on failure.  Returns false if the
		 *  system doesn't support blocking/nonblocking modes. */
		virtual bool	useNonBlockingMode() const;

		/** Puts the client socket in blocking mode.  Returns true on
		 *  success and false on failure.  Returns false if the system
		 *  doesn't support blocking/nonblocking modes. */
		virtual bool	useBlockingMode() const;

	#include <rudiments/private/clientsocket.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
