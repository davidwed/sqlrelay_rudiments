// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTENER_H
#define RUDIMENTS_LISTENER_H

#include <rudiments/private/listenerincludes.h>

typedef linkedlist<int>		listenerlist;
typedef linkedlistnode<int>	listenerlistnode;

// The listener class listens on a pool of file descriptors.

class listener {
	public:
			listener();
		virtual	~listener();

		virtual	void	addFileDescriptor(int fd);
                        	// Adds the specified file descriptor to
				// the pool that the listener is listening on.

		virtual	int	waitForNonBlockingRead(long sec, long usec);
				// Causes the application to wait until a read()
				// will proceed without blocking or until "sec"
				// seconds and "usec" microseconds have elapsed.
				//
				// Entering -1 for either parameter causes the
				// method to wait indefinitely.  
				//
				// Entering 0 for both parameters causes the
				// method to fall through immediately, returning
				// RESULT_TIMEOUT unless a read() will
				// immediately proceed without blocking.
				//
				// Returns RESULT_ERROR on error, RESULT_TIMEOUT
				// on timeout and otherwise returns the number
				// of file descriptors that are ready to be
				// read from.
		virtual	int	waitForNonBlockingWrite(long sec, long usec);
				// Causes the application to wait until a
				// write() will proceed without blocking or
				// until "sec" seconds and "usec" microseconds
				// have elapsed.
				//
				// Entering -1 for either parameter causes the
				// method to wait indefinitely.  
				//
				// Entering 0 for both parameters causes the
				// method to fall through immediately, returning
				// RESULT_TIMEOUT unless a write() will
				// immediately proceed without blocking.
				//
				// Returns RESULT_ERROR on error, RESULT_TIMEOUT
				// on timeout and otherwise returns the number
				// of file descriptors that are ready to be 
				// written to.

		listenerlist	*getReadyList();
				// Returns the list of file descriptors that
				// were ready for read after the last call to
				// waitForNonBlockingRead() or ready for
				// write after the last call to
				// waitForNonBlockingWrite().

		virtual	void	removeFileDescriptor(int fd);
                        	// Removes the specified file descriptor from
				// the pool.
			
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

#endif
