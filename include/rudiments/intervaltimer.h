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


		// These methods allow you set the interval timer.  They return
		// true on success and false on failure.
		bool	setTimer(long seconds);
			// Set the timer to expire in "seconds" seconds
			// and 0 microseconds.
		bool	setTimer(long seconds, long microseconds);
			// Set the timer to expire in "seconds" seconds
			// and "microseconds" microseconds.
		bool	setTimer(timeval *value);
			// Set the timer to expire in the number of seconds
			// and microseconds specified in value.
		bool	setTimer(itimerval *values);
			// Set the timer to expire in the number of seconds
			// and microseconds specified in values->it_value
			// and then again, periodically in the number of
			// seconds and microseconds specified in
			// values->it_interval.
		bool	setTimer(itimerval *values, itimerval *oldvalues);
			// Set the timer to expire in the number of seconds
			// and microseconds specified in values->it_value
			// and then again, periodically in the number of
			// seconds and microseconds specified in
			// values->it_interval.  Populate oldvalues with the
			// intervals that the timer was previously set to.

		bool	cancelTimer();
			// If a timer was set to expire periodically, this
			// method will cancel it.

		bool	getTimer(itimerval *values);
			// Populate values with the initial timeout and
			// interval that the timer are currently set to use.


		// These methods allow you get the time remaining on the timer.
		// They return true on success and false on failure.
		bool	getTimeRemaining(long *seconds);
		bool	getTimeRemaining(long *seconds, long *microseconds);
		bool	getTimeRemaining(timeval *value);

		// These methods allow you get the interval that the timer will
		// be reset to after expiration.  They return true on success
		// and false on failure.
		bool	getInterval(long *seconds);
		bool	getInterval(long *seconds, long *microseconds);
		bool	getInterval(timeval *value);




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
