// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVERSOCKET_H
#define RUDIMENTS_SERVERSOCKET_H

#include <rudiments/private/serversocketincludes.h>

class serversocket : public server {
	public:
			serversocket();
		virtual	~serversocket();

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

		bool	lingerOnClose(int timeout);
			// Instructs the socket to stay open for
			// "timeout" seconds even after close() is
			// called to allow it to receive any data that
			// may still be buffered.
			//
			// Returns true on success and false on failure.
		bool	dontLingerOnClose();
			// Instructs the socket to close immediately
			// when close() is called, dumping any data
			// that may still be buffered but that it
			// may not have received.  This is the default.
			//
			// Returns true on success and false on failure.
		bool	reuseAddresses();
			// Allows sockets in the TIME_WAIT state to be
			// reused.
			//
			// Returns true on success and false on failure.
		bool	dontReuseAddresses();
			// Prevents sockets in the TIME_WAIT state from
			// being reused.  This is the default.
			//
			// Returns true on success and false on failure.

		virtual	bool	listen(int backlog);
			// Waits until a client connects then places
			// that connection in queue.  Up to "backlog"
			// connections may be queued before future
			// conenctions are refused.
			//
			// Returns true on success and false on failure.

	#include <rudiments/private/serversocket.h>
};

#endif
