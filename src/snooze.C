// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>

#if defined(HAVE_NANOSLEEP) || defined (HAVE_CLOCK_NANOSLEEP)
	#include <time.h>
#else
	#include <sys/types.h>
	#ifdef HAVE_SYS_SELECT_H
		#include <sys/select.h>
	#endif
	#ifdef HAVE_UNISTD_H
		#include <unistd.h>
	#endif
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

bool snooze::macrosnooze(long seconds) {
	timespec	nanotimetosnooze;
	nanotimetosnooze.tv_sec=seconds;
	nanotimetosnooze.tv_nsec=0;
	return nanosnooze(&nanotimetosnooze);
}

bool snooze::macrosnooze(long seconds, long *remainingseconds) {
	timespec	nanotimetosnooze;
	nanotimetosnooze.tv_sec=seconds;
	nanotimetosnooze.tv_nsec=0;
	timespec	nanotimeremaining;
	bool	retval=nanosnooze(&nanotimetosnooze,&nanotimeremaining);
	*remainingseconds=nanotimeremaining.tv_sec;
	return retval;
}

bool snooze::microsnooze(long seconds, long microseconds) {
	timeval		timetosnooze;
	timetosnooze.tv_sec=seconds;
	timetosnooze.tv_usec=microseconds;
	return microsnooze(&timetosnooze);
}

bool snooze::microsnooze(long seconds, long microseconds,
		long *secondsremaining, long *microsecondsremaining) {
	timeval		timetosnooze;
	timetosnooze.tv_sec=seconds;
	timetosnooze.tv_usec=microseconds;
	timeval		timeremaining;
	bool	retval=microsnooze(&timetosnooze,&timeremaining);
	if (secondsremaining) {
		*secondsremaining=timeremaining.tv_sec;
	}
	if (microsecondsremaining) {
		*microsecondsremaining=timeremaining.tv_usec;
	}
	return retval;
}

bool snooze::microsnooze(timeval *timetosnooze) {
	timespec	nanotimetosnooze;
	nanotimetosnooze.tv_sec=timetosnooze->tv_sec;
	nanotimetosnooze.tv_nsec=timetosnooze->tv_usec*1000;
	return nanosnooze(&nanotimetosnooze);
}

bool snooze::microsnooze(timeval *timetosnooze, timeval *timeremaining) {
	timespec	nanotimetosnooze;
	nanotimetosnooze.tv_sec=timetosnooze->tv_sec;
	nanotimetosnooze.tv_nsec=timetosnooze->tv_usec*1000;
	timespec	nanotimeremaining;
	bool	retval=nanosnooze(&nanotimetosnooze,&nanotimeremaining);
	timeremaining->tv_sec=nanotimeremaining.tv_sec;
	timeremaining->tv_usec=nanotimeremaining.tv_nsec/1000;
	return retval;
}

bool snooze::nanosnooze(long seconds, long nanoseconds) {
	timespec	timetosnooze;
	timetosnooze.tv_sec=seconds;
	timetosnooze.tv_nsec=nanoseconds;
	return nanosnooze(&timetosnooze);
}

bool snooze::nanosnooze(long seconds, long nanoseconds,
				long *secondsremaining,
				long *nanosecondsremaining) {
	timespec	timetosnooze;
	timetosnooze.tv_sec=seconds;
	timetosnooze.tv_nsec=nanoseconds;
	timespec	timeremaining;
	bool	retval=nanosnooze(&timetosnooze,&timeremaining);
	if (secondsremaining) {
		*secondsremaining=timeremaining.tv_sec;
	}
	if (nanosecondsremaining) {
		*nanosecondsremaining=timeremaining.tv_nsec;
	}
	return retval;
}

bool snooze::nanosnooze(timespec *timetosnooze) {

	timespec	snoozetime;
	snoozetime.tv_sec=timetosnooze->tv_sec;
	snoozetime.tv_nsec=timetosnooze->tv_nsec;
	timespec	remaining;
	for (;;) {
		if (nanosnooze(&snoozetime,&remaining)) {
			return true;
		} else if (error::getErrorNumber()!=EINTR) {
			return false;
		}
		snoozetime.tv_sec=remaining.tv_sec;
		snoozetime.tv_nsec=remaining.tv_nsec;
	}
}

bool snooze::nanosnooze(timespec *timetosnooze, timespec *timeremaining) {

	#ifdef HAVE_NANOSLEEP
	return !::nanosleep(timetosnooze,timeremaining);
	#elif HAVE_CLOCK_NANOSLEEP
	return !clock_nanosleep(CLOCK_REALTIME,TIME_ABSTIME,
					timetosnooze,timeremaining);
	#else

		// use regular sleep command to handle the whole seconds
		if (timetosnooze->tv_sec) {
			unsigned int	remainder=::sleep(timetosnooze->tv_sec);
			if (remainder) {
				timeremaining->tv_sec=remainder;
				timeremaining->tv_nsec=timetosnooze->tv_nsec;
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
		ts.tv_nsec=timetosnooze->tv_nsec;
		return (pselect(0,NULL,NULL,NULL,&ts,NULL)!=-1);
		#else
		timeval		tv;
		tv.tv_sec=0;
		tv.tv_usec=timetosnooze->tv_nsec/1000;
		return (select(0,NULL,NULL,NULL,&tv)!=-1);
		#endif

	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
