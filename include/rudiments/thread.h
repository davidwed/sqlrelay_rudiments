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

		/** Sets this thread's stack size to "stacksize".  Returns true
		 *  on success and false if an error occurred. */
		bool	setStackSize(size_t stacksize);

		/** Sets "stacksize" to this thread's stack size.  Returns true
		 *  on success and false if an error occurred. */
		bool	getStackSize(size_t *stacksize);

		/** Starts a new thread by running whatever function was set
		 *  by the setFunction() method and passing that function
		 *  whatever argument was set by the setArgument() method.
		 *  Returns true on success and false if an error occurred. */
		bool	spawn(void *(*function)(void *),
					void *arg, bool detached);

		/** Causes a thread to exit with the value stored in the
		 *  location pointed to by "status".  Unless the calling
		 *  thread is detached, if another thread is waiting on this
		 *  thread using wait(), then that thread's wait() method will
		 *  fall through.  */
		void	exit(int32_t *status);

		/** Waits for the spawn()ed function to call exit() or return.
		 *  This method should be called by a second thread.
		 *  If non-NULL, "status" will be set to the exit status of the
		 *  thread.  Returns true on success and false if an error
		 *  occurred. */
		bool	wait(int32_t *status);

		/** Detaches the thread so that it may exit without calling
		 *  exit() and without another thread needing to call wait().
		 *  Returns true on success and false if an error occurred. */
		bool	detach();

		/** Sends signal "signum" to the thread. */
		bool	raiseSignal(int32_t signum);

		/** Returns true if the platform supports threads and rudiments
		 *  was built with thread support and false otherwise. */
		static bool	supportsThreads();

		/** Causes spawn() calls to be automatically retried if they
		 *  fail because of insufficient system resources.  This
		 *  is the default behavior.  Otherwise, if a spawn() fails,
		 *  the system error is set to EAGAIN and the spawn() must
		 *  be retried by the calling program. */
		void	retryFailedSpawn();

		/** Causes spawn() calls not to be automatically retried if
 		 *  they fail because of insufficient system resources.  If
 		 *  set, if a spawn() fails, the system error is set to EAGAIN
 		 *  and the spawn() must be retried by the calling program. */
		void	dontRetryFailedSpawn();

		/** Returns true if failed spawn() calls will be retried and
		 *  false otherwise. */
		bool	getRetryFailedSpawn();

	#include <rudiments/private/thread.h>
};

#endif
