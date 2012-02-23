// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SNOOZE_H
#define RUDIMENTS_SNOOZE_H

#include <rudiments/private/snoozeincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

/** The snooze class provides methods for suspending process execution. */
class RUDIMENTS_DLLSPEC snooze {
	public:

		/** Suspend execution for "seconds" seconds.
		 *  Returns true on success and false on failure.
		 *  snooze methods most commonly fail if they are interrupted
		 *  by a signal. */
		static bool	macrosnooze(long seconds);

		/** Suspend execution for "seconds" seconds.
		 *  Returns true on success and false on failure.
		 *  snooze methods most commonly fail if they are interrupted
		 *  by a signal.  "remainingseconds" is populated with the
		 *  number of seconds that remained when the method was
		 *  interrupted. */
		static bool	macrosnooze(long seconds,
					long *remainingseconds);


		/** Suspend execution for "seconds" seconds and "microseconds"
		 *  microseconds.
		 *  Returns true on success and false on failure.
		 *  snooze methods most commonly fail if they are interrupted
		 *  by a signal. */
		static bool	microsnooze(long seconds,
					long microseconds);

		/** Suspend execution for "seconds" seconds and "microseconds"
		 *  microseconds.
		 *  Returns true on success and false on failure.
		 *  snooze methods most commonly fail if they are interrupted
		 *  by a signal.  "remainingseconds" and "remainingmicroseconds"
		 *  are populated with the number of seconds and microseconds
		 *  that remained when the method was interrupted. */
		static bool	microsnooze(long seconds,
					long microseconds,
					long *secondsremaining,
					long *microsecondsremaining);


		/** Suspend execution for "seconds" seconds and "nanoseconds"
		 *  nanoseconds.  Note that some systems do not have true
		 *  nanosecond clock resolution.
		 *  Returns true on success and false on failure.
		 *  snooze methods most commonly fail if they are interrupted
		 *  by a signal. */
		static bool	nanosnooze(long seconds,
					long nanoseconds);

		/** Suspend execution for "seconds" seconds and "nanoseconds"
		 *  nanoseconds.  Note that some systems do not have true
		 *  nanosecond clock resolution.
		 *  Returns true on success and false on failure.
		 *  snooze methods most commonly fail if they are interrupted
		 *  by a signal.  "remainingseconds" and "remainingnanoseconds"
		 *  are populated with the number of seconds and nanoseconds
		 *  that remained when the method was interrupted. */
		static bool	nanosnooze(long seconds,
					long nanoseconds,
					long *secondsremaining,
					long *nanosecondsremaining);
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
