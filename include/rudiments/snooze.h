// Copyright (c) 2004-2015 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SNOOZE_H
#define RUDIMENTS_SNOOZE_H

#include <rudiments/private/snoozeincludes.h>

/** The snooze class provides static methods for suspending process
 *  execution. */
class RUDIMENTS_DLLSPEC snooze {
	public:

		/** Suspend execution for "seconds" seconds.
		 *  Returns true on success and false on failure.
		 *  If the snooze is interrupted, it resumes after the
		 *  interruption until the specified duration has elapsed. */
		static bool	macrosnooze(uint32_t seconds);

		/** Suspend execution for "seconds" seconds.
		 *  Returns true on success and false on failure, or if it was
		 *  interrupted (usually by a signal).
		 *  If the snooze is interrupted or failed, "secondsremaining"
		 *  is populated with the number of seconds that remained.
		 *  If "secondsremaining" may be NULL then it isn't populated,
		 *  but the snooze may still be interrupted. */
		static bool	macrosnooze(uint32_t seconds,
					uint32_t *secondsremaining);


		/** Suspend execution for "seconds" seconds and "microseconds"
		 *  microseconds.
		 *  Returns true on success and false on failure.
		 *  If the snooze is interrupted, it resumes after the
		 *  interruption until the specified duration has elapsed. */
		static bool	microsnooze(uint32_t seconds,
					uint32_t microseconds);

		/** Suspend execution for "seconds" seconds and "microseconds"
		 *  microseconds.
		 *  Returns true on success and false on failure, or if it was
		 *  interrupted (usually by a signal).
		 *  If the snooze is interrupted or failed, "secondsremaining"
		 *  and "microsecondsremaining" are populated with the number
		 *  of seconds and microseconds that remained.
		 *  If "secondsremaining" or "microsecondsremaining" is NULL
		 *  then it isn't populated, but the snooze may still be
		 *  interrupted. */
		static bool	microsnooze(uint32_t seconds,
					uint32_t microseconds,
					uint32_t *secondsremaining,
					uint32_t *microsecondsremaining);


		/** Suspend execution for "seconds" seconds and "nanoseconds"
		 *  nanoseconds.
		 *  Returns true on success and false on failure.
		 *  If the snooze is interrupted, it resumes after the
		 *  interruption until the specified duration has elapsed. */
		static bool	nanosnooze(uint32_t seconds,
					uint32_t nanoseconds);

		/** Suspend execution for "seconds" seconds and "nanoseconds"
		 *  nanoseconds.
		 *  Returns true on success and false on failure, or if it was
		 *  interrupted (usually by a signal).
		 *  If the snooze is interrupted or failed, "secondsremaining"
		 *  and "nanosecondsremaining" are populated with the number
		 *  of seconds and nanoseconds that remained.
		 *  If "secondsremaining" or "nanosecondsremaining" is NULL
		 *  then it isn't populated, but the snooze may still be
		 *  interrupted. */
		static bool	nanosnooze(uint32_t seconds,
					uint32_t nanoseconds,
					uint32_t *secondsremaining,
					uint32_t *nanosecondsremaining);

	#include <rudiments/private/snooze.h>
};

#endif
