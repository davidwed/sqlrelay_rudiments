// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_THREADMUTEX_H
#define RUDIMENTS_THREADMUTEX_H

#include <rudiments/private/threadmutexincludes.h>

class threadmutexprivate;

/** The threadmutex class provides mutual exclusion locks between separate
 *  threads of a single process. */
class RUDIMENTS_DLLSPEC threadmutex {
	public:

		/** Creates an instance of the threadmutex class. */
		threadmutex();

		/** Creates an instance of the threadmutex class that uses an
		 *  already-initialized, system-specific threadmutex structure.
		 *  On unix systems, this would likely be an instance of
		 *  struct pthread_mutex_t, though it could possibly be
		 *  something else. */
		threadmutex(void *mut);

		/** Deletes this instance of the threadmutex class. */
		~threadmutex();

		/** Locks the mutex.  Will wait until the lock succeds.
		 *  Returns true on success and false if an error occurs. */
		bool	lock();

		/** Attempts to lock the mutex.  Will return immediately
		 *  if the mutex could not be locked, returning false
		 *  and setting errno to EBUSY.
		 *  Returns true on success and false if an error occurs. */
		bool	tryLock();

		/** Unlocks the mutex.
		 *  Returns true on success and false if an error occurs. */
		bool	unlock();

		/** Returns the mutex structure used by the class internally.
		 *  On unix systems this would most likely be a
		 *  struct pthread_mutex_t *, though it could possibly be
		 *  something else. */
		void	*getInternalMutexStructure();

	#include <rudiments/private/threadmutex.h>
};

#endif
