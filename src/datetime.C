// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#ifdef HAVE_RTC
	#include <rudiments/file.h>
#endif
#ifndef ENABLE_INLINES
	#include <rudiments/private/datetimeinlines.h>
#endif

#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif
#include <stdio.h>
#include <sys/time.h>

#ifdef HAVE_RTC
	#include <linux/rtc.h>
	#include <sys/ioctl.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif

#ifdef __GNUC__
pthread_mutex_t	*datetime::ltmutex;
pthread_mutex_t	*datetime::envmutex;
#endif

int datetime::initialize(const char *tmstring) {

	initTimeString();
	initTimeStruct();

	// get the date
	char	*ptr=(char *)tmstring;
	timestruct->tm_mon=atoi(ptr)-1;
	ptr=strchr(ptr,'/')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return 0;
	}
	timestruct->tm_mday=atoi(ptr);
	ptr=strchr(ptr,'/')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return 0;
	}
	timestruct->tm_year=atoi(ptr)-1900;

	// get the time
	ptr=strchr(ptr,' ')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return 0;
	}
	timestruct->tm_hour=atoi(ptr);
	ptr=strchr(ptr,':')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return 0;
	}
	timestruct->tm_min=atoi(ptr);
	ptr=strchr(ptr,':')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return 0;
	}
	timestruct->tm_sec=atoi(ptr);

	// initialize these fields
	timestruct->tm_wday=0;
	timestruct->tm_yday=0;
	timestruct->tm_isdst=0;

	// get the time zone (setTimeZone() will run updateTime())
	ptr=strchr(ptr,' ')+sizeof(char);
	return setTimeZone((ptr && ptr[0])?ptr:"GMT");
}

int datetime::initialize(time_t seconds) {

	if (seconds<0) {
		return 0;
	}

	initTimeString();
	initTimeStruct();

	#ifdef HAVE_LOCALTIME_R
		if (!localtime_r(&seconds,timestruct)) {
			return 0;
		}
		// update time will attach the current timezone
		return updateTime();
	#else
		if (ltmutex && pthread_mutex_lock(ltmutex)) {
			return 0;
		}
		struct tm	*lcltm=localtime(&seconds);
		int	retval=(lcltm && copyStructTm(lcltm,timestruct));
		if (ltmutex && pthread_mutex_unlock(ltmutex)) {
			return 0;
		}
		return retval;
	#endif
}

int datetime::initialize(const struct tm *tmstruct) {

	initTimeString();
	initTimeStruct();

	return copyStructTm(tmstruct,timestruct);
}

char *datetime::getString() {
	if (!timestring) {
		timestring=new char[100];
		strftime(timestring,100,"%m/%d/%Y %H:%M:%S %Z",timestruct);
	}
	return timestring;
}

int datetime::getSystemDateAndTime() {
	return initialize(time(NULL));
}

int datetime::setSystemDateAndTime() {
	timeval	tv;
	tv.tv_sec=epoch;
	tv.tv_usec=0;
	// should set the time zone too...
	return !settimeofday(&tv,NULL);
}

int datetime::getHardwareDateAndTime(const char *hwtz) {

	#ifdef HAVE_RTC
		// open the rtc
		file	devrtc;
		if ((devrtc.open("/dev/rtc",O_RDONLY))==-1) {
			return 0;
		}

		// get the time from the rtc
		rtc_time rt;
		if (ioctl(devrtc.getFileDescriptor(),RTC_RD_TIME,&rt)==-1) {
			devrtc.close();
			return 0;
		}
		devrtc.close();

		initTimeString();
		initTimeStruct();

		// set the local timestruct values
		timestruct->tm_mon=rt.tm_mon;
		timestruct->tm_mday=rt.tm_mday;
		timestruct->tm_year=rt.tm_year;
		timestruct->tm_hour=rt.tm_hour;
		timestruct->tm_min=rt.tm_min;
		timestruct->tm_sec=rt.tm_sec;
		timestruct->tm_wday=0;
		timestruct->tm_yday=0;
		timestruct->tm_isdst=rt.tm_isdst;

		// setTimeZone() will call updateTime()
		// so we don't need to do that here
		if (!setTimeZone(hwtz)) {
			return 0;
		}
		return 1;
	#else
		return 0;
	#endif
}

int datetime::getAdjustedHardwareDateAndTime(const char *hwtz) {
	return (getHardwareDateAndTime(hwtz) && adjustTimeZone(NULL));
}

int datetime::setHardwareDateAndTime(const char *hwtz) {

	#ifdef HAVE_RTC
		// open the rtc
		file	devrtc;
		if ((devrtc.open("/dev/rtc",O_WRONLY))==-1) {
			return 0;
		}

		// adjust the time zone
		if (!adjustTimeZone(hwtz)) {
			devrtc.close();
			return 0;
		}

		// set the values to be stored in the rtc
		rtc_time rt;
		rt.tm_mon=timestruct->tm_mon;
		rt.tm_mday=timestruct->tm_mday;
		rt.tm_year=timestruct->tm_year;
		rt.tm_hour=timestruct->tm_hour;
		rt.tm_min=timestruct->tm_min;
		rt.tm_sec=timestruct->tm_sec;
		rt.tm_wday=timestruct->tm_wday;
		rt.tm_yday=timestruct->tm_yday;
		rt.tm_isdst=timestruct->tm_isdst;

		// set the rtc and clean up
		int	retval=(ioctl(devrtc.getFileDescriptor(),
						RTC_SET_TIME,&rt)!=-1);
		devrtc.close();

		return retval;
	#else
		return 0;
	#endif
}

