// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMSERVER_H
#define RUDIMENTS_MODEMSERVER_H

#include <rudiments/private/modemserverincludes.h>

class modemserverprivate;

/** The modemserver class allows you to write programs taht can talk to
 *  other programs across a direct serial modem connection.
 *
 *  The modemserver class provides methods for setting up a modem and accepting
 *  client connections.  Its ultimate parent class, filedescriptor, provides
 *  methods for reading and writing data.
 *
 *  If you need to listen on more than one modem at a time or a combination of
 *  modems and other file descriptors (such as sockets), then you should use the
 *  modemserver class in conjunction with the listener class. */
class RUDIMENTS_DLLSPEC modemserver : public server, private modemutil {
	public:

		/** Creates an instance of the modemserver class. */
		modemserver();

		/** Creates an instance of the modemserver class
		 *  that is a copy of "m". */
		modemserver(const modemserver &m);

		/** Makes this instance of the modemserver class
		 *  identical to "m". */
		modemserver	&operator=(const modemserver &m);

		/** Deletes this instance of the modemserver class. */
		virtual		~modemserver();

		/** Convenience method that calls initialize(), bind(),
 		 *  and listen().  If you need to do anything special between
 		 *  those discrete steps, then you should use the methods
 		 *  individually.
 		 *
 		 *  Opens "device", sets the baud rate to "baud", then
 		 *  runs "listenscript".  When a client connects,
 		 *  "acceptscript" will be run.  When the client disconnects,
 		 *  "disconnecctscript" will be run.
 		 *
 		 *  Returns true on success and false on failure. */
		bool	listen(const char *device, const char *baud,
						const char *listenscript,
						const char *acceptscript,
						const char *disconnectscript);

		/** Initializes the class to opens "device", set the baud rate
		 *  to "baud", and run "listenscript" when listen() is called.
		 *  When a client connects, "acceptscript" will be run.  When
		 *  the client disconnects, "disconnecctscript" will be run.
 		 *
 		 *  Returns true on success and false on failure. */
		void	initialize(const char *device, const char *baud,
						const char *listenscript,
						const char *acceptscript,
						const char *disconnectscript);

		/** Does nothing in the default implementation but a child
		 *  class might implement it to do something.  This version
		 *  always returns true. */
		bool	bind();

		/** Opens the device set in the call to initalize(), sets the
 		 *  baud rate and runs "listenscript".
 		 *
 		 *  Returns true on success and false on failure. */
		bool	listen(int32_t backlog);

		/** Waits for a client connection.  When a client connects,
 		 *  it runs "acceptscript" which was set in the call to
 		 *  initialize().
 		 *
 		 *  Returns true on success and false on failure. */
		filedescriptor	*accept();

		/** Runs "disconnectscript" which was set in the call to
		 *  initialize() and closes the device.
		 * 
		 *  Returns true on success and false on failure. */
		bool	close();

	#include <rudiments/private/modemserver.h>
};

#endif
