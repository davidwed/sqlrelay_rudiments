// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETCLIENTSOCKET_H
#define RUDIMENTS_INETCLIENTSOCKET_H

#include <rudiments/private/inetclientsocketincludes.h>

// The inetclientsocket class allows you to write programs that can talk to
// other programs across a network over TCP stream sockets.
//
// The inetclientsocket class provides methods for connecting to servers.
// Its ultimate parent class: filedescriptor provides methods for reading and 
// writing data and closing connections.

class inetclientsocket : public clientsocket, public inetsocketutil  {
	public:
			inetclientsocket();
		virtual	~inetclientsocket();

		int	connect(const char *host,
					unsigned short port,
					long timeoutsec,
					long timeoutusec,
					unsigned int retrywait,
					unsigned int retrycount);
			// Convenience method that calls the initialize() and
			// connect() methods defined below.
			//
			// Returns RESULT_SUCCESS on success and RESULT_ERROR
			// on failure.
			//
			// See connect() below for a note about use in
			// multithreaded applications.



		// These methods are split up in case you need to use them
		// seperately.
		void	initialize(namevaluepairs *cd);
			// Queries namevaluepairs "cd" for "host", "port",
			// "timeoutsec", "timeoutusec", "retrywait" and
			// "retrycount".  Initializes the class to use the
			// result when connect() is called.
		void	initialize(const char *host,
						unsigned short port,
						long timeoutsec,
						long timeoutusec,
						unsigned int retrywait,
						unsigned int retrycount);
			// Initializes the class to use "host", "port",
			// "timeoutsec", "timeoutusec", "retrywait" and
			// "retrycount" when connect() is called.

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
			// Each attempt to connect will wait "timeoutsec"
			// seconds and "timeoutusec" microseconds for the
			// connect to succeed.  Specifying -1 for either
			// parameter will cause the attempt to wait until the
			// underlying protocol times out which may be up to 2
			// minutes.
			//
			// Returns RESULT_SUCCESS on success and RESULT_ERROR
			// on failure.
			//
			// Note that if your system does not support
			// getaddrinfo() then connect() uses the hostentry and
			// protocolentry classes.  If you are using this method
			// in a multithreaded application, you may need to
			// supply those classes with mutexes.  See hostentry.h
			// and protocolentry.h for more detail.

	#include <rudiments/private/inetclientsocket.h>
};

#endif
