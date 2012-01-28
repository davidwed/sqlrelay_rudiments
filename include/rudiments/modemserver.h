// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MODEMSERVER_H
#define RUDIMENTS_MODEMSERVER_H

#include <rudiments/private/modemserverincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class modemserverprivate;

class modemserver : public server, private modemutil {
	public:
				modemserver();
				modemserver(const modemserver &m);
		modemserver	&operator=(const modemserver &m);
		virtual		~modemserver();

		bool	listen(const char *device, const char *baud,
						const char *listenscript,
						const char *acceptscript,
						const char *disconnectscript);

		// If you need to set socket options or do anything special
		// between the discrete steps of socket initialization, you
		// should use a combination of these methods.
		void	initialize(const char *device, const char *baud,
						const char *listenscript,
						const char *acceptscript,
						const char *disconnectscript);
			// Returns true on success and false on failure.
		bool	bind();
			// Returns true on success and false on failure.
		bool	listen(int32_t backlog);
			// Returns true on success and false on failure.

		filedescriptor	*accept();
				// Returns an inetsocket on success and NULL
				// on failure.

		bool	close();
			// Hangs up the modem and closes the device.
			//
			// Returns true on success and false on failure.

	#include <rudiments/private/modemserver.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
