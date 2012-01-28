// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RPCENTRY_H
#define RUDIMENTS_RPCENTRY_H

#include <rudiments/private/rpcentryincludes.h>

// The rpcentry class provides methods for retrieving entries from /etc/rpc

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class rpcentryprivate;

class rpcentry {
	public:

		// If you need to quickly look up a specific field, use one of
		// these methods.
		//
		// These methods return true on success and false on failure.
		static	bool	getNumber(const char *name, int32_t *number);
		static	bool	getAliasList(const char *name,
						char ***aliaslist);

		static	bool	getName(int32_t number, char **name);
		static	bool	getAliasList(int32_t number, char ***aliaslist);

		// If you need to look up a rpc entry and refer to multiple
		// fields, use these methods.
				rpcentry();
				rpcentry(const rpcentry &r);
		rpcentry	&operator=(const rpcentry &r);
				~rpcentry();

		bool	initialize(const char *name);
			// Looks up a rpc entry by name.
			// Returns true on success and false on failure.
		bool	initialize(int32_t number);
			// Looks up a rpc entry by number.
			// Returns true on success and false on failure.

		const char		*getName() const;
		int32_t			getNumber() const;
		const char * const	*getAliasList() const;

		void	print() const;
			// Prints out the rpc entry.

		static	bool	needsMutex();
			// If your system doesn't support getrpcbyname_r()
			// and getrpcbynumber_r() then this class needs a
			// mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.

	#include <rudiments/private/rpcentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
