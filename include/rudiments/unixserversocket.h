// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSERVERSOCKET_H
#define RUDIMENTS_UNIXSERVERSOCKET_H

#include <rudiments/private/unixserversocketincludes.h>

// The unixserversocket class allows you to write programs that can talk to
// other programs on the same machine over TCP stream sockets.
//
// Inet sockets (see the inetserversocket class) can be used by clients and
// servers on the same machine as well, but Unix sockets generally perform
// better.
//
// The unixserversocket class provides methods for setting up sockets and
// accepting client connections.  Its ultimate parent class: transport,
// provides methods for reading and writing data and closing connections.
// An immediate parent class: unixsocket provides methods for passing file
// descriptors between connected instances of unixservertsocket's and
// unixclientsocket's.
//
// If you need to listen on more than 1 socket at a time, you should use the 
// unixserversocket class (and possibly the unixserversocket class) in
// conjunction with the listener class.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class unixserversocket : public serversocket, private unixsocketutil {
	public:
				unixserversocket();
				unixserversocket(const unixserversocket &u);
		unixserversocket	&operator=(const unixserversocket &u);
		virtual		~unixserversocket();

		bool	listen(const char *filename,
					mode_t mask,
					int backlog);
				// Listen on "filename" and allow "backlog"
				// connections to pile up before refusing them.
				// Set the permissions on "filename" using
				// umask "mask".
				//
				// Returns true on success and false on failure.



		// The following 9 methods provide discrete control over socket
		// initialization.
		//
		// If you need to set socket options or do anything special
		// between the discrete steps of socket initialization, you
		// should use a combination of these methods.
		bool	initialize(const char *filename, mode_t mask);
			// Creates the actual socket and initializes the class
			// to use "filename" when bind() is called.  The
			// permissions on "filename" will be set using umask
			// "mask".
			//
			// Returns true on success and false on failure.
		bool	bind();
			// Associates the socket with an address.
			//
			// Returns true on success and false on failure.
		bool	listen(int backlog);
			// Waits until a client connects then places
			// that connection in queue.  Up to "backlog"
			// connections may be queued before future
			// conenctions are refused.
			//
			// Returns true on success and false on failure.


		filedescriptor	*accept();
				// Removes the client connection from the queue
				// and associates a new socket with that
				// connection.  Communication with the client
				// may be done over this new socket.  
				//
				// Returns an inetsocket on success and NULL
				// on failure.

	#include <rudiments/private/unixserversocket.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
