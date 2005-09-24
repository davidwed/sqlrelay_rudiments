// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/intervaltimer.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class intervaltimerprivate {
	friend class intervaltimer;
	public:
		int		_which;
		itimerval	_values;
};

intervaltimer::intervaltimer(int which) {
	pvt=new intervaltimerprivate;
	pvt->_which=which;
	initialize();
}

intervaltimer::~intervaltimer() {
	delete pvt;
}

void intervaltimer::initialize() {
	rawbuffer::zero(&pvt->_values,sizeof(pvt->_values));
}

void intervaltimer::setInitialInterval(long seconds, long microseconds) {
	pvt->_values.it_value.tv_sec=seconds;
	pvt->_values.it_value.tv_usec=microseconds;
}

void intervaltimer::setInitialInterval(const timeval *tv) {
	pvt->_values.it_value=*tv;
}

void intervaltimer::setPeriodicInterval(long seconds, long microseconds) {
	pvt->_values.it_interval.tv_sec=seconds;
	pvt->_values.it_interval.tv_usec=microseconds;
}

void intervaltimer::setPeriodicInterval(const timeval *tv) {
	pvt->_values.it_interval=*tv;
}

void intervaltimer::setIntervals(long seconds, long microseconds) {
	setInitialInterval(seconds,microseconds);
	setPeriodicInterval(seconds,microseconds);
}

void intervaltimer::setIntervals(const timeval *tv) {
	setInitialInterval(tv);
	setPeriodicInterval(tv);
}

void intervaltimer::setIntervals(const itimerval *itv) {
	pvt->_values=*itv;
}

void intervaltimer::getInitialInterval(long *seconds,
					long *microseconds) const {
	if (seconds) {
		*seconds=pvt->_values.it_value.tv_sec;
	}
	if (microseconds) {
		*microseconds=pvt->_values.it_value.tv_usec;
	}
}

void intervaltimer::getInitialInterval(timeval *tv) const {
	*tv=pvt->_values.it_value;
}

void intervaltimer::getPeriodicInterval(long *seconds,
					long *microseconds) const {
	if (seconds) {
		*seconds=pvt->_values.it_interval.tv_sec;
	}
	if (microseconds) {
		*microseconds=pvt->_values.it_interval.tv_usec;
	}
}

void intervaltimer::getPeriodicInterval(timeval *tv) const {
	*tv=pvt->_values.it_interval;
}

void intervaltimer::getIntervals(itimerval *itv) const {
	*itv=pvt->_values;
}

bool intervaltimer::start() const {
	return start(NULL);
}

bool intervaltimer::start(itimerval *itv) const {
	// Solaris 8 complains if the 2nd argument isn't cast
	int	result;
	do {
		result=setitimer(pvt->_which,
				const_cast<itimerval *>(&pvt->_values),itv);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool intervaltimer::getTimeRemaining(long *seconds, long *microseconds) const {
	itimerval	val;
	int	result;
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
}

bool intervaltimer::getTimeRemaining(timeval *tv) const {
	// Some systems define tv_sec and/or tv_usec as something other than
	// long so we need to get longs and then let them auto-cast during
	// assignment below.
	long	sec;
	long	usec;
	bool	retval=getTimeRemaining(&sec,&usec);
	tv->tv_sec=sec;
	tv->tv_usec=usec;
	return retval;
}

bool intervaltimer::stop() const {
	itimerval	stopvals;
	rawbuffer::zero(&stopvals,sizeof(stopvals));
	return start(&stopvals);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
