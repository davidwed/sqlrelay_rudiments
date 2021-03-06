// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMCLIENT_H
#define RUDIMENTS_MODEMCLIENT_H

#include <rudiments/private/modemclientincludes.h>

class modemclientprivate;

/** The modemclient implements methods for connecting to and communicating with
 *  a remote host using a modem. */
class RUDIMENTS_DLLSPEC modemclient : public client, private modemutil {
	public:

		/** Creates an instance of the modemclient class. */
		modemclient();

		/** Creates an instance of the modemclient class
		 *  that is a copy of "m". */
		modemclient(const modemclient &m);

		/** Makes this instance of the modemclient class
		 *  identical to "m". */
		modemclient	&operator=(const modemclient &m);

		/** Deletes this instance of the modemclient class. */
		virtual		~modemclient();

		/** Initializes the class to use "device",
		 *  "baud, "connectscript", "phonenumber",
		 *  "disconnectscript", "retrywait" and
		 *  "tries" when connect() is called. */
		void	initialize(constnamevaluepairs *cd);

		/** Initializes the class to use "device",
		 *  "baud, "customatcommands",
		 *  "connectscript", "phonenumber",
		 *  "disconnectscript", "retrywait" and
		 *  "tries" when connect() is called. */
		void	initialize(const char *device,
					const char *baud,
					const char *customatcommands,
					const char *connectscript,
					const char *phonenumber,
					const char *disconnectscript,
					uint32_t retrywait,
					uint32_t tries);

		/** Dials the modem and attempt to establish a connection.
		 *  If the connection fails, it will retry, for a total of
		 *  "tries" attempts, waiting "retrywait" seconds between
		 *  each attempt.
		 * 
		 *  Setting "tries" to 0 will cause it to try to 
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

		/** Hangs up the modem and closes the device.
		 * 
		 *  Returns true on success and false on failure. */
		bool	close();

	#include <rudiments/private/modemclient.h>
};

#endif
