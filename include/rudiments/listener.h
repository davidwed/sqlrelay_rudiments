// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTENER_H
#define RUDIMENTS_LISTENER_H

#include <rudiments/list.h>

#include <sys/types.h>

typedef list<int>	listenerlist;
typedef listnode<int>	listenerlistnode;

// The listener class listens on a pool of file descriptors.

class listener {
	public:
			listener();
		virtual	~listener();

		virtual	void	addFileDescriptor(int fd);
                        // Adds the specified file descriptor to
			// the pool that the listener is listening on.

		virtual	int	waitForNonBlockingRead(long timeoutsec,
							long timeoutusec);
			// Causes the application to wait until a read() will
			// proceed without blocking or until "sec" seconds and
			// "usec" microseconds have elapsed.
			//
			// Entering -1 for either parameter causes the method
			// to wait indefinitely.  
			//
			// Entering 0 for both parameters causes the method to
			// fall through immediately unless a data is
			// immediately available.
			//
			// Returns the file descriptor that data was available
			// on or -1 on error.
		virtual	int	waitForNonBlockingWrite(long timeoutsec,
							long timeoutusec);
			// Causes the application to wait until a write() will
			// proceed without blocking or until "sec" seconds and
			// "usec" microseconds have elapsed.
			//
			// Entering -1 for either parameter causes the method
			// to wait indefinitely.  
			//
			// Entering 0 for both parameters causes the method to
			// fall through immediately unless a data is
			// immediately available.
			//
			// Returns the file descriptor that data was available
			// on or -1 on error.

		virtual	void	removeFileDescriptor(int fd);
                        // Removes the specified file descriptor from the pool.
			
		virtual	void	removeAllFileDescriptors();
			// Removes all file descriptors from the pool.


		// By default, if a wait is occurring and a signal interrupts
		// it, the read or write returns with errno set to EINTR and
		// must be retried.  These methods override that behavior.
		void	retryInterruptedWaits();
			// Causes waits to automatically retry if
			// interrupted by a signal.  This is the default.
		void	dontRetryInterruptedWaits();
			// Causes waits not to automatically retry if
			// interrupted by a signal.  This is the default.

	#include <rudiments/private/listener.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/listenerinlines.h>
#endif

#endif
