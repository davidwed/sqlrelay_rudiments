// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef UNIXSERVERSOCKET_H
#define UNIXSERVERSOCKET_H

#include <rudiments/private/config.h>

#include <sys/types.h>

#include <rudiments/private/serversocket.h>
#include <rudiments/private/unixsocket.h>

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

class unixserversocket : public serversocket, public unixsocket {
	public:
				unixserversocket();

		int		listenOnSocket(const char *filename,
						mode_t mask,
						int backlog);
				// Listen on "filename" and allow "backlog"
				// connections to pile up before refusing them.
				// Set the permissions on "filename" using
				// umask "mask".
				//
				// Returns 1 on success and 0 on failure.



		// The following 9 methods provide discrete control over socket
		// initialization.
		//
		// If you need to set socket options or do anything special
		// between the discrete steps of socket initialization, you
		// should use a combination of these methods.
		int	initialize(const char *filename, mode_t mask);
			// Creates the actual socket and initializes the class
			// to use "filename" when bind() is called.  The
			// permissions on "filename" will be set using umask
			// "mask".
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

		int		listen(int backlog);
				// Waits until a client connects then places
				// that connection in queue.  Up to "backlog"
				// connections may be queued before future
				// conenctions are refused.
				//
				// Returns 1 on success and 0 on failure.
		unixsocket	*acceptClientConnection();
				// Removes the client connection from the queue
				// and associates a new socket with that
				// connection.  Communication with the client
				// may be done over this new socket.  
				//
				// Returns an inetsocket on success and NULL
				// on failure.



		// Many operating systems support passing an open file
		// descriptor between processes.  The receiving process can
		// then read or write data to or from that file descriptor.
		// These methods implement that functionality.
		int	passFileDescriptor(int descriptor);
		int	receiveFileDescriptor(int *descriptor);

	#include <rudiments/private/unixserversocket.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/unixserversocketinlines.h>
#endif

#endif
