// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <rudiments/intervaltimer.h>
#include <rudiments/rawbuffer.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

intervaltimer::intervaltimer(int which) {
	this->which=which;
	initialize();
}

void intervaltimer::initialize() {
	rawbuffer::zero(static_cast<void *>(&values),sizeof(values));
}

void intervaltimer::setInitialInterval(long seconds, long microseconds) {
	values.it_value.tv_sec=seconds;
	values.it_value.tv_usec=microseconds;
}

void intervaltimer::setInitialInterval(timeval *tv) {
	values.it_value=*tv;
}

void intervaltimer::setPeriodicInterval(long seconds, long microseconds) {
	values.it_interval.tv_sec=seconds;
	values.it_interval.tv_usec=microseconds;
}

void intervaltimer::setPeriodicInterval(timeval *tv) {
	values.it_interval=*tv;
}

void intervaltimer::setIntervals(long seconds, long microseconds) {
	setInitialInterval(seconds,microseconds);
	setPeriodicInterval(seconds,microseconds);
}

void intervaltimer::setIntervals(timeval *tv) {
	setInitialInterval(tv);
	setPeriodicInterval(tv);
}

void intervaltimer::setIntervals(itimerval *itv) {
	values=*itv;
}

void intervaltimer::getInitialInterval(long *seconds,
					long *microseconds) const {
	if (seconds) {
		*seconds=values.it_value.tv_sec;
	}
	if (microseconds) {
		*microseconds=values.it_value.tv_usec;
	}
}

void intervaltimer::getInitialInterval(timeval *tv) const {
	*tv=values.it_value;
}

void intervaltimer::getPeriodicInterval(long *seconds,
					long *microseconds) const {
	if (seconds) {
		*seconds=values.it_interval.tv_sec;
	}
	if (microseconds) {
		*microseconds=values.it_interval.tv_usec;
	}
}

void intervaltimer::getPeriodicInterval(timeval *tv) const {
	*tv=values.it_interval;
}

void intervaltimer::getIntervals(itimerval *itv) const {
	*itv=values;
}

bool intervaltimer::start() const {
	return start(NULL);
}

bool intervaltimer::start(itimerval *itv) const {
	// Solaris 8 complains if the 2nd argument isn't cast
	return !setitimer(which,const_cast<itimerval *>(&values),itv);
}

bool intervaltimer::getTimeRemaining(long *seconds, long *microseconds) const {
	itimerval	val;
	bool	retval=getitimer(which,&val);
	if (seconds) {
		*seconds=val.it_value.tv_sec;
	}
	if (microseconds) {
		*microseconds=val.it_value.tv_usec;
	}
	return retval;
}

bool intervaltimer::getTimeRemaining(timeval *tv) const {
	// Some systems define tv_usec as something other than long so we need
	// to get a long and then let it auto-cast during assignment below.
	long	usec;
	bool	retval=getTimeRemaining(&(tv->tv_sec),&usec);
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
