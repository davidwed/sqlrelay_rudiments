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
	#else
		struct tm	*lcltm=localtime(&seconds);
		if (!lcltm || !copyStructTm(lcltm,timestruct)) {
			return 0;
		}
	#endif

	return updateTime();
}

int datetime::initialize(const struct tm *tmstruct) {

	initTimeString();
	initTimeStruct();

	copyStructTm(tmstruct,timestruct);

	return updateTime();
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

	if (!getHardwareDateAndTime(hwtz)) {
		return 0;
	}

	datetime	dt;
	dt.getSystemDateAndTime();

	// adjust the current timezone to the timezone of the system clock
	return adjustTimeZone(dt.getTimeZoneString());
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

	initTimeString();
	timestruct->tm_sec=timestruct->tm_sec-
				getTimeZoneOffset()+
					getTimeZoneOffset(newtz);

	// setTimeZone() will call updateTime()
	// so we don't have to do that here
	return (!setTimeZone(newtz));
}

int datetime::setTimeZone(const char *tz) {

	// this whole function should be surrounded by a mutex

	// change the current timezone
	char	*oldtz;
	setTimeZoneEnvVar(tz,&oldtz);

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

	// call mktime to fill in the zone/offset and set the epoch
	int	retval=updateTime();

	// restore the original timezone
	restoreTimeZoneEnvVar(oldtz);

	return retval;
}

long datetime::getTimeZoneOffset(const char *zone) {

	// this whole function should be surrounded by a mutex

	// change the current timezone
	char	*oldtz;
	setTimeZoneEnvVar(zone,&oldtz);

	// set "timezone" (a stinkin' global variable)
	tzset();

	// interestingly enough, tzset() returns seconds east of GMT while
	// every other time-related function returns seconds west of GMT,
	// go figure...
	long	retval=-1*timezone;

	// restore the original timezone
	restoreTimeZoneEnvVar(oldtz);

	return retval;
}

int	datetime::updateTime() {
	return ((epoch=mktime(timestruct))!=-1);
}

int	datetime::copyStructTm(const struct tm *oldtm, struct tm *newtm) {

	// copy parts
	newtm->tm_mon=oldtm->tm_mon;
	newtm->tm_mday=oldtm->tm_mday;
	newtm->tm_year=oldtm->tm_year;
	newtm->tm_hour=oldtm->tm_hour;
	newtm->tm_min=oldtm->tm_min;
	newtm->tm_sec=oldtm->tm_sec;
	newtm->tm_wday=0;
	newtm->tm_yday=0;
	newtm->tm_isdst=oldtm->tm_isdst;

	// this whole function should be surrounded by a mutex

	// get the current timezone
	char	*oldtz;
	setTimeZoneEnvVar(getTimeZoneString(oldtm),&oldtz);

	// call mktime to fill in the zone/offset
	int	retval=updateTime();

	// restore the original timezone
	restoreTimeZoneEnvVar(oldtz);

	return retval;
}
