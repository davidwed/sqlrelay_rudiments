// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKETCLIENT_H
#define RUDIMENTS_INETSOCKETCLIENT_H

#include <rudiments/private/inetsocketclientincludes.h>

class inetsocketclientprivate;

/** The inetsocketclient class allows you to write programs that can talk to
 *  other programs across a network over TCP stream sockets.
 * 
 *  The inetsocketclient class provides methods for connecting to servers.
 *  Its ultimate parent class: filedescriptor provides methods for reading and 
 *  writing data and closing connections. */
class RUDIMENTS_DLLSPEC inetsocketclient : public socketclient, private inetsocketutil  {
	public:

		/** Creates an instance of the inetsocketclient class. */
		inetsocketclient();

		/** Creates an instance of the inetsocketclient class
		 *  that is a copy of "i". */
		inetsocketclient(const inetsocketclient &i);

		/** Makes this instance of the inetsocketclient class
		 *  identical to "i". */
		inetsocketclient	&operator=(const inetsocketclient &i);

		/** Deletes this instance of the inetsocketclient class. */
		virtual		~inetsocketclient();

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
					int32_t timeoutsec,
					int32_t timeoutusec,
					uint32_t retrywait,
					uint32_t retrycount);



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
						int32_t timeoutsec,
						int32_t timeoutusec,
						uint32_t retrywait,
						uint32_t retrycount);

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
		 *  and protocolentry.h for more detail.
		 *
		 *  Note also that if your system supports getaddrinfo()
		 *  then round-robin DNS may not work as expected.  Some
		 *  systems support the sortv4=no directive in /etc/gai.conf
		 *  to work around this, but many do not.  Another workaround
		 *  is to set the RUDIMENTS_DISABLE_GETADDRINFO environment
		 *  variable to "yes".  This will cause connect() to use the
		 *  hostentry and protocolentry classes instead of
		 *  getaddrinfo() and should make round-robin DNS work
		 *  as expected. */
		int32_t	connect();

	#include <rudiments/private/inetsocketclient.h>
};

#endif
