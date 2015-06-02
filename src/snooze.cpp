// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>

// for timespec
#if defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_TIME_H)
	#include <time.h>
#elif defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SIGINFO)
	#include <sys/siginfo.h>
#elif defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_PTHREAD)
	// for mingw32
	#include <pthread.h>
#elif !defined(RUDIMENTS_HAVE_STRUCT_TIMESPEC_IN_SYS_TIME_H)
	#ifdef RUDIMENTS_HAVE_TIME_H
		// some systems define time_t in time.h
		#include <time.h>
	#endif
	struct timespec {
		time_t	tv_sec;
		long	tv_nsec;
	};
#endif

// for timeval
#if defined(RUDIMENTS_HAVE_SYS_TIME_H)
	#include <sys/time.h>
#endif

#if defined(RUDIMENTS_HAVE_NANOSLEEP) || defined(RUDIMENTS_HAVE_CLOCK_NANOSLEEP)
	#include <time.h>
#else
	#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
		#include <sys/types.h>
	#endif
	#ifdef RUDIMENTS_HAVE_SYS_SELECT_H
		#include <sys/select.h>
	#endif
	#ifdef RUDIMENTS_HAVE_UNISTD_H
		#include <unistd.h>
	#endif
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

bool snooze::macrosnooze(uint32_t seconds) {
	return nanosnooze(seconds,0,NULL,NULL);
}

bool snooze::macrosnooze(uint32_t seconds, uint32_t *remainingseconds) {
	return nanosnooze(seconds,0,remainingseconds,NULL);
}

bool snooze::microsnooze(uint32_t seconds, uint32_t microseconds) {
	return nanosnooze(seconds,microseconds*1000,NULL,NULL);
}

bool snooze::microsnooze(uint32_t seconds, uint32_t microseconds,
		uint32_t *secondsremaining, uint32_t *microsecondsremaining) {
	bool	retval=nanosnooze(seconds,microseconds*1000,
					secondsremaining,microsecondsremaining);
	if (microsecondsremaining) {
		*microsecondsremaining=(*microsecondsremaining)/1000;
	}
	return retval;
}

bool snooze::nanosnooze(uint32_t seconds, uint32_t nanoseconds) {
	return nanosnooze(seconds,nanoseconds,NULL,NULL);
}

bool snooze::nanosnooze(uint32_t seconds, uint32_t nanoseconds,
				uint32_t *secondsremaining,
				uint32_t *nanosecondsremaining) {

	timespec	timetosnooze;
	timetosnooze.tv_sec=seconds;
	timetosnooze.tv_nsec=nanoseconds;

	timespec	timeremaining;

	bool	retval=false;
	#ifdef RUDIMENTS_HAVE_NANOSLEEP
		retval=!::nanosleep(&timetosnooze,&timeremaining);
	#elif RUDIMENTS_HAVE_CLOCK_NANOSLEEP
		retval=!clock_nanosleep(CLOCK_REALTIME,TIME_ABSTIME,
					&timetosnooze,&timeremaining);
	#elif RUDIMENTS_HAVE_WINDOWS_SLEEP
		// on windows, we only have millisecond resolution and we
		// can't send the remaining time back
		Sleep(timetosnooze.tv_sec*1000+timetosnooze.tv_nsec/1000000);

		// set timeremaining to 0
		timeremaining.tv_sec=0;
		timeremaining.tv_nsec=0;
		retval=true;
	#else

		// use regular sleep command to handle the whole seconds
		bool	keepgoing=true;
		if (timetosnooze.tv_sec) {
			uint32_t	remainder=timetosnooze.tv_sec;
			error::clearError();
			do {
				remainder=::sleep(remainder);
			} while (error::getErrorNumber()==EINTR);
			if (remainder) {
				timeremaining.tv_sec=remainder;
				timeremaining.tv_nsec=timetosnooze.tv_nsec;
				keepgoing=false;
			}
		}

		if (keepgoing) {

			// set timeremaining to 0
			timeremaining.tv_sec=0;
			timeremaining.tv_nsec=0;
	
			// Use select or pselect to wait for the remaining time.
			//
			// It's tempting just to use select/pselect to sleep
			// the entire interval.  But on many platforms, select()
			// doesn't modify it's time struct to indicate how much
			// time was left when it timed out.  And on the
			// platforms that it does do that, if an error occurs
			// such as being interrupted by a signal, then the
			// values in the time struct are undefined anyway.
			//
			// So, when using select/pselect, we can't return the
			// number of nanoseconds that were left if a signal
			// interrupts the call.  But, at least we can return
			// the number of seconds.
			#ifdef RUDIMENTS_HAVE_PSELECT
				timespec	ts;
				ts.tv_sec=0;
				ts.tv_nsec=timetosnooze.tv_nsec;
				retval=(pselect(0,NULL,NULL,NULL,&ts,NULL)!=-1);
			#else
				timeval		tv;
				tv.tv_sec=0;
				tv.tv_usec=timetosnooze.tv_nsec/1000;
				retval=(select(0,NULL,NULL,NULL,&tv)!=-1);
			#endif
		}
	#endif

	if (secondsremaining) {
		*secondsremaining=timeremaining.tv_sec;
	}
	if (nanosecondsremaining) {
		*nanosecondsremaining=timeremaining.tv_nsec;
	}
	return retval;
}
