// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKETSERVER_H
#define RUDIMENTS_INETSOCKETSERVER_H

#include <rudiments/private/inetsocketserverincludes.h>

class inetsocketserverprivate;

/** The inetsocketserver class allows you to write programs that can talk to
 *  other programs across a network over TCP stream sockets.
 * 
 *  The inetsocketserver class provides methods for setting up sockets and
 *  accepting client connections.  Its ultimate parent class, filedescriptor,
 *  provides methods for reading and writing data and closing connections.
 * 
 *  If you need to listen on more than one socket at a time or a combination
 *  of sockets and other file descriptors, then you should use the
 *  inetsocketserver class in conjunction with the listener class. */
class RUDIMENTS_DLLSPEC inetsocketserver : public socketserver, private inetsocketutil {
	public:

		/** Creates an instance of the inetsocketserver class. */
		inetsocketserver();

		/** Creates an instance of the inetsocketserver class
		 *  that is a copy of "i". */
		inetsocketserver(const inetsocketserver &i);

		/** Makes this instance of the inetsocketserver class
		 *  identical to "i". */
		inetsocketserver	&operator=(const inetsocketserver &i);

		/** Deletes this instance of the inetsocketserver class. */
		virtual		~inetsocketserver();

		/** Convenience method that calls initialize(),
		 *  reuseAddresses(), bind() and listen().  If you need to set
		 *  other socket options or do anything else special between
		 *  those discrete steps then you should use the methods
		 *  individually.
		 * 
		 *  Listens on "address" and "port" and allow
		 *  "backlog" connections to pile up before
		 *  refusing them.
		 * 
		 *  To listen on all addresses enter a NULL
		 *  for "address".  Entering a value of 0 for
		 *  the "port" causes the server to listen on an
		 *  arbitrary port.
		 * 
		 *  The getPort() method can be used later to 
		 *  discover this port.
		 * 
		 *  Returns true on success and false on failure. */
		bool	listen(const char *address,
					uint16_t port,
					int32_t backlog);



		/** Creates the actual socket and initializes the class
		 *  to use "address" and "port" when bind() is called.
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *address, uint16_t port);

		/** Associates the socket with an address.
		 * 
		 *  Returns true on success and false on failure. */
		bool	bind();

		/** Waits until a client connects then places that connection
		 *  in queue.  Up to "backlog" connections may be queued before
		 *  future conenctions are refused. 
		 *
		 *  Returns true on success and false on failure. */
		bool	listen(int32_t backlog);

		/** Removes the client connection from the queue and associates
		 *  a new socket with that connection.  Communication with the
		 *  client may be done over this new socket.  
		 * 
		 *  Returns an inetsocket on success and NULL on failure. */
		filedescriptor	*accept();

		/** Returns the inet port number that the socket is listening
		 *  on.  If the port has not been set, 0 is returned instead.
		 *  Writes the address of the client at the other end of "sock"
		 *  to "buffer". */
		uint16_t	getPort();

	#include <rudiments/private/inetsocketserver.h>
};

#endif
