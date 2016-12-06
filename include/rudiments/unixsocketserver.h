// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETSERVER_H
#define RUDIMENTS_UNIXSOCKETSERVER_H

#include <rudiments/private/unixsocketserverincludes.h>

class unixsocketserverprivate;

/** The unixsocketserver class allows you to write programs that can talk to
 *  other programs on the same machine over TCP stream sockets.
 * 
 *  Inet sockets (see the inetsocketserver class) can be used by clients and
 *  servers on the same machine as well, but Unix sockets generally perform
 *  better.
 *
 *  Some platforms (line Windows) don't support Unix sockets.  For those
 *  platforms, Unix sockets are simulated using Inet sockets on localhost
 *  by translating the socket filename to a port.  See
 *  unixsocketutil::filenameToPort() for details.
 * 
 *  The unixsocketserver class provides methods for setting up sockets and
 *  accepting client connections.  Its ultimate parent class: transport,
 *  provides methods for reading and writing data and closing connections.
 *  An immediate parent class: unixsocket provides methods for passing file
 *  descriptors between connected instances of unixservertsocket's and
 *  unixsocketclient's.
 * 
 *  If you need to listen on more than 1 socket at a time, you should use the 
 *  unixsocketserver class (and possibly the unixsocketserver class) in
 *  conjunction with the listener class. */
class RUDIMENTS_DLLSPEC unixsocketserver : public socketserver, private unixsocketutil {
	public:

		/** Creates an instance of the unixsocketserver class. */
		unixsocketserver();

		/** Creates an instance of the unixsocketserver class
		 *  that is a copy of "u". */
		unixsocketserver(const unixsocketserver &u);

		/** Makes this instance of the unixsocketserver class
		 *  identical to "u". */
		unixsocketserver	&operator=(const unixsocketserver &u);

		/** Deletes this instance of the unixsocketserver class. */
		virtual		~unixsocketserver();

		/** Convenience method that calls initialize(), bind()
		 *  and listen().  If you need to set socket options or
		 *  do anything else special between those discrete steps
		 *  then you should use the methods individually.
		 *
		 *  Listen on "filename" and allow "backlog"
		 *  connections to pile up before refusing them.
		 *  Set the permissions on "filename" using
		 *  umask "mask".
		 * 
		 *  Returns true on success and false on failure. */
		bool	listen(const char *filename,
					mode_t mask,
					int32_t backlog);



		/** Creates the actual socket and initializes the class
		 *  to use "filename" when bind() is called.  The
		 *  permissions on "filename" will be set using umask
		 *  "mask".
		 * 
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *filename, mode_t mask);

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


		/** Closes the socket.
		 *  Returns true on success and false on failure. */
		bool	close();

	#include <rudiments/private/unixsocketserver.h>
};

#endif
