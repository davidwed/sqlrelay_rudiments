// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMCLIENT_H
#define RUDIMENTS_MODEMCLIENT_H

#include <rudiments/private/modemclientincludes.h>

// The modemclient implements methods for connecting to and communicating with
// a remote host using a modem.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class modemclientprivate;

class RUDIMENTS_DLLSPEC modemclient : public client, private modemutil {
	public:
				modemclient();
				modemclient(const modemclient &m);
		modemclient	&operator=(const modemclient &m);
		virtual		~modemclient();

		void	initialize(constnamevaluepairs *cd);
			// Initializes the class to use "device",
			// "baud, "connectscript", "phonenumber",
			// "disconnectscript", "retrywait" and
			// "retrycount" when connect() is called.
		void	initialize(const char *device,
					const char *baud,
					const char *connectscript,
					const char *phonenumber,
					const char *disconnectscript,
					unsigned long retrywait,
					unsigned long retrycount);
			// Initializes the class to use "device",
			// "baud, "connectscript", "phonenumber",
			// "disconnectscript", "retrywait" and
			// "retrycount" when connect() is called.

		int32_t	connect();
			// Dial the modem and attempt to establish a connection.
			// If the connection fails, it will retry "retrycount"
			// times, waiting "retrywait" seconds between
			// retrycount.
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

		bool	close();
			// Hangs up the modem and closes the device.
			//
			// Returns true on success and false on failure.

	#include <rudiments/private/modemclient.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
