// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSERVERSOCKET_H
#define RUDIMENTS_INETSERVERSOCKET_H

#include <rudiments/private/inetserversocketincludes.h>

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

		bool	listenOnSocket(const char *address,
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
				// Returns true on success and false on failure.



		// If you need to set socket options or do anything special
		// between the discrete steps of socket initialization, you
		// should use a combination of these methods.
		bool	initialize(const char *address, unsigned short port);
			// Creates the actual socket and Initializes the class
			// to use "address" and "port" when bind() is called.
			//
			// Returns true on success and false on failure.
		bool	bind();
			// Associates the socket with an address.
			//
			// Returns true on success and false on failure.

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

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/inetserversocketinlines.h>
#endif

#endif
