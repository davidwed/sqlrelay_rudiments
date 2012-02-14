// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SNOOZE_H
#define RUDIMENTS_SNOOZE_H

#include <rudiments/private/snoozeincludes.h>

// The snooze class provides methods for suspending process execution.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class RUDIMENTS_DLLSPEC snooze {
	public:

		// These methods allow you to suspend execution of the process
		// for a specified amount of time.  They return true on success
		// and false on failure (such as if a signal interrupts them).
		static bool	macrosnooze(long seconds);
				// Suspend execution for "seconds" seconds.
		static bool	macrosnooze(long seconds,
					long *remainingseconds);
				// Suspend execution for "seconds" seconds.
				// If a signal interrupts the snooze,
				// "remainingseconds" is populated with the
				// number of seconds that were not slept.


		static bool	microsnooze(long seconds,
					long microseconds);
				// Suspend execution for "seconds" seconds
				// and "microseconds" microseconds.
		static bool	microsnooze(long seconds,
					long microseconds,
					long *secondsremaining,
					long *microsecondsremaining);
				// Suspend execution for "seconds" seconds
				// and "microseconds" microseconds.
				// If a signal interrupts the snooze,
				// "remainingseconds" is populated with the
				// number of seconds that were not slept and
				// "remainingmicroseconds" is populated with the
				// number of microseconds that were not slept.


		// These methods allow you to snooze for very small amounts
		// of time.  Note that some systems do not have true
		// nanosecond clock resolution.
		static bool	nanosnooze(long seconds,
					long nanoseconds);
				// Suspend execution for "seconds" seconds
				// and "nanoseconds" nanoseconds.
		static bool	nanosnooze(long seconds,
					long nanoseconds,
					long *secondsremaining,
					long *nanosecondsremaining);
				// Suspend execution for "seconds" seconds
				// and "nanoseconds" nanoseconds.
				// If a signal interrupts the snooze,
				// "remainingseconds" is populated with the
				// number of seconds that were not slept and
				// "remainingnanoseconds" is populated with the
				// number of nanoseconds that were not slept.
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
