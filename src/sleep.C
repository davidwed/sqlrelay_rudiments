// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/sleep.h>

#include <errno.h>
#if defined(HAVE_NANOSLEEP) || defined (HAVE_CLOCK_NANOSLEEP)
	#include <time.h>
#else
	#include <sys/types.h>
	#include <sys/select.h>
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

bool sleep::macrosleep(long seconds) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=seconds;
	nanotimetosleep.tv_nsec=0;
	return nanosleep(&nanotimetosleep);
}

bool sleep::macrosleep(long seconds, long *remainingseconds) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=seconds;
	nanotimetosleep.tv_nsec=0;
	timespec	nanotimeremaining;
	bool	retval=nanosleep(&nanotimetosleep);
	*remainingseconds=nanotimeremaining.tv_sec;
	return retval;
}

bool sleep::microsleep(long seconds, long microseconds) {
	timeval		timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_usec=microseconds;
	return microsleep(&timetosleep);
}

bool sleep::microsleep(long seconds, long microseconds,
		long *secondsremaining, long *microsecondsremaining) {
	timeval		timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_usec=microseconds;
	timeval		timeremaining;
	bool	retval=microsleep(&timetosleep,&timeremaining);
	if (secondsremaining) {
		*secondsremaining=timeremaining.tv_sec;
	}
	if (microsecondsremaining) {
		*microsecondsremaining=timeremaining.tv_usec;
	}
	return retval;
}

bool sleep::microsleep(timeval *timetosleep) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=timetosleep->tv_sec;
	nanotimetosleep.tv_nsec=timetosleep->tv_usec*1000;
	return nanosleep(&nanotimetosleep);
}

bool sleep::microsleep(timeval *timetosleep, timeval *timeremaining) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=timetosleep->tv_sec;
	nanotimetosleep.tv_nsec=timetosleep->tv_usec*1000;
	timespec	nanotimeremaining;
	bool	retval=nanosleep(&nanotimetosleep,&nanotimeremaining);
	timeremaining->tv_sec=nanotimeremaining.tv_sec;
	timeremaining->tv_usec=nanotimeremaining.tv_nsec/1000;
	return retval;
}

bool sleep::nanosleep(long seconds, long nanoseconds) {
	timespec	timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_nsec=nanoseconds;
	return nanosleep(&timetosleep);
}

bool sleep::nanosleep(long seconds, long nanoseconds,
				long *secondsremaining,
				long *nanosecondsremaining) {
	timespec	timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_nsec=nanoseconds;
	timespec	timeremaining;
	bool	retval=nanosleep(&timetosleep,&timeremaining);
	if (secondsremaining) {
		*secondsremaining=timeremaining.tv_sec;
	}
	if (nanosecondsremaining) {
		*nanosecondsremaining=timeremaining.tv_nsec;
	}
	return retval;
}

bool sleep::nanosleep(timespec *timetosleep) {

	timespec	sleeptime;
	sleeptime.tv_sec=timetosleep->tv_sec;
	sleeptime.tv_nsec=timetosleep->tv_nsec;
	timespec	remaining;
	for (;;) {
		if (nanosleep(&sleeptime,&remaining)) {
			return true;
		} else if (errno!=EINTR) {
			return false;
		}
		sleeptime.tv_sec=remaining.tv_sec;
		sleeptime.tv_nsec=remaining.tv_nsec;
	}
}

bool sleep::nanosleep(timespec *timetosleep, timespec *timeremaining) {

	#ifdef HAVE_NANOSLEEP
	return !::nanosleep(timetosleep,timeremaining);
	#elif HAVE_CLOCK_NANOSLEEP
	return !clock_nanosleep(CLOCK_REALTIME,TIME_ABSTIME,
					timetosleep,timeremaining);
	#else

		// use regular sleep command to handle the whole seconds
		if (timetosleep->tv_sec) {
			unsigned int	remainder=::sleep(timetosleep->tv_sec);
			if (remainder) {
				timeremaining->tv_sec=remainder;
				timeremaining->tv_nsec=timetosleep->tv_nsec;
				return false;
			}
		}

		// set timeremaining to 0
		timeremaining->tv_sec=0;
		timeremaining->tv_nsec=0;

		// Use select or pselect to wait for the remaining time.
		//
		// It's tempting just to use select/pselect to sleep the entire
		// interval.  But on many platforms, select() doesn't modify
		// it's time struct to indicate how much time was left when it
		// timed out.  And on the platforms that it does do that, if
		// an error occurs such as being interrupted by a signal, then
		// the values in the time struct are undefined anyway.
		//
		// So, when using select/pselect, we can't return the number of
		// nanoseconds that were left if a signal interrupts the call.
		// But, at least we can return the number of seconds.
		#ifdef HAVE_PSELECT
		timespec	ts;
		ts.tv_sec=0;
		ts.tv_nsec=timetosleep->tv_nsec;
		return (pselect(0,NULL,NULL,NULL,&ts,NULL)!=-1);
		#else
		timeval		tv;
		tv.tv_sec=0;
		tv.tv_usec=timetosleep->tv_nsec/1000;
		return (select(0,NULL,NULL,NULL,&tv)!=-1);
		#endif

	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
