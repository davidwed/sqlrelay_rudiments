// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENTSOCKET_H
#define RUDIMENTS_CLIENTSOCKET_H

#include <rudiments/private/clientsocketincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class clientsocketprivate;

class RUDIMENTS_DLLSPEC clientsocket : public client {
	public:
				clientsocket();
				clientsocket(const clientsocket &c);
		clientsocket	&operator=(const clientsocket &c);
		virtual		~clientsocket();

		/** Returns true if the system supports blocking/nonblocking
		 *  modes and false otherwise. */
		virtual bool	supportsBlockingNonBlockingModes();

		/** Puts the file descriptor in non-blocking mode.  Returns true
		 *  on success and false on failure.  Returns false if the
		 *  system doesn't support blocking/nonblocking modes. */
		virtual bool	useNonBlockingMode() const;

		/** Puts the file descriptor in blocking mode.  Returns true on
		 *  success and false on failure.  Returns false if the system
		 *  doesn't support blocking/nonblocking modes. */
		virtual bool	useBlockingMode() const;

	#include <rudiments/private/clientsocket.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
