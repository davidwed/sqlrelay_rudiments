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

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class unixclientsocketprivate;

class unixclientsocket : public clientsocket, private unixsocketutil {
	public:

				unixclientsocket();
				unixclientsocket(const unixclientsocket &u);
		unixclientsocket	&operator=(const unixclientsocket &u);
		virtual		~unixclientsocket();

		int32_t	connect(const char *filename,
					long timeoutsec,
					long timeoutusec,
					unsigned long retrywait,
					unsigned long retrycount);
			// Convenience method that calls the initialize() and
			// connect() methods defined below.
			//
			// Returns RESULT_SUCCESS on success and RESULT_ERROR
			// on failure.



		// These methods are split up in case you need to use them
		// seperately.
		void	initialize(constnamevaluepairs *cd);
			// Queries namevaluepairs "cd" for "filename",
			// "timeoutsec", "timeoutusec", "retrywait" and
			// "retrycount".  Initializes the class to use the
			// result when connect() is called.
		void	initialize(const char *filename,
						long timeoutsec,
						long timeoutusec,
						unsigned long retrywait,
						unsigned long retrycount);
			// Initializes the class to use "filename",
			// "timeoutsec", "timeoutusec", "retrywait" and
			// "retrycount" when connect() is called.

		int32_t	connect();
			// Attempts to connect to the "filename" set earlier.
			// If the connection fails, it will retry "retrycount"
			// times, waiting "retrywait" seconds between tries.
			//
			// Setting "retrycount" to 0 will cause it to try to 
			// connect indefinitely.  Setting "retrywait" to 0
			// will cause it to try to connect over and over
			// as fast as possible (not recommended).
			//
			// Each attempt to connect will wait "timeoutsec"
			// seconds and "timeoutusec" microseconds for the
			// connect to succeed.  Specifying -1 for either
			// parameter will cause the attempt to wait until the
			// underlying protocol times out which may be up to 2
			// minutes.
			//
			// Returns RESULT_SUCCESS on success and RESULT_ERROR
			// on failure.

	#include <rudiments/private/unixclientsocket.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
