// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVICEENTRY_H
#define RUDIMENTS_SERVICEENTRY_H

#include <rudiments/private/serviceentryincludes.h>

// The serviceentry class provides methods for retrieving
// entries from /etc/services

class serviceentry {
	public:

		// If you need to quickly look up a specific field, use one of
		// these methods.
		//
		// These methods return true on success and false on failure.
		static	bool	getAliasList(const char *servicename,
						const char *protocol,
						char ***aliaslist);
		static	bool	getPort(const char *servicename,
						const char *protocol,
						int *port);

		static	bool	getName(int port, const char *protocol,
						char **name);
		static	bool	getAliasList(int port, const char *protocol,
						char ***aliaslist);


		// If you need to look up a service entry and refer to multiple
		// fields, use these methods.
			serviceentry();
			~serviceentry();

		bool	initialize(const char *servicename,
					const char *protocol);
			// Looks up a service entry by name and protocol.
			// Returns true on success and false on failure.
		bool	initialize(int port,
					const char *protocol);
			// Looks up a service entry by port and protocol.
			// Returns true on success and false on failure.

		char	*getName() const;
		int	getPort() const;
		char	*getProtocol() const;
		char	**getAliasList() const;

		void	print() const;
			// Prints out the service entry.

		static	bool	needsMutex();
			// If your system doesn't support getservbyname_r()
			// and getservbyport_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/serviceentry.h>
};

#endif
