// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVICEENTRY_H
#define RUDIMENTS_SERVICEENTRY_H

#include <rudiments/private/serviceentryincludes.h>

// The serviceentry class provides methods for retrieving
// entries from /etc/services

class serviceentry {
	public:

		// if you need to quickly look up a specific field, use one of
		// these methods
		static	int	getAliasList(const char *servicename,
						const char *protocol,
						char ***aliaslist);
		static	int	getPort(const char *servicename,
						const char *protocol,
						int *port);

		static	int	getName(int port, const char *protocol,
						char **name);
		static	int	getAliasList(int port, const char *protocol,
						char ***aliaslist);


		// if you need to look up a service entry and refer to multiple
		// fields, use these methods
			serviceentry();
			~serviceentry();

		int	initialize(const char *servicename,
					const char *protocol);
			// looks up a service entry by name and protocol
		int	initialize(int port,
					const char *protocol);
			// looks up a service entry by port and protocol

		char	*getName() const;
		int	getPort() const;
		char	*getProtocol() const;
		char	**getAliasList() const;

		void	print() const;
			// prints out the service entry

		static	int	needsMutex();
			// If your system doesn't support getservbyname_r()
			// and getservbyport_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns 1 if this class needs a mutex to
			// operate safely in a threaded environment and 0
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/serviceentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/serviceentryinlines.h>
#endif

#endif
