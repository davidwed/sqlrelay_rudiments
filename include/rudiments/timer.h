// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TIMER_H
#define RUDIMENTS_TIMER_H

#include <rudiments/private/timerincludes.h>

// The timer class provides methods for interfacing with timers and
// suspending execution.

class timer {
	public:
			timer(int which);
		bool	setTimer(itimerval *values);
		bool	setTimer(itimerval *values, itimerval *oldvalues);
		bool	getTimer(itimerval *values);

		static bool	sleep(long seconds);
		static bool	sleep(long seconds, long *remainingseconds);

		static bool	microsleep(long seconds, long microseconds);
		static bool	microsleep(long seconds, long microseconds,
						long *secondsremaining,
						long *microsecondsremaining);
		static bool	microsleep(timeval *timetosleep);
		static bool	microsleep(timeval *timetosleep,
						timeval *timeremaining);

		static bool	nanosleep(long seconds, long nanoseconds);
		static bool	nanosleep(long seconds, long nanoseconds,
						long *secondsremaining,
						long *nanosecondsremaining);
		static bool	nanosleep(timespec *timetosleep);
		static bool	nanosleep(timespec *timetosleep,
						timespec *timeremaining);

	#include <rudiments/private/timer.h>
};

#endif
