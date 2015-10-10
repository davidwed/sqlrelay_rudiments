// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTENER_H
#define RUDIMENTS_LISTENER_H

#include <rudiments/private/listenerincludes.h>

typedef singlylinkedlistnode<filedescriptor *>	listenerlistnode;
typedef singlylinkedlist<filedescriptor *>	listenerlist;

class listenerprivate;

/** The listener class listens for activity on a pool of filedescriptors
 *  or children of the filedescriptor class. */
class RUDIMENTS_DLLSPEC listener {
	public:
		/** Creates an instance of the listener class. */
		listener();

		/** Deletes this instance of the listener class. */
		virtual	~listener();

		/** Adds the specified file descriptor to the pool that the
		 *  listener is listening on.  When listen() is called, it will
		 *  listen for read or write operations on this file
		 *  descriptor. */
		void	addFileDescriptor(filedescriptor *fd);

		/** Adds the specified file descriptor to the pool that the
		 *  listener is listening on.  When listen() is called, it will
		 *  only listen for read operations on this file descriptor. */
		void	addReadFileDescriptor(filedescriptor *fd);

		/** Adds the specified file descriptor to the pool that the
		 *  listener is listening on.  When listen() is called, it will
		 *  only listen for write operations on this file descriptor. */
		void	addWriteFileDescriptor(filedescriptor *fd);

		/** Removes the specified file descriptor from the pool. */
		void	removeFileDescriptor(filedescriptor *fd);
			
		/** Removes all file descriptors from the pool. */
		void	removeAllFileDescriptors();


		/** Causes the application to wait until a read or write
		 *  operation will proceed without blocking, as appropriate
		 *  for each file descriptor added by previous calls.
		 * 
		 *  Returns RESULT_ERROR on error or otherwise returns the
		 *  number of file descriptors that are ready to be
		 *  read from or written to. */
		int32_t	listen();

		/** Causes the application to wait until a read or write
		 *  operation will proceed without blocking, as appropriate
		 *  for each file descriptor added by previous calls, or
		 *  until "sec" seconds and "usec" microseconds have elapsed.
		 * 
		 *  Entering -1 for either parameter causes the
		 *  method to wait indefinitely.  
		 * 
		 *  Entering 0 for both parameters causes the
		 *  method to fall through immediately, returning
		 *  RESULT_TIMEOUT unless a write operation will
		 *  immediately proceed without blocking.
		 * 
		 *  Returns RESULT_ERROR on error, RESULT_TIMEOUT
		 *  on timeout and otherwise returns the number
		 *  of file descriptors that are ready to be 
		 *  read from or written to. */
		int32_t	listen(int32_t sec, int32_t usec);


		/** Returns the list of file descriptors that were ready for
		 *  read operations after the last call to listen(). */
		listenerlist	*getReadReadyList();

		/** Returns the list of file descriptors that were ready for
		 *  write operations after the last call to listen(). */
		listenerlist	*getWriteReadyList();


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
