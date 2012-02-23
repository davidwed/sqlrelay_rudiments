// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RPCENTRY_H
#define RUDIMENTS_RPCENTRY_H

#include <rudiments/private/rpcentryincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class rpcentryprivate;

/** The rpcentry class provides methods for retrieving entries from the rpc file
 *  (/etc/rpc on unix). */
class RUDIMENTS_DLLSPEC rpcentry {
	public:

		/** Sets "number" to the number associated with rpc "name".
		 *
		 *  Returns true on success and false otherwise. */
		static	bool	getNumber(const char *name, int32_t *number);

		/** Sets "aliaslist" to a NULL-terminated list of aliases for
		 *  rpc "name".  Note that "aliaslist" is allocated
		 *  internally and returned.  The calling program must free
		 *  each element and the array itself.
		 *  
		 *  Returns true on success and false otherwise. */
		static	bool	getAliasList(const char *name,
						char ***aliaslist);

		/** Sets "name" to the name associated with rpc "number". 
		 *  
		 *  Returns true on success and false otherwise. */
		static	bool	getName(int32_t number, char **name);

		/** Sets "aliaslist" to a NULL-terminated list of aliases for
		 *  rpc "number".  Note that "aliaslist" is allocated
		 *  internally and returned.  The calling program must free
		 *  each element and the array itself.
		 *  
		 *  Returns true on success and false otherwise. */
		static	bool	getAliasList(int32_t number, char ***aliaslist);

		/** Creates an instance of the rpcentry class. */
		rpcentry();

		/** Creates an instance of the rpcentry class
		 *  that is identical to "r". */
		rpcentry(const rpcentry &r);

		/** Makes this instance of the rpcentry class
		 *  identical ot "r". */
		rpcentry	&operator=(const rpcentry &r);

		/** Deletes this instance of the rpcentry class. */
		~rpcentry();

		/** Looks up a rpc entry by name.
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *name);

		/** Looks up a rpc entry by number.
		 *  Returns true on success and false on failure. */
		bool	initialize(int32_t number);

		/** Returns the name of the rpc entry. */
		const char		*getName() const;

		/** Returns the number of the rpc entry. */
		int32_t			getNumber() const;

		/** Returns a NULL-terminated list of aliases for the rpc
		 *  entry. */
		const char * const	*getAliasList() const;

		/** Prints out a representation of the rpc entry. */
		void	print() const;

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(mutex *mtx);

	#include <rudiments/private/rpcentry.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
