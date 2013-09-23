// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETCLIENT_H
#define RUDIMENTS_UNIXSOCKETCLIENT_H

#include <rudiments/private/unixsocketclientincludes.h>

class unixsocketclientprivate;

/** The unixsocketclient class allows you to write programs that can talk to
 *  other programs on the same machine over TCP stream sockets.
 * 
 *  Inet sockets (see the inetsocketclient class) can be used by clients and
 *  servers on the same machine as well, but Unix sockets generally perform
 *  better.
 * 
 *  The unixsocketclient class provides methods for connecting to servers.
 *  Its ultimate parent class: filedescriptor provides methods for reading and 
 *  writing data and closing connections. */
class RUDIMENTS_DLLSPEC unixsocketclient :
			public socketclient, private unixsocketutil {
	public:

		/** Creates an instance of the unixsocketclient class. */
		unixsocketclient();

		/** Creates an instance of the unixsocketclient class.
 		 *  that is a copy of "u". */
		unixsocketclient(const unixsocketclient &u);

		/** Makes this instance of the unixsocketclient class
		 *  identical to "u". */
		unixsocketclient	&operator=(const unixsocketclient &u);

		/** Deletes this instance of unixsocketclient. */
		virtual		~unixsocketclient();

		// This convenience method that calls the initialize() and
		// connect() methods of this class.
		//
		// Returns RESULT_SUCCESS on success and RESULT_ERROR
		// on failure.
		int32_t	connect(const char *filename,
					int32_t timeoutsec,
					int32_t timeoutusec,
					uint32_t retrywait,
					uint32_t retrycount);



		/** Queries namevaluepairs "cd" for "filename",
		 *  "timeoutsec", "timeoutusec", "retrywait" and
		 *  "retrycount".  Initializes the class to use the
		 *  result when connect() is called. */
		void	initialize(constnamevaluepairs *cd);

		/** Initializes the class to use "filename",
		 *  "timeoutsec", "timeoutusec", "retrywait" and
		 *  "retrycount" when connect() is called. */
		void	initialize(const char *filename,
						int32_t timeoutsec,
						int32_t timeoutusec,
						uint32_t retrywait,
						uint32_t retrycount);

		/** Attempts to connect to the "filename" set earlier.
		 *  If the connection fails, it will retry "retrycount"
		 *  times, waiting "retrywait" seconds between tries.
		 * 
		 *  Setting "retrycount" to 0 will cause it to try to 
		 *  connect indefinitely.  Setting "retrywait" to 0
		 *  will cause it to try to connect over and over
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
		 *  on failure. */
		int32_t	connect();

	#include <rudiments/private/unixsocketclient.h>
};

#endif
