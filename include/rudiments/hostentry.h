// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HOSTENTRY_H
#define RUDIMENTS_HOSTENTRY_H

#include <rudiments/private/hostentryincludes.h>

// The hostentry class provides methods for retrieving entries from /etc/hosts

class hostentry {
	public:

		// If you need to quickly look up a specific field, use one of
		// these methods.
		//
		// These methods return true on success and false on failure.
		static	bool	getAliasList(const char *hostname,
						char ***aliaslist);
		static	bool	getAddressType(const char *hostname,
						int *addresstype);
		static	bool	getAddressLength(const char *hostname,
						int *addresslength);
		static	bool	getAddressList(const char *hostname,
						char ***addresslist);
		static	bool	getAddressString(const char *hostname,
						int index,
						char **addressstring);

		static	bool	getName(const char *address,
						int len, int type,
						char **name);
		static	bool	getAliasList(const char *address,
						int len, int type,
						char ***aliaslist);
		static	bool	getAddressList(const char *address,
						int len, int type,
						char ***addresslist);
		static	bool	getAddressString(const char *address,
						int len, int type,
						int index,
						char **addressstring);

		// If you need to look up a host entry and refer to multiple
		// fields, use these methods.
			hostentry();
			~hostentry();

		bool	initialize(const char *hostname);
			// Looks up a host entry by name.
		bool	initialize(const char *address, int len, int type);
			// Looks up a host entry by address.

		char	*getName() const;
		char	**getAliasList() const;
		int	getAddressType() const;
		int	getAddressLength() const;
		char	**getAddressList() const;
		char	*getAddressString(int index) const;

		void	print() const;
			// Prints out the host entry.

		static	bool	needsMutex();
			// If your system doesn't support gethostbyname_r()
			// and gethostbyaddr_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/hostentry.h>
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/hostentryinlines.h>
#endif

#endif
