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

		/** Defines the function that will be run when run() is
		 *  called. */
		void	setFunction(void *(*function)(void *));

		/** Defines the argument that will be passed to the function
		 *  when run() is called. */
		void	setArgument(void *arg);

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
		bool	run();

		/** Starts a new thread by running whatever function was set
		 *  by the setFunction() method, passing that function argument
		 *  "arg".  Returns true on success and false if an error
		 *  occurred. */
		bool	run(void *arg);

		/** Causes a thread to exit with the value stored in the
		 *  location pointed to by "status".  If another thread is
		 *  waiting on this thread using join(), then that thread's
		 *  join() method will fall through.  */
		void	exit(int32_t *status);

		/** Waits for the function set by setFunction() and run by
		 *  run() to call exit() or return.  This method should be
		 *  called by a second thread.  If non-NULL, "status" will be
		 *  set to the exit status of the thread.  Returns true on
		 *  success and false if an error occurred. */
		bool	join(int32_t *status);

		/** Detaches the thread so that it may exit without calling
		 *  exit() and without another thread needing to call join().
		 *  Returns true on success and false if an error occurred. */
		bool	detach();

		/** Cancels execution of the thread. */
		bool	cancel();

		/** Sends signal "signum" to the thread. */
		bool	raiseSignal(int32_t signum);

		/** Returns true if the platform supports threads and rudiments
		 *  was built with thread support and false otherwise. */
		static bool	supportsThreads();

		/** Causes run() calls to be automatically retried if they
		 *  fail because of insufficient system resources.  This
		 *  is the default behavior.  Otherwise, if a run() fails,
		 *  the system error is set to EAGAIN and the run() must
		 *  be retried by the calling program. */
		void	retryFailedRun();

		/** Causes run() calls not to be automatically retried if
 		 *  they fail because of insufficient system resources.  If
 		 *  set, if a run() fails, the system error is set to EAGAIN
 		 *  and the run() must be retried by the calling program. */
		void	dontRetryFailedRun();

		/** Returns true if failed run() calls will be retried and
		 *  false otherwise. */
		bool	getRetryFailedRun();

	#include <rudiments/private/thread.h>
};

#endif
