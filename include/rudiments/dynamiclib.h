// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DYNAMICLIB_H
#define RUDIMENTS_DYNAMICLIB_H

#include <rudiments/private/dynamiclibincludes.h>

// The dynamiclib class provides methods for loading, unloading and calling
// functions from dynamically linked libraries.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class dynamiclib {
	public:
			dynamiclib();
			~dynamiclib();

		bool	open(const char *library,
				bool loaddependencies,
				bool global);
			// Opens library "library".  If "loaddependencies"
			// is true, then all libraries required by this library
			// are also loaded, if it is false, they are loaded
			// later, as needed.  If "global" is true, then the
			// symbols defined in the library are made available
			// to libraries which are loaded later.
			//
			// Returns true on success and false on failure.
		bool	close();
			// Closes and unloads the previously opened library.
		void	*getSymbol(const char *symbol) const;
			// Returns a handle to "symbol" (exported function or
			// variable) in the currently open library or NULL if
			// an error occurs or if no library is currently open.
		char	*getError() const;
			// Returns a human-readable description of the previous
			// error that occurred, or NULL if no error has
			// occurred.
			//
			// This method allocates a buffer internally and
			// returns it.  The calling program must deallocate
			// the buffer.
		
#ifdef RUDIMENTS_HAS_THREADS
		// getError() is not reentrant and thus not thread safe.  If
		// your application is multi-threaded and you use getError(),
		// you must use this method to supply a mutex and ensure thread
		// safety.
		//
		// If you don't supply a mutex, getError() will still work, but
		// will not be thread-safe.
		static	void	setErrorMutex(pthread_mutex_t *mutex);
				// Allows you to supply a mutex to regulate
				// access to getError().
#endif

	#include <rudiments/private/dynamiclib.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
