// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HOSTENTRY_H
#define RUDIMENTS_HOSTENTRY_H

#include <rudiments/private/config.h>

#if !defined(HAVE_GETHOSTBYNAME_R) || !defined(HAVE_GETHOSTBYADDR_R)
	#include <pthread.h>
#endif

#include <netdb.h>

// The hostentry class provides methods for retrieving entries from /etc/hosts

class hostentry {
	public:

		// if you need to quickly look up a specific field, use one of
		// these methods
		static	int	getAliasList(const char *hostname,
						char ***aliaslist);
		static	int	getAddressType(const char *hostname,
						int *addresstype);
		static	int	getAddressLength(const char *hostname,
						int *addresslength);
		static	int	getAddressList(const char *hostname,
						char ***addresslist);
		static	int	getAddressString(const char *hostname,
						int index,
						char **addressstring);

		static	int	getName(const char *address,
						int len, int type,
						char **name);
		static	int	getAliasList(const char *address,
						int len, int type,
						char ***aliaslist);
		static	int	getAddressList(const char *address,
						int len, int type,
						char ***addresslist);
		static	int	getAddressString(const char *address,
						int len, int type,
						int index,
						char **addressstring);

		// if you need to look up a host entry and refer to multiple
		// fields, use these methods
			hostentry();
			~hostentry();

		int	initialize(const char *hostname);
			// looks up a host entry by name
		int	initialize(const char *address, int len, int type);
			// looks up a host entry by address

		char	*getName() const;
		char	**getAliasList() const;
		int	getAddressType() const;
		int	getAddressLength() const;
		char	**getAddressList() const;
		char	*getAddressString(int index) const;

		void	print() const;
			// prints out the host entry

		static	int	needsMutex();
			// Returns 1 if this class needs a mutex to operate
			// safely in a threaded environment and 0 otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/hostentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/hostentryinlines.h>
#endif

#endif
