// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RPCENTRY_H
#define RUDIMENTS_RPCENTRY_H

#include <rudiments/private/rpcentryincludes.h>

// The rpcentry class provides methods for retrieving entries from /etc/rpc

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class rpcentry {
	public:

		// If you need to quickly look up a specific field, use one of
		// these methods.
		//
		// These methods return true on success and false on failure.
		static	bool	getNumber(const char *name, int *number);
		static	bool	getAliasList(const char *name,
						char ***aliaslist);

		static	bool	getName(int number, char **name);
		static	bool	getAliasList(int number, char ***aliaslist);

		// If you need to look up a rpc entry and refer to multiple
		// fields, use these methods.
			rpcentry();
			~rpcentry();

		bool	initialize(const char *name);
			// Looks up a rpc entry by name.
			// Returns true on success and false on failure.
		bool	initialize(int number);
			// Looks up a rpc entry by number.
			// Returns true on success and false on failure.

		char	*getName() const;
		int	getNumber() const;
		char	**getAliasList() const;

		void	print() const;
			// Prints out the rpc entry.

#ifdef RUDIMENTS_HAS_THREADS
		static	bool	needsMutex();
			// If your system doesn't support getrpcbyname_r()
			// and getrpcbynumber_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.
#endif

	#include <rudiments/private/rpcentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
