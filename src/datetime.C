// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#include <rudiments/file.h>
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

datetime::datetime() {
	timestruct=NULL;
	timestring=NULL;
	epoch=0;
}

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
		// should protect this with a mutex
		struct tm	*lcltm=localtime(&seconds);
		return (lcltm && copyStructTm(lcltm,timestruct));
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

	// this should be surrounded by a mutex...

	// change the time zone
	char	*oldzone;
	if (newtz) {
		if (!setTimeZoneEnvVar(newtz,&oldzone)) {
			return 0;
		}
	}

	initTimeString();
	initTimeStruct();

	// get a struct tm relative to this new time zone
	//	(note that this must use localtime, not
	//		localtime_r or the timezone will not be set)
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

	// this should be surrounded by a mutex...

	// change the current timezone
	char	*oldtz;
	setTimeZoneEnvVar(tz,&oldtz);

	// call mktime to fill in the zone/offset and set the epoch
	int	retval=updateTime();

	// restore the original timezone
	restoreTimeZoneEnvVar(oldtz);

	return retval;
}

long datetime::getTimeZoneOffset(const char *zone) {

	// this should be surrounded by a mutex...

	// change the current timezone
	char	*oldzone;
	setTimeZoneEnvVar(zone,&oldzone);

	// set "timezone" (a stinkin' global variable)
	tzset();

	// interestingly enough, tzset() returns seconds east of GMT while
	// every other time-related function returns seconds west of GMT,
	// go figure...
	long	retval=-1*timezone;

	// restore the original timezone
	restoreTimeZoneEnvVar(oldzone);

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

	// get the epoch here, we don't care if temptm's zone gets screwed up
	struct tm	temptm;
	memcpy(&temptm,oldtm,sizeof(struct tm));
	time_t	tempepoch=mktime(&temptm);
	if (tempepoch==-1) {
		return 0;
	}
	epoch=tempepoch;

	// now copy newtm into oldtm
	memcpy(newtm,oldtm,sizeof(struct tm));

	// FIXME: what should happen here if oldtm's timezone/offset were
	// not set?

	return 1;
}
