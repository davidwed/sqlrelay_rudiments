// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RPCENTRY_H
#define RUDIMENTS_RPCENTRY_H

#include <rudiments/private/rpcentryincludes.h>

// The rpcentry class provides methods for retrieving entries from /etc/rpc

class rpcentry {
	public:

		// if you need to quickly look up a specific field, use one of
		// these methods
		static	int	getNumber(const char *name, int *number);
		static	int	getAliasList(const char *name,
						char ***aliaslist);

		static	int	getName(int number, char **name);
		static	int	getAliasList(int number, char ***aliaslist);

		// if you need to look up a rpc entry and refer to multiple
		// fields, use these methods
			rpcentry();
			~rpcentry();

		int	initialize(const char *name);
			// looks up a rpc entry by name
		int	initialize(int number);
			// looks up a rpc entry by number

		char	*getName() const;
		int	getNumber() const;
		char	**getAliasList() const;

		void	print() const;
			// prints out the rpc entry

		static	int	needsMutex();
			// If your system doesn't support getrpcbyname_r()
			// and getrpcbynumber_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns 1 if this class needs a mutex to
			// operate safely in a threaded environment and 0
			// otherwise.
		static	void	setMutex(pthread_mutex_t *mutex);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/rpcentry.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/rpcentryinlines.h>
#endif

#endif
