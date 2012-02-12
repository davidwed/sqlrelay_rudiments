// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SERVICEENTRY_H
#define RUDIMENTS_SERVICEENTRY_H

#include <rudiments/private/serviceentryincludes.h>

// The serviceentry class provides methods for retrieving
// entries from /etc/services

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class serviceentryprivate;

class DLLSPEC serviceentry {
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
						int32_t *port);

		static	bool	getName(int32_t port, const char *protocol,
						char **name);
		static	bool	getAliasList(int32_t port, const char *protocol,
						char ***aliaslist);


		// If you need to look up a service entry and refer to multiple
		// fields, use these methods.
				serviceentry();
				serviceentry(const serviceentry &s);
		serviceentry	&operator=(const serviceentry &s);
				~serviceentry();

		bool	initialize(const char *servicename,
					const char *protocol);
			// Looks up a service entry by name and protocol.
			// Returns true on success and false on failure.
		bool	initialize(int32_t port,
					const char *protocol);
			// Looks up a service entry by port and protocol.
			// Returns true on success and false on failure.

		const char		*getName() const;
		int32_t			getPort() const;
		const char		*getProtocol() const;
		const char * const *	getAliasList() const;

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
		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/serviceentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
