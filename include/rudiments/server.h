// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVER_H
#define RUDIMENTS_SERVER_H

#include <rudiments/private/serverincludes.h>

class serverprivate;

/** The server class provides a base class for classes that implement the server
 *  side of a client-server communication model. */
class RUDIMENTS_DLLSPEC server : public filedescriptor {
	public:

		/** Creates an instance of the server class. */
		server();

		/** Creates an instance of the server class
		 *  that is a copy of "s". */
		server(const server &s);

		/** Makes this instance of the server class
		 *  identical to "s". */
		server	&operator=(const server &s);

		/** Deletes this instance of the server class. */
		virtual	~server();

		/** This method may be implemented by a child class to
		 *  associated the server with an address.
		 *
		 *  This implementation just returns true. */
		virtual	bool	bind();

		/** This method may be implemented by a child class to
		 *  wait for client connections and fall through when a
		 *  client connects.
		 *
		 *  This implementation just returns true. */
		virtual	bool	listen(int32_t backlog);

		/** This method may be implemented by a child class to
		 *  accept the client connection and return a file descriptor
		 *  that can be used to communicate with the client.
		 *
		 *  This implementation just creates a new instance of
		 *  filedescriptor and sets it to point to the same
		 *  file descriptor used by this instance. */
		virtual	filedescriptor	*accept();

	#include <rudiments/private/server.h>
};

#endif