int datetime::adjustTimeZone(const char *newtz) {

	// obtain exclusive access to localtime
	if (ltmutex && pthread_mutex_lock(ltmutex)) {
		return 0;
	}

	// change the time zone
	char	*oldzone;
	if (newtz) {
		if (!setTimeZoneEnvVar(newtz,&oldzone)) {
			return 0;
		}
	}

	initTimeString();
	initTimeStruct();

	// Get a struct tm relative to this new time zone...
	// Note that this must use localtime, not localtime_r or the timezone
	// will not be set.  Also, there's no advantage to using localtime_r
	// here because this whole method is mutex'ed anyway.
	struct tm	*lcltm=localtime(&epoch);
	if (!lcltm) {
		return 0;
	}

	// replace the current struct tm the new struct tm
	if (!copyStructTm(lcltm,timestruct)) {
		return 0;
	}

	if (newtz) {
		restoreTimeZoneEnvVar(oldzone);
	}

	// release exclusive access to localtime
	if (ltmutex && pthread_mutex_unlock(ltmutex)) {
		return 0;
	}
	return 1;
}

int datetime::setTimeZone(const char *tz) {

	// create a copy of the current struct tm and use it from now on
	//
	// Why not just call mktime() on the existing struct tm?
	//
	// The timezone string is (const char *), at least on linux it is.
	// So if you set it once, you can't change it.  If the timezone has
	// already been set by a previous call to mktime() and this call to
	// mktime() would cause it to be changed, it will cause serious
	// problems.  To get around this, we have to create an all new
	// struct tm, copy the old data into it, then call mktime() to set the
	// timezone/offset.
	struct tm	*newtimestruct=new struct tm;
	newtimestruct->tm_mon=timestruct->tm_mon;
	newtimestruct->tm_mday=timestruct->tm_mday;
	newtimestruct->tm_year=timestruct->tm_year;
	newtimestruct->tm_hour=timestruct->tm_hour;
	newtimestruct->tm_min=timestruct->tm_min;
	newtimestruct->tm_sec=timestruct->tm_sec;
	newtimestruct->tm_wday=0;
	newtimestruct->tm_yday=0;
	newtimestruct->tm_isdst=timestruct->tm_isdst;
	delete timestruct;
	timestruct=newtimestruct;

	// obtain exclusive access to the environment variables
	if (envmutex && pthread_mutex_lock(envmutex)) {
		return 0;
	}

	// change the current timezone
	char	*oldtz;
	setTimeZoneEnvVar(tz,&oldtz);

	// call mktime to fill in the zone/offset and set the epoch
	int	retval=updateTime();

	// restore the original timezone
	restoreTimeZoneEnvVar(oldtz);

	// release exclusive access to the environment variables
	if (envmutex && pthread_mutex_lock(envmutex)) {
		return 0;
	}

	return retval;
}

int	datetime::updateTime() {
	return ((epoch=mktime(timestruct))!=-1);
}

int	datetime::updateTimePreservingTimeZone() {
	// FIXME: What to do if oldtm doesn't
	// have it's tm_zone and tm_gmtoff set?
	char	*currentzone=strdup(getTimeZoneString());
	int	retval=setTimeZone(currentzone);
	delete[] currentzone;
	return retval;
}

int	datetime::copyStructTm(const struct tm *oldtm, struct tm *newtm) {

	// what we want to do here is copy oldtm to newtm and set epoch,
	// without modifying the timezone stored in oldtm or newtm...

	// Get the epoch here
	//
	// We can't just call mktime on oldtm because it will try to set it's
	// timezone (which is a const and can't be set more than once), so
	// we'll copy the relevent parts over to temptm and call mktime on that.
	// We don't care what temptm's time zone gets set to.
	//
	// Oddly, gcc-3.3 gets pissed if we just do:
	// struct tm	temptm;
	// for struct tm's that define the timezone abbreviationm as a const
	// char array instead of a const char pointer.  But it allows it if
	// you do:
	// struct tm	*temptm=new struct tm;
	// go figure...
	struct tm	*temptm=new struct tm;
	temptm->tm_sec=oldtm->tm_sec;
	temptm->tm_min=oldtm->tm_min;
	temptm->tm_hour=oldtm->tm_hour;
	temptm->tm_mday=oldtm->tm_mday;
	temptm->tm_mon=oldtm->tm_mon;
	temptm->tm_year=oldtm->tm_year;
	temptm->tm_wday=oldtm->tm_wday;
	temptm->tm_yday=oldtm->tm_yday;
	temptm->tm_isdst=oldtm->tm_isdst;
	time_t	tempepoch=mktime(temptm);
	delete temptm;
	if (tempepoch==-1) {
		return 0;
	}
	epoch=tempepoch;

	// now copy oldtm into newtm
	newtm->tm_sec=oldtm->tm_sec;
	newtm->tm_min=oldtm->tm_min;
	newtm->tm_hour=oldtm->tm_hour;
	newtm->tm_mday=oldtm->tm_mday;
	newtm->tm_mon=oldtm->tm_mon;
	newtm->tm_year=oldtm->tm_year;
	newtm->tm_wday=oldtm->tm_wday;
	newtm->tm_yday=oldtm->tm_yday;
	newtm->tm_isdst=oldtm->tm_isdst;

	// FIXME: what should happen here if oldtm's timezone/offset were
	// not set?

	return 1;
}
