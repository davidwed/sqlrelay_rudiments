// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETCLIENTSOCKET_H
#define RUDIMENTS_INETCLIENTSOCKET_H

#include <rudiments/private/config.h>

#include <rudiments/client.h>
#include <rudiments/private/inetsocket.h>

// The inetclientsocket class allows you to write programs that can talk to
// other programs across a network over TCP stream sockets.
//
// The inetclientsocket class provides methods for connecting to servers.
// Its ultimate parent class: filedescriptor provides methods for reading and 
// writing data and closing connections.

class inetclientsocket : public client, public inetsocket  {
	public:

		int	connectToServer(const char *host,
					unsigned short port,
					unsigned int retrywait,
					int retrycount);
			// Convenience method that calls the
			// initialize() and connect()
			// methods below.
			//
			// Returns 1 on success and 0 on failure.



		// These methods are split up in case you need to use them
		// seperately.
		void	initialize(namevaluepairs *cd);
			// Queries namevaluepairs "cd" for "host", "port",
			// "retrywait" and "retrycount".  Initializes the class
			// to use the result when connect() is called.
		void	initialize(const char *host,
						unsigned short port,
						unsigned int retrywait,
						int retrycount);
			// Initializes the class to use "host", "port",
			// "retrywait" and "retrycount" when connect() is
			// called.
		int	connect();
			// Attempts to connect to the "host" and "port" set
			// earlier.  If the connection fails, it will retry
			// "retrycount" times, waiting "retrywait" seconds
			// between retrycount.  If "host" resolves to multiple
			// addresses, each address will be tried "retrycount"
			// times.
			//
			// Setting "retrycount" to 0 will cause it to try to 
			// connect indefinitely.  Setting "retrywait" to 0
			// will cause it to try to connect over and over
			// as fast as possible (not recommended).
			//
			// Returns 1 on success and 0 on failure.
};

#ifdef ENABLE_INLINES
	#define INLINE inline
	#include <rudiments/private/inetclientsocketinlines.h>
#endif

#endif
