// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/timer.h>

#include <stdlib.h>
#include <errno.h>

#if defined(HAVE_NANOSLEEP) || defined (HAVE_CLOCK_NANOSLEEP)
	#include <time.h>
#else
	#include <unistd.h>
#endif

timer::timer(int which) {
	this->which=which;
}

bool timer::setTimer(itimerval *values) {
	itimerval	oldvalues;
	return !setitimer(which,values,&oldvalues);
}

bool timer::setTimer(itimerval *values, itimerval *oldvalues) {
	return !setitimer(which,values,oldvalues);
}

bool timer::getTimer(itimerval *values) {
	return !getitimer(which,values);
}

bool timer::sleep(long seconds) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=seconds;
	nanotimetosleep.tv_nsec=0;
	return nanosleep(&nanotimetosleep);
}

bool timer::sleep(long seconds, long *remainingseconds) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=seconds;
	nanotimetosleep.tv_nsec=0;
	timespec	nanotimeremaining;
	bool	retval=nanosleep(&nanotimetosleep);
	*remainingseconds=nanotimeremaining.tv_sec;
	return retval;
}

bool timer::microsleep(long seconds, long microseconds) {
	timeval		timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_usec=microseconds;
	return microsleep(&timetosleep);
}

bool timer::microsleep(long seconds, long microseconds,
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

bool timer::microsleep(timeval *timetosleep) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=timetosleep->tv_sec;
	nanotimetosleep.tv_nsec=timetosleep->tv_usec*1000;
	return nanosleep(&nanotimetosleep);
}

bool timer::microsleep(timeval *timetosleep, timeval *timeremaining) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=timetosleep->tv_sec;
	nanotimetosleep.tv_nsec=timetosleep->tv_usec*1000;
	timespec	nanotimeremaining;
	bool	retval=nanosleep(&nanotimetosleep,&nanotimeremaining);
	timeremaining->tv_sec=nanotimeremaining.tv_sec;
	timeremaining->tv_usec=nanotimeremaining.tv_nsec/1000;
	return retval;
}

bool timer::nanosleep(long seconds, long nanoseconds) {
	timespec	timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_nsec=nanoseconds;
	return nanosleep(&timetosleep);
}

bool timer::nanosleep(long seconds, long nanoseconds,
			long *secondsremaining, long *nanosecondsremaining) {
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

bool timer::nanosleep(timespec *timetosleep) {

	timespec	sleeptime;
	sleeptime.tv_sec=timetosleep->tv_sec;
	sleeptime.tv_nsec=timetosleep->tv_nsec;
	timespec	remaining;
	while (sleeptime.tv_sec>0 && sleeptime.tv_nsec>0) {
		if (!nanosleep(&sleeptime,&remaining) && errno!=EINTR) {
			return false;
		}
		sleeptime.tv_sec=remaining.tv_sec;
		sleeptime.tv_nsec=remaining.tv_nsec;
	}
	return true;
}

bool timer::nanosleep(timespec *timetosleep, timespec *timeremaining) {

	#ifdef HAVE_NANOSLEEP
	return !::nanosleep(timetosleep,timeremaining);
	#elif HAVE_CLOCK_NANOSLEEP
	return !clock_nanosleep(CLOCK_REALTIME,TIME_ABSTIME,
					timetosleep,timeremaining);
	#else

		// use regular sleeep command to handle the whole seconds
		if (timetosleep->tv_sec) {
			unsigned int	remainder=sleep(timetosleep->tv_sec);
			if (remainder) {
				timeremaining->tv_sec=remainder;
				timeremaining->tv_nsec=timetosleep->tv_nsec;
				return false;
			}
		}

		// FIXME:
		// if we have pselect, use it to sleep the remaining nanoseconds
		// (although won't know how many nanoseconds we weren't able to
		// sleep)...

		// if we have select, use it to sleep the remaining nanoseconds
		// (although we'll actually only get millisecond resolution
		// and won't know how many milliseconds we weren't able to
		// sleep)...

		// can we use a timer instead???
	#endif
}
