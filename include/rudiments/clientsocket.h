// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CLIENTSOCKET_H
#define RUDIMENTS_CLIENTSOCKET_H

#include <rudiments/private/clientsocketincludes.h>

class clientsocket : public client {
	public:
			clientsocket();
		virtual	~clientsocket();

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

#endif
