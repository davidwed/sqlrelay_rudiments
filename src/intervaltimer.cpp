// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/intervaltimer.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif

#ifndef RUDIMENTS_HAVE_STRUCT_TIMEVAL
	struct timeval {
		time_t		tv_sec;
		uint32_t	tv_usec;
	};
#endif

#ifndef RUDIMENTS_HAVE_STRUCT_ITIMERVAL
	struct itimerval {
		struct timeval	it_interval;
		struct timeval	it_value;
	};
#endif

class intervaltimerprivate {
	friend class intervaltimer;
	public:
		int32_t		_which;
		itimerval	_values;
};

intervaltimer::intervaltimer(int32_t which) {
	pvt=new intervaltimerprivate;
	pvt->_which=which;
	initialize();
}

intervaltimer::~intervaltimer() {
	delete pvt;
}

void intervaltimer::initialize() {
	bytestring::zero(&pvt->_values,sizeof(pvt->_values));
}

void intervaltimer::setInitialInterval(int32_t seconds, int32_t microseconds) {
	pvt->_values.it_value.tv_sec=seconds;
	pvt->_values.it_value.tv_usec=microseconds;
}

void intervaltimer::setPeriodicInterval(int32_t seconds, int32_t microseconds) {
	pvt->_values.it_interval.tv_sec=seconds;
	pvt->_values.it_interval.tv_usec=microseconds;
}

void intervaltimer::setIntervals(int32_t seconds, int32_t microseconds) {
	setInitialInterval(seconds,microseconds);
	setPeriodicInterval(seconds,microseconds);
}

void intervaltimer::getInitialInterval(int32_t *seconds,
					int32_t *microseconds) const {
	if (seconds) {
		*seconds=pvt->_values.it_value.tv_sec;
	}
	if (microseconds) {
		*microseconds=pvt->_values.it_value.tv_usec;
	}
}

void intervaltimer::getPeriodicInterval(int32_t *seconds,
					int32_t *microseconds) const {
	if (seconds) {
		*seconds=pvt->_values.it_interval.tv_sec;
	}
	if (microseconds) {
		*microseconds=pvt->_values.it_interval.tv_usec;
	}
}

bool intervaltimer::start() const {
	return start(NULL,NULL);
}

bool intervaltimer::start(int32_t *seconds, int32_t *microseconds) const {
	#ifdef RUDIMENTS_HAVE_SETITIMER
		itimerval	oldval;
		int32_t		result;
		do {
			// Solaris 8 complains if the 2nd argument isn't cast
			result=setitimer(pvt->_which,
				const_cast<itimerval *>(&pvt->_values),
				&oldval);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (seconds) {
			*seconds=oldval.it_value.tv_sec;
		}
		if (microseconds) {
			*microseconds=oldval.it_value.tv_usec;
		}
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool intervaltimer::getTimeRemaining(int32_t *seconds,
					int32_t *microseconds) const {
	#ifdef RUDIMENTS_HAVE_SETITIMER
		itimerval	val;
		int32_t	result;
		do {
			result=getitimer(pvt->_which,&val);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (seconds) {
			*seconds=val.it_value.tv_sec;
		}
		if (microseconds) {
			*microseconds=val.it_value.tv_usec;
		}
		return !result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool intervaltimer::stop() {
	int32_t	isec;
	int32_t	imicrosec;
	getInitialInterval(&isec,&imicrosec);
	int32_t	psec;
	int32_t	pmicrosec;
	getPeriodicInterval(&psec,&pmicrosec);
	setIntervals(0,0);
	bool	retval=start(NULL,NULL);
	setInitialInterval(isec,imicrosec);
	setPeriodicInterval(psec,pmicrosec);
	return retval;
}
