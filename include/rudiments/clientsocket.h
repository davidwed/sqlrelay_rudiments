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

#ifdef FIONBIO
		virtual bool	useNonBlockingMode() const;
				// Puts the file descriptor in non-blocking
				// mode.  Returns true on success and false on
				// failure.
		virtual bool	useBlockingMode() const;
				// Puts the file descriptor in blocking mode.
				// Returns true on success and false on
				// failure.
#endif

	#include <rudiments/private/clientsocket.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
