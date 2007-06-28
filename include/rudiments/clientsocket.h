// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENTSOCKET_H
#define RUDIMENTS_CLIENTSOCKET_H

#include <rudiments/private/clientsocketincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class clientsocketprivate;

class clientsocket : public client {
	public:
				clientsocket();
				clientsocket(const clientsocket &c);
		clientsocket	&operator=(const clientsocket &c);
		virtual		~clientsocket();

		virtual bool	supportsBlockingNonBlockingModes();
				// Returns false if the system doesn't support
				// blocking/nonblocking modes and true
				// otherwise.

		virtual bool	useNonBlockingMode() const;
				// Puts the file descriptor in non-blocking
				// mode.  Returns true on success and false on
				// failure.
				// Returns false if the system doesn't support
				// blocking/nonblocking modes.
		virtual bool	useBlockingMode() const;
				// Puts the file descriptor in blocking mode.
				// Returns true on success and false on
				// failure.
				// Returns false if the system doesn't support
				// blocking/nonblocking modes.

	#include <rudiments/private/clientsocket.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
