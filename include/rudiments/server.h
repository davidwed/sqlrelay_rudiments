// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVER_H
#define RUDIMENTS_SERVER_H

#include <rudiments/private/serverincludes.h>

/** The server class provides a base class for classes that implement the server
 *  side of a client-server communication model. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serverprivate;

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

		/** This method must be implemented by a child class to
		 *  associated the server with an address. */
		virtual	bool	bind()=0;

		/** This method must be implemented by a child class to
		 *  wait for client connections and fall through when a
		 *  client connects. */
		virtual	bool	listen(int32_t backlog)=0;

		/** This method must be implemented by a child class to
		 *  accept the client connection and return a file descriptor
		 *  that can be used to communicate with the client. */
		virtual	filedescriptor	*accept()=0;

	#include <rudiments/private/server.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
