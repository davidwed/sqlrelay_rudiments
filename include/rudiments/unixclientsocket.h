// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXCLIENTSOCKET_H
#define RUDIMENTS_UNIXCLIENTSOCKET_H

#include <rudiments/private/unixclientsocketincludes.h>

// The unixclientsocket class allows you to write programs that can talk to
// other programs on the same machine over TCP stream sockets.
//
// Inet sockets (see the inetclientsocket class) can be used by clients and
// servers on the same machine as well, but Unix sockets generally perform
// better.
//
// The inetclientsocket class provides methods for connecting to servers.
// Its ultimate parent class: filedescriptor provides methods for reading and 
// writing data and closing connections.

class unixclientsocket : public client, public unixsocket {
	public:

		int	connectToServer(const char *filename,
					unsigned int retrywait,
					unsigned int retrycount);
			// Convenience method that calls the
			// initialize() and connect()
			// methods below.
			//
			// Returns RESULT_SUCCESS on success and RESULT_ERROR
			// on failure.



		// These methods are split up in case you need to use them
		// seperately.
		void	initialize(namevaluepairs *cd);
			// Queries namevaluepairs "cd" for "filename",
			// "retrywait" and "retrycount".  Initializes the class
			// to use the result when connect() is called.
		void	initialize(const char *filename,
					unsigned int retrywait,
					unsigned int retrycount);
			// Initializes the class to use "filename",
			// "retrywait" and "retrycount" when connect() is
			// called.
		int	connect();
			// Attempts to connect to the "filename" set earlier.
			// If the connection fails, it will retry "retrycount"
			// times, waiting "retrywait" seconds between tries.
			//
			// Setting "retrycount" to 0 will cause it to try to 
			// connect indefinitely.  Setting "retrywait" to 0
			// will cause it to try to connect over and over
			// as fast as possible (not recommended).
			//
			// Returns RESULT_SUCCESS on success and RESULT_ERROR
			// on failure.
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/unixclientsocketinlines.h>
#endif

#endif
