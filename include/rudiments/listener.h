// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTENER_H
#define RUDIMENTS_LISTENER_H

#include <rudiments/private/listenerincludes.h>

typedef linkedlist<filedescriptor *>		listenerlist;
typedef linkedlistnode<filedescriptor *>	listenerlistnode;

class listenerprivate;

/** The listener class listens for activity on a pool of filedescriptors
 *  or children of the filedescriptor class. */
class RUDIMENTS_DLLSPEC listener {
	public:
		/** Creates an instance of the listener class. */
		listener();

		/** Deletes this instance of the listener class. */
		virtual	~listener();

		/** Adds the specified file descriptor to
		 *  the pool that the listener is listening on. */
		void	addFileDescriptor(filedescriptor *fd);

		/** Causes the application to wait until a read()
		 *  will proceed without blocking or until "sec"
		 *  seconds and "usec" microseconds have elapsed.
		 * 
		 *  Entering -1 for either parameter causes the
		 *  method to wait indefinitely.  
		 * 
		 *  Entering 0 for both parameters causes the
		 *  method to fall through immediately, returning
		 *  RESULT_TIMEOUT unless a read() will
		 *  immediately proceed without blocking.
		 * 
		 *  Returns RESULT_ERROR on error, RESULT_TIMEOUT
		 *  on timeout and otherwise returns the number
		 *  of file descriptors that are ready to be
		 *  read from. */
		int32_t	waitForNonBlockingRead(long sec, long usec);

		/** Causes the application to wait until a
		 *  write() will proceed without blocking or
		 *  until "sec" seconds and "usec" microseconds
		 *  have elapsed.
		 * 
		 *  Entering -1 for either parameter causes the
		 *  method to wait indefinitely.  
		 * 
		 *  Entering 0 for both parameters causes the
		 *  method to fall through immediately, returning
		 *  RESULT_TIMEOUT unless a write() will
		 *  immediately proceed without blocking.
		 * 
		 *  Returns RESULT_ERROR on error, RESULT_TIMEOUT
		 *  on timeout and otherwise returns the number
		 *  of file descriptors that are ready to be 
		 *  written to. */
		int32_t	waitForNonBlockingWrite(long sec, long usec);

		/** Returns the list of file descriptors that
		 *  were ready for read after the last call to
		 *  waitForNonBlockingRead() or ready for
		 *  write after the last call to
		 *  waitForNonBlockingWrite(). */
		listenerlist	*getReadyList();

		/** Removes the specified file descriptor from the pool. */
		void	removeFileDescriptor(filedescriptor *fd);
			
		/** Removes all file descriptors from the pool. */
		void	removeAllFileDescriptors();


		/** Causes waits to automatically retry if interrupted by a
		 *  signal.  This is the default. */
		void	retryInterruptedWaits();

		/** Causes waits not to automatically retry if
		 *  interrupted by a signal.  The default is to retry
		 *  automatically. */
		void	dontRetryInterruptedWaits();

	#include <rudiments/private/listener.h>
};

#endif
