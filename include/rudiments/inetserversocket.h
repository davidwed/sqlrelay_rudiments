// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef INETSERVERSOCKET_H
#define INETSERVERSOCKET_H

#include <rudiments/private/config.h>

#include <rudiments/private/serversocket.h>
#include <rudiments/private/inetsocket.h>

// The inetserversocket class allows you to write programs that can talk to
// other programs across a network over TCP stream sockets.
//
// The inetserversocket class provides methods for setting up sockets and
// accepting client connections.  Its ultimate parent class: transport,
// provides methods for reading and writing data and closing connections.
//
// If you need to listen on more than 1 socket at a time, you should use the 
// inetserversocket class (and possibly the unixserversocket class) in
// conjunction with the listener class.

class inetserversocket : public serversocket, public inetsocket {
	public:
				inetserversocket();

		int		listenOnSocket(const char *address,
						unsigned short port,
						int backlog);
				// Listen on "address" and "port" and allow
				// "backlog" connections to pile up before
				// refusing them.
				//
				// To listen on all addresses enter a NULL
				// for "address".  Entering a value of 0 for
				// the "port" causes the server to listen on an
				// arbitrary port.
				//
				// The getPort() method can be used later to 
				// discover this port.
				//
				// Returns 1 on success and 0 on failure.



		// The following 9 methods provide discrete control over socket
		// initialization.
		//
		// If you need to set socket options or do anything special
		// between the discrete steps of socket initialization, you
		// should use a combination of these methods.
		int	initialize(const char *address, unsigned short port);
			// Creates the actual socket and Initializes the class
			// to use "address" and "port" when bind() is called.
			//
			// Returns 1 on success and 0 on failure.
		int	bind();
			// Associates the socket with an address.
			//
			// Returns 1 on success and 0 on failure.

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


		// These 4 methods allow you to set socket options.  They must
		// be called after calling bind() and before calling listen().
		int		listen(int backlog);
				// Waits until a client connects then places
				// that connection in queue.  Up to "backlog"
				// connections may be queued before future
				// conenctions are refused.
				//
				// Returns 1 on success and 0 on failure.
		inetsocket	*acceptClientConnection();
				// Removes the client connection from the queue
				// and associates a new socket with that
				// connection.  Communication with the client
				// may be done over this new socket.  
				//
				// Returns an inetsocket on success and NULL
				// on failure.

		unsigned short	getPort();
				// Returns the inet port number that
				// the socket is listening on.  If the
				// port has not been set or if the
				// socket is a unix socket, 0 is 
				// returned instead.
		char		*getClientAddress();
				// Writes the address of the client at
				// the other end of "sock" to "buffer".
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/inetserversocketinlines.h>
#endif

#endif
