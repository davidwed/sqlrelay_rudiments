// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVERSOCKET_H
#define RUDIMENTS_SERVERSOCKET_H

#include <rudiments/server.h>

class serversocket : public server {
	public:

		// These 4 methods allow you to set socket options.  They must
		// be called after calling bind() and before calling listen().
		int	lingerOnClose(int timeout);
			// Instructs the socket to stay open for
			// "timeout" seconds even after close() is
			// called to allow the client to receive
			// any data that may still be buffered.
			//
			// Returns 1 on success and 0 on failure.
		int	dontLingerOnClose();
			// Instructs the socket to close immediately
			// when close() is called, dumping any data
			// that may still be buffered but that the
			// client may not have received.  This is
			// the default.
			//
			// Returns 1 on success and 0 on failure.
		int	reuseAddresses();
			// Allows sockets in the TIME_WAIT state to be
			// reused.
			//
			// Returns 1 on success and 0 on failure.
		int	dontReuseAddresses();
			// Prevents sockets in the TIME_WAIT state from
			// being reused.  This is the default.
			//
			// Returns 1 on success and 0 on failure.


		virtual	int	listen(int backlog);
				// Waits until a client connects then places
				// that connection in queue.  Up to "backlog"
				// connections may be queued before future
				// conenctions are refused.
				//
				// Returns 1 on success and 0 on failure.

	#include <rudiments/private/serversocket.h>
};

#ifdef ENABLE_INLINES
	#define INLINE inline
	#include <rudiments/private/serversocketinlines.h>
#endif

#endif
