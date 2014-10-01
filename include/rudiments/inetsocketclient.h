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
					uint32_t tries);



		/** Queries constnamevaluepairs "cd" for "host", "port",
		 *  "timeoutsec", "timeoutusec", "retrywait" and
		 *  "tries".  Initializes the class to use the
		 *  result when connect() is called. */
		void	initialize(constnamevaluepairs *cd);

		/** Initializes the class to use "host", "port",
		 *  "timeoutsec", "timeoutusec", "retrywait" and
		 *  "tries" when connect() is called. */
		void	initialize(const char *host,
						uint16_t port,
						int32_t timeoutsec,
						int32_t timeoutusec,
						uint32_t retrywait,
						uint32_t tries);

		/** Attempts to connect to the "host" and "port" set
		 *  earlier using one of the initialize() methods.
		 *  If the connection fails, it will retry, for a total of
		 *  "tries" attempts, waiting "retrywait" seconds
		 *  between each attempt.
		 *
		 *  If "host" resolves to multiple addresses (ie. if round-robin
		 *  DNS or another DNS-based load-balancing strategy is used),
		 *  then an attempt will be made to connect to each address
		 *  until the attempt succeeds or there are no more addresses
		 *  left to try.  This process will be tried "tries" times.
		 * 
		 *  Setting "tries" to 0 will cause it to try to 
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
		 *
		 *  Notes on round-robin DNS and DNS-based load-balancing:
		 *
		 *  By default, the connect() method randomizes the list of
		 *  addresses that "host" resolves to.  This is intended to
		 *  work around two issues.
		 *
		 *
		 *  Issue 1:
		 *  When round-robin DNS is used, if one host is down, then the
		 *  next host in the list receives all of the traffic that would
		 *  have gone to the downed host.  It is usually prefereable
		 *  for the traffic to be distributed among the remaining hosts
		 *  instead.  Randomization of the list of addresses
		 *  accomplishes this.
		 *
		 *
		 *  Issue 2:
		 *  Rudiments uses getaddrinfo() if it is available, but
		 *  implementations of getaddrinfo() vary widely.
		 *
		 *  Some implementations sort the list of addresses that "host"
		 *  resolves to rather than returning them in the order
		 *  returned by the DNS server, defeating any attempt at
		 *  DNS-based load-balancing.
		 *
		 *  Some implementations support a sortv4=no directive in
		 *  /etc/gai.conf that instructs getaddrinfo() not to sort the
		 *  addresses but many do not.
		 *
		 *  Some implementations randomize the list.
		 *
		 *  Some implementations return the list in the order returned
		 *  by DNS.  
		 *
		 *  Rudiments doesn't have a good way of knowing how
		 *  getaddrinfo() is implemented or configured, so to err on
		 *  the side of caution, connect() randomizes the list of
		 *  addresses that "host" resolves to.
		 *
		 *
		 *  Randomization can be disabled and configured using
		 *  dontRandomizeAddresses() and randomizeAddresses().
		 */
		int32_t	connect();

		/** Instructs connect() to randomize the list of addresses
 		 *  that "host" resolves to using "seed".
		 *  
		 *  Randomization of addresses is the default behavior, but by
		 *  default the random number generator used internally is
		 *  seeded using randomnumber::getSeed().  This may or may not
		 *  be suitable as it will fall back to seeding the generator
		 *  with the time of day on platforms that don't support any
		 *  other method.
		 *
		 *  This method allows you to provide a more suitable seed
		 *  if necessary. */
		void	randomizeAddresses(uint32_t seed);

		/** Instructs connect() not to randomize the list of addresses
 		 *  that "host" resolves to.
 		 *
 		 *  See connect() for issues that this can cause on some
 		 *  platforms or environments. */
		void	dontRandomizeAddresses();

	#include <rudiments/private/inetsocketclient.h>
};

#endif
