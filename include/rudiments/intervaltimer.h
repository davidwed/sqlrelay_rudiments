// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INTERVALTIMER_H
#define RUDIMENTS_INTERVALTIMER_H

#include <rudiments/private/intervaltimerincludes.h>

// The intervaltimer class provides methods for interfacing with
// interval timers.

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


		void	setInitialInterval(long seconds, long microseconds);
			// Set the timer's initial interval to "seconds"
			// seconds and "microseconds" microseconds.
		void	setInitialInterval(timeval *tv);
			// Set the timer's initial interval to seconds and
			// microseconds specified in "tv".

		void	setPeriodicInterval(long seconds, long microseconds);
			// Set the timer's periodic interval to "seconds"
			// seconds and "microseconds" microseconds.
		void	setPeriodicInterval(timeval *tv);
			// Set the timer's periodic interval to seconds and
			// microseconds specified in "tv".

		void	setIntervals(long seconds, long microseconds);
			// Set the timer's initial and periodic intervals both
			// to "seconds" seconds and "microseconds" microseconds.
		void	setIntervals(timeval *tv);
			// Set the timer's initial and periodic intervals both
			// to seconds and microseconds specified in "tv".
		void	setIntervals(itimerval *itv);
			// Set the timer's initial and periodic intervals as
			// specified by "itv".


		void	getInitialInterval(long *seconds, long *microseconds);
			// Set "seconds" and "microseconds" to the timer's
			// initial interval values.
		void	getInitialInterval(timeval *tv);
			// Set "tv" to the timer's initial interval values.

		void	getPeriodicInterval(long *seconds, long *microseconds);
			// Set "seconds" and "microseconds" to the timer's
			// periodic interval values.
		void	getPeriodicInterval(timeval *tv);
			// Set "tv" to the timer's periodic interval values.

		void	getIntervals(itimerval *itv);
			// Set "itv" to the timer's intial and periodic values.


		bool	start();
			// Start (or restart) the timer.  It will first run for
			// the initial interval, then raise a signal, then run
			// over and over for the periodic interval, raising the
			// signal at the end of each interval.
		bool	start(itimerval *itv);
			// Start (or restart) the timer.  It will first run for
			// the initial interval, then raise a signal, then run
			// over and over for the periodic interval, raising the
			// signal at the end of each interval.  "itv" will be
			// set to the intervals that the timer was using prior
			// to calling start().


		bool	getTimeRemaining(long *seconds, long *microseconds);
			// Sets "seconds" and "microseconds" to the time
			// remaining before the timer will raise the signal.
		bool	getTimeRemaining(timeval *tv);
			// Sets "tv" to the time remaining before the timer
			// will raise the signal.


		bool	cancelTimer();
			// If a timer was set to run periodically, this
			// method will cancel it.


		// These methods allow you to suspend execution of the process
		// for a specified amount of time.  They return true on success
		// and false on failure (such as if a signal interrupts them).
		static bool	sleep(long seconds);
				// Suspend execution for "seconds" seconds.
		static bool	sleep(long seconds, long *remainingseconds);
				// Suspend execution for "seconds" seconds.
				// If a signal interrupts the sleep,
				// "remainingseconds" is populated with the
				// number of seconds that were not slept.


		static bool	microsleep(long seconds, long microseconds);
				// Suspend execution for "seconds" seconds
				// and "microseconds" microseconds.
		static bool	microsleep(long seconds, long microseconds,
						long *secondsremaining,
						long *microsecondsremaining);
				// Suspend execution for "seconds" seconds
				// and "microseconds" microseconds.
				// If a signal interrupts the sleep,
				// "remainingseconds" is populated with the
				// number of seconds that were not slept and
				// "remainingmicroseconds" is populated with the
				// number of microseconds that were not slept.
		static bool	microsleep(timeval *timetosleep);
				// Suspend execution for the number of seconds
				// and microseconds specified in "timetosleep".
		static bool	microsleep(timeval *timetosleep,
						timeval *timeremaining);
				// Suspend execution for the number of seconds
				// and microseconds specified in "timetosleep".
				// If a signal interrupts the sleep,
				// "timeremaining" is populated with the number
				// of seconds and microseconds that were not
				// slept.


		// These methods allow you to sleep for very small amounts
		// of time.  Note that some systems do not have true
		// nanosecond clock resolution.
		static bool	nanosleep(long seconds, long nanoseconds);
				// Suspend execution for "seconds" seconds
				// and "nanoseconds" nanoseconds.
		static bool	nanosleep(long seconds, long nanoseconds,
						long *secondsremaining,
						long *nanosecondsremaining);
				// Suspend execution for "seconds" seconds
				// and "nanoseconds" nanoseconds.
				// If a signal interrupts the sleep,
				// "remainingseconds" is populated with the
				// number of seconds that were not slept and
				// "remainingnanoseconds" is populated with the
				// number of nanoseconds that were not slept.
		static bool	nanosleep(timespec *timetosleep);
				// Suspend execution for the number of seconds
				// and nanoseconds specified in "timetosleep".
		static bool	nanosleep(timespec *timetosleep,
						timespec *timeremaining);
				// Suspend execution for the number of seconds
				// and nanoseconds specified in "timetosleep".
				// If a signal interrupts the sleep,
				// "timeremaining" is populated with the number
				// of seconds and nanoseconds that were not
				// slept.

	#include <rudiments/private/intervaltimer.h>
};

#endif
