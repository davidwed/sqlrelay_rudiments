// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SLEEP_H
#define RUDIMENTS_SLEEP_H

#include <rudiments/private/sleepincludes.h>

// The sleep class provides methods for suspending process execution.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class sleep {
	public:

		// These methods allow you to suspend execution of the process
		// for a specified amount of time.  They return true on success
		// and false on failure (such as if a signal interrupts them).
		static bool	macrosleep(long seconds);
				// Suspend execution for "seconds" seconds.
		static bool	macrosleep(long seconds,
					long *remainingseconds);
				// Suspend execution for "seconds" seconds.
				// If a signal interrupts the sleep,
				// "remainingseconds" is populated with the
				// number of seconds that were not slept.


		static bool	microsleep(long seconds,
					long microseconds);
				// Suspend execution for "seconds" seconds
				// and "microseconds" microseconds.
		static bool	microsleep(long seconds,
					long microseconds,
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
		static bool	nanosleep(long seconds,
					long nanoseconds);
				// Suspend execution for "seconds" seconds
				// and "nanoseconds" nanoseconds.
		static bool	nanosleep(long seconds,
					long nanoseconds,
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
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
