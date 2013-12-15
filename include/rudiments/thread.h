// Copyright (c) 2013 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_THREAD_H
#define RUDIMENTS_THREAD_H

#include <rudiments/private/threadincludes.h>

class threadprivate;

/** The thread class provides a simple thread implementation. */
class RUDIMENTS_DLLSPEC thread {
	public:
		/** Creates an instance of the thread class. */
		thread();

		/** Deletes this instance of the thread class. */
		virtual	~thread();

		/** Defines the function that will be run when create() is
		 *  called. */
		void	setFunction(void *(*function)(void *), void *arg);

		/** Sets this thread's stack size to "stacksize".  Returns true
		 *  on success and false if an error occurred. */
		bool	setStackSize(size_t stacksize);

		/** Sets "stacksize" to this thread's stack size.  Returns true
		 *  on success and false if an error occurred. */
		bool	getStackSize(size_t *stacksize);

		/** Starts a new thread by running whatever function was set
		 *  by the setFunction() method.  Returns true on success
		 *  and false if an error occurred. */
		bool	create();

		/** Causes a thread to exit with the specified "status".  If
		 *  another thread is waiting on this thread using join(), then
		 *  that thread's join() method will fall through.  */
		void	exit(void *status);

		/** Waits for the function set by setFunction() and run by
		 *  create() to call exit() or return.  This method should be
		 *  called by a second thread.  If non-NULL, "status" will be
		 *  set to the exit status of the thread.  Returns true on
		 *  success and false if an error occurred. */
		bool	join(void **status);

		/** Detaches the thread so that it may exit without calling
		 *  exit() and without another thread needing to call join().
		 *  Returns true on success and false if an error occurred. */
		bool	detach();

	#include <rudiments/private/thread.h>
};

#endif
