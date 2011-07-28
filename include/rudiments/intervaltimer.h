// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INTERVALTIMER_H
#define RUDIMENTS_INTERVALTIMER_H

#include <rudiments/private/intervaltimerincludes.h>

// The intervaltimer class provides methods for interfacing with
// interval timers.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class intervaltimerprivate;

class intervaltimer {
	public:
			intervaltimer(int which);
			// Initialize the interval timer.  "which" must be one
			// of:
			// ITIMER_REAL - decrements in real time, raises SIGALRM
			// ITIMER_VIRTUAL - decrements when the process is
			//			executing but not during system
			//			calls, raises SIGVTALRM
			// ITIMER_PROF - decrements when the process is 
			//			executing or and during system
			//			calls, raises SIGPROF

			~intervaltimer();

		void	initialize();
			// Sets (or resets) the initial and periodic intervals
			// to 0 seconds and 0 microseconds.

		void	setInitialInterval(long seconds, long microseconds);
			// Set the timer's initial interval to "seconds"
			// seconds and "microseconds" microseconds.

		void	setPeriodicInterval(long seconds, long microseconds);
			// Set the timer's periodic interval to "seconds"
			// seconds and "microseconds" microseconds.

		void	setIntervals(long seconds, long microseconds);
			// Set the timer's initial and periodic intervals both
			// to "seconds" seconds and "microseconds" microseconds.


		void	getInitialInterval(long *seconds,
						long *microseconds) const;
			// Set "seconds" and "microseconds" to the timer's
			// initial interval values.
		void	getPeriodicInterval(long *seconds,
						long *microseconds) const;
			// Set "seconds" and "microseconds" to the timer's
			// periodic interval values.

		bool	start() const;
			// Start (or restart) the timer.  It will first run for
			// the initial interval, then raise a signal, then run
			// over and over for the periodic interval, raising the
			// signal at the end of each interval.
		bool	start(long *seconds, long *microseconds) const;
			// Start (or restart) the timer.  It will first run for
			// the initial interval, then raise a signal, then run
			// over and over for the periodic interval, raising the
			// signal at the end of each interval.  "seconds" and
			// "microseconds" will be set to the interval that the
			// timer was using prior to calling start().


		bool	getTimeRemaining(long *seconds,
						long *microseconds) const;
			// Sets "seconds" and "microseconds" to the time
			// remaining before the timer will raise the signal.


		bool	stop();
			// Stops the currently running timer, but preserves
			// it's initial and perodic intervals.

	#include <rudiments/private/intervaltimer.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
