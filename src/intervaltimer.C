// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/intervaltimer.h>

#include <stdlib.h>
#include <errno.h>

#if defined(HAVE_NANOSLEEP) || defined (HAVE_CLOCK_NANOSLEEP)
	#include <time.h>
#else
	#include <sys/types.h>
	#include <sys/select.h>
	#include <unistd.h>
#endif

intervaltimer::intervaltimer(int which) {
	this->which=which;
}

bool intervaltimer::setTimer(long seconds) {
	return setTimer(seconds,0);
}

bool intervaltimer::setTimer(long seconds, long microseconds) {
	timeval	value;
	value.tv_sec=seconds;
	value.tv_usec=microseconds;
	return setTimer(&value);
}

bool intervaltimer::setTimer(timeval *value) {
	itimerval	values;
	values.it_interval.tv_sec=0;
	values.it_interval.tv_usec=0;
	values.it_value.tv_sec=value->tv_sec;
	values.it_value.tv_usec=value->tv_usec;
	return setTimer(&values);
}

bool intervaltimer::setTimer(itimerval *values) {
	itimerval	oldvalues;
	return setTimer(values,&oldvalues);
}

bool intervaltimer::setTimer(itimerval *values, itimerval *oldvalues) {
	return !setitimer(which,values,oldvalues);
}

bool intervaltimer::cancelTimer() {
	return setTimer((long)0,(long)0);
}

bool intervaltimer::getTimeRemaining(long *seconds) {
	return getTimeRemaining(seconds,NULL);
}

bool intervaltimer::getTimeRemaining(long *seconds, long *microseconds) {
	timeval	value;
	bool	retval=getTimeRemaining(&value);
	if (seconds) {
		*seconds=value.tv_sec;
	}
	if (microseconds) {
		*microseconds=value.tv_usec;
	}
	return retval;
}

bool intervaltimer::getTimeRemaining(timeval *value) {
	itimerval	values;
	bool	retval=getitimer(which,&values);
	value->tv_sec=values.it_value.tv_sec;
	value->tv_usec=values.it_value.tv_usec;
	return retval;
}

bool intervaltimer::getInterval(long *seconds) {
	return getInterval(seconds,NULL);
}

bool intervaltimer::getInterval(long *seconds, long *microseconds) {
	timeval	value;
	bool	retval=getInterval(&value);
	if (seconds) {
		*seconds=value.tv_sec;
	}
	if (microseconds) {
		*microseconds=value.tv_usec;
	}
	return retval;
}

bool intervaltimer::getInterval(timeval *value) {
	itimerval	values;
	bool	retval=getitimer(which,&values);
	value->tv_sec=values.it_interval.tv_sec;
	value->tv_usec=values.it_interval.tv_usec;
	return retval;
}

bool intervaltimer::getTimer(itimerval *values) {
	return !getitimer(which,values);
}

bool intervaltimer::sleep(long seconds) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=seconds;
	nanotimetosleep.tv_nsec=0;
	return nanosleep(&nanotimetosleep);
}

bool intervaltimer::sleep(long seconds, long *remainingseconds) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=seconds;
	nanotimetosleep.tv_nsec=0;
	timespec	nanotimeremaining;
	bool	retval=nanosleep(&nanotimetosleep);
	*remainingseconds=nanotimeremaining.tv_sec;
	return retval;
}

bool intervaltimer::microsleep(long seconds, long microseconds) {
	timeval		timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_usec=microseconds;
	return microsleep(&timetosleep);
}

bool intervaltimer::microsleep(long seconds, long microseconds,
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

bool intervaltimer::microsleep(timeval *timetosleep) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=timetosleep->tv_sec;
	nanotimetosleep.tv_nsec=timetosleep->tv_usec*1000;
	return nanosleep(&nanotimetosleep);
}

bool intervaltimer::microsleep(timeval *timetosleep, timeval *timeremaining) {
	timespec	nanotimetosleep;
	nanotimetosleep.tv_sec=timetosleep->tv_sec;
	nanotimetosleep.tv_nsec=timetosleep->tv_usec*1000;
	timespec	nanotimeremaining;
	bool	retval=nanosleep(&nanotimetosleep,&nanotimeremaining);
	timeremaining->tv_sec=nanotimeremaining.tv_sec;
	timeremaining->tv_usec=nanotimeremaining.tv_nsec/1000;
	return retval;
}

bool intervaltimer::nanosleep(long seconds, long nanoseconds) {
	timespec	timetosleep;
	timetosleep.tv_sec=seconds;
	timetosleep.tv_nsec=nanoseconds;
	return nanosleep(&timetosleep);
}

bool intervaltimer::nanosleep(long seconds, long nanoseconds,
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

bool intervaltimer::nanosleep(timespec *timetosleep) {

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

bool intervaltimer::nanosleep(timespec *timetosleep, timespec *timeremaining) {

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
		// the values in the time struct are indefined anyway.
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
