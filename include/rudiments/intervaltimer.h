// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INTERVALTIMER_H
#define RUDIMENTS_INTERVALTIMER_H

#include <rudiments/private/intervaltimerincludes.h>

/** The intervaltimer class provides methods for interfacing with timers.
 *
 *  A timer has an initial interval and periodic intervals.  The timer will
 *  raise a signal after the initial interval and then again after each
 *  periodic interval. */

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class intervaltimerprivate;

class RUDIMENTS_DLLSPEC intervaltimer {
	public:

		/** Creates an instance of the intervaltimer class.
		 * 
		 *  "which" must be one of:
		 *  ITIMER_REAL - decrements in real time, raises SIGALRM
		 *  ITIMER_VIRTUAL - decrements when the process is
		 * 			executing but not during system
		 * 			calls, raises SIGVTALRM
		 *  ITIMER_PROF - decrements when the process is 
		 * 			executing or and during system
		 * 			calls, raises SIGPROF */
		intervaltimer(int32_t which);

		/** Deletes this instance of the intervaltimer class. */
		~intervaltimer();

		/** Sets (or resets) the initial and periodic intervals
		 *  to 0 seconds and 0 microseconds. */
		void	initialize();

		/** Sets the timer's initial interval to "seconds"
		 *  seconds and "microseconds" microseconds. */
		void	setInitialInterval(long seconds, long microseconds);

		/** Sets the timer's periodic interval to "seconds"
		 *  seconds and "microseconds" microseconds. */
		void	setPeriodicInterval(long seconds, long microseconds);

		/** Sets the timer's initial and periodic intervals both
		 *  to "seconds" seconds and "microseconds" microseconds. */
		void	setIntervals(long seconds, long microseconds);


		/** Sets "seconds" and "microseconds" to the timer's
		 *  initial interval values. */
		void	getInitialInterval(long *seconds,
						long *microseconds) const;

		/** Sets "seconds" and "microseconds" to the timer's
		 *  periodic interval values. */
		void	getPeriodicInterval(long *seconds,
						long *microseconds) const;

		/** Starts (or restarts) the timer.  It will first run for
		 *  the initial interval, then raise a signal, then run
		 *  over and over for the periodic interval, raising the
		 *  signal at the end of each interval. */
		bool	start() const;

		/** Starts (or restarts) the timer.  It will first run for
		 *  the initial interval, then raise a signal, then run
		 *  over and over for the periodic interval, raising the
		 *  signal at the end of each interval.  "seconds" and
		 *  "microseconds" will be set to the interval that the
		 *  timer was using prior to calling start(). */
		bool	start(long *seconds, long *microseconds) const;


		/** Sets "seconds" and "microseconds" to the time
		 *  remaining before the timer will raise the signal. */
		bool	getTimeRemaining(long *seconds,
						long *microseconds) const;


		/** Stops the currently running timer, but preserves
		 *  it's initial and perodic intervals. */
		bool	stop();

	#include <rudiments/private/intervaltimer.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
