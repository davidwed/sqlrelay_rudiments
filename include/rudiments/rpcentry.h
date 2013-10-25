// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RPCENTRY_H
#define RUDIMENTS_RPCENTRY_H

#include <rudiments/private/rpcentryincludes.h>

class rpcentryprivate;

/** The rpcentry class provides methods for retrieving entries from the rpc file
 *  (/etc/rpc on unix). */
class RUDIMENTS_DLLSPEC rpcentry {
	public:

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

		/** Convenience method.
		 *  Sets "number" to the number associated with rpc "name".
		 *
		 *  Returns -1 if an error occurred or if "name" is invalid.
		 *  */
		static	int32_t	getNumber(const char *name);

		/** Convenience method.
		 *  Sets "name" to the name associated with rpc "number". 
		 *
		 *  Note that the return value is allocated internally and
		 *  returned.  The calling program must free the buffer.
		 *  
		 *  Returns NULL if an error occurred or if "name" is invalid.
		 *  */
		static	char	*getName(int32_t number);

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/rpcentry.h>
};

#endif
