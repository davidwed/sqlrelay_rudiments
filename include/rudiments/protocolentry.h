// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROTOCOLENTRY_H
#define RUDIMENTS_PROTOCOLENTRY_H

#include <rudiments/private/config.h>

#if !defined(HAVE_GETPROTOBYNAME_R) || !defined(HAVE_GETPROTOBYADDR_R)
	#include <pthread.h>
#endif

#include <netdb.h>

// The protocolentry class provides methods for retrieving
// entries from /etc/protocols

class protocolentry {
	public:

		// if you need to quickly look up a specific field, use one of
		// these methods
		static	int	getAliasList(const char *protocolname,
						char ***aliaslist);
		static	int	getNumber(const char *protocolname,
						int *number);

		static	int	getName(int number, char **name);
		static	int	getAliasList(int number, char ***aliaslist);


		// if you need to look up a protocol entry and refer to multiple
		// fields, use these methods
			protocolentry();
			~protocolentry();

		int	initialize(const char *protocolname);
			// looks up a protocol entry by name
		int	initialize(int number);
			// looks up a protocol entry by number

		char	*getName() const;
		char	**getAliasList() const;
		int	getNumber() const;

		void	print() const;
			// prints out the protocol entry

		static	int	needsMutex();
			// Returns 1 if this class needs a mutex to operate
			// safely in a threaded environment and 0 otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/protocolentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/protocolentryinlines.h>
#endif

#endif
