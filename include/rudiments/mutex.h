// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MUTEX_H
#define RUDIMENTS_MUTEX_H

#include <rudiments/private/mutexincludes.h>

// The mutex class provides an interface to pthreads mutual exclusion locks.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class mutexprivate;

class mutex {
	public:
			mutex();
			// Initialize a mutex.
			#ifdef HAVE_PTHREAD_MUTEX_T
			mutex(pthread_mutex_t *mut);
			#endif
			#ifdef HAVE_CREATE_MUTEX
			mutex(HANDLE mut);
			#endif
			// Attach already initialized "mut" to this instance.
			~mutex();
			// Destroy the mutex.

		bool	lock();
			// Lock the mutex.  Will wait until the lock succeds.
			// Returns true on success and false if an error occurs.
		bool	tryLock();
			// Try to lock the mutex.  Will return immediately
			// if the mutex could not be locked, returning false
			// and setting errno to EBUSY.
			// Returns true on success and false if an error occurs.
		bool	unlock();
			// Unlock the mutex.
			// Returns true on success and false if an error occurs.

		#ifdef HAVE_PTHREAD_MUTEX_T
		pthread_mutex_t	*getMutex();
		#endif
		#ifdef HAVE_CREATE_MUTEX
		HANDLE		getMutex();
		#endif
				// Returns the mutex used by
				// the class internally.

	#include <rudiments/private/mutex.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
