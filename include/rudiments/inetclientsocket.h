// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETCLIENTSOCKET_H
#define RUDIMENTS_INETCLIENTSOCKET_H

#include <rudiments/private/inetclientsocketincludes.h>

/** The inetclientsocket class allows you to write programs that can talk to
 *  other programs across a network over TCP stream sockets.
 * 
 *  The inetclientsocket class provides methods for connecting to servers.
 *  Its ultimate parent class: filedescriptor provides methods for reading and 
 *  writing data and closing connections. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class inetclientsocketprivate;

class RUDIMENTS_DLLSPEC inetclientsocket : public clientsocket, private inetsocketutil  {
	public:

		/** Creates an instance of the inetclientsocket class. */
		inetclientsocket();

		/** Creates an instance of the inetclientsocket class
		 *  that is a copy of "i". */
		inetclientsocket(const inetclientsocket &i);

		/** Makes this instance of the inetclientsocket class
		 *  identical to "i". */
		inetclientsocket	&operator=(const inetclientsocket &i);

		/** Deletes this instance of the inetclientsocket class. */
		virtual		~inetclientsocket();

		/** This convenience method calls the initialize() and
		 *  connect() methods of the class.
		 * 
		 *  Returns RESULT_SUCCESS on success and RESULT_ERROR
		 *  on failure.
		 * 
		 *  See the other connect() method for a note about use in
		 *  multithreaded applications. */
		int32_t	connect(const char *host,
					uint16_t port,
					long timeoutsec,
					long timeoutusec,
					unsigned long retrywait,
					unsigned long retrycount);



		/** Queries constnamevaluepairs "cd" for "host", "port",
		 *  "timeoutsec", "timeoutusec", "retrywait" and
		 *  "retrycount".  Initializes the class to use the
		 *  result when connect() is called. */
		void	initialize(constnamevaluepairs *cd);

		/** Initializes the class to use "host", "port",
		 *  "timeoutsec", "timeoutusec", "retrywait" and
		 *  "retrycount" when connect() is called. */
		void	initialize(const char *host,
						uint16_t port,
						long timeoutsec,
						long timeoutusec,
						unsigned long retrywait,
						unsigned long retrycount);

		/** Attempts to connect to the "host" and "port" set
		 *  earlier using one of the initialize() methods.
		 *  If the connection fails, it will retry
		 *  "retrycount" times, waiting "retrywait" seconds
		 *  between retrycount.  If "host" resolves to multiple
		 *  addresses, each address will be tried "retrycount"
		 *  times.
		 * 
		 *  Setting "retrycount" to 0 will cause it to try to 
		 *  connect indefinitely.  Setting "retrywait" to
		 *  0 will cause it to try to connect over and over
		 *  as fast as possible (not recommended).
		 * 
		 *  Each attempt to connect will wait "timeoutsec"
		 *  seconds and "timeoutusec" microseconds for the
		 *  connect to succeed.  Specifying -1 for either
		 *  parameter will cause the attempt to wait until the
		 *  underlying protocol times out which may be up to 2
		 *  minutes.
		 * 
		 *  Returns RESULT_SUCCESS on success and RESULT_ERROR
		 *  on failure.
		 * 
		 *  Note that if your system does not support
		 *  getaddrinfo() then connect() uses the hostentry and
		 *  protocolentry classes.  If you are using this method
		 *  in a multithreaded application, you may need to
		 *  supply those classes with mutexes.  See hostentry.h
		 *  and protocolentry.h for more detail. */
		int32_t	connect();

	#include <rudiments/private/inetclientsocket.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
