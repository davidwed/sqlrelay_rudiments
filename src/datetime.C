// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#include <rudiments/charstring.h>
#ifdef HAVE_RTC
	#include <rudiments/file.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#ifdef HAVE_RTC
	#include <linux/rtc.h>
	#include <sys/ioctl.h>
#endif

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__)
	pthread_mutex_t	*datetime::timemutex;
#endif

datetime::datetime() {
	zone=NULL;
	timestring=NULL;
	structtm=NULL;
}

datetime::~datetime() {
	delete[] zone;
	delete[] timestring;
	delete structtm;
}

bool datetime::initialize(const char *tmstring) {

	// get the date
	char	*ptr=(char *)tmstring;
	mon=atoi(ptr)-1;
	ptr=charstring::findFirst(ptr,'/')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	mday=atoi(ptr);
	ptr=charstring::findFirst(ptr,'/')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	year=atoi(ptr)-1900;

	// get the time
	ptr=charstring::findFirst(ptr,' ')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	hour=atoi(ptr);
	ptr=charstring::findFirst(ptr,':')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	min=atoi(ptr);
	ptr=charstring::findFirst(ptr,':')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	sec=atoi(ptr);

	// initialize the daylight savings time flag
	isdst=-1;

	// get the time zone if it was provided
	delete[] zone;
	if ((ptr=charstring::findFirst(ptr,' '))) {
		ptr=ptr+sizeof(char);
		zone=(ptr && ptr[0])?charstring::duplicate(ptr):NULL;
	} else {
		zone=NULL;
	}

	// normalize, string and epoch
	return normalizeBrokenDownTime(true);
}

bool datetime::initialize(time_t seconds) {
	epoch=seconds;
	return getBrokenDownTimeFromEpoch(true);
}

bool datetime::initialize(const struct tm *tmstruct) {

	sec=tmstruct->tm_sec;
	min=tmstruct->tm_min;
	hour=tmstruct->tm_hour;
	mday=tmstruct->tm_mday;
	mon=tmstruct->tm_mon;
	year=tmstruct->tm_year;
	wday=tmstruct->tm_wday;
	yday=tmstruct->tm_yday;
	isdst=tmstruct->tm_isdst;
	// FIXME: what if the zone/offset are garbage, is there a good way to
	// tell?
	#ifdef HAS___TM_ZONE
		zone=charstring::duplicate(tmstruct->__tm_zone);
	#elif HAS_TM_ZONE
		zone=charstring::duplicate(tmstruct->tm_zone);
	#elif HAS_TM_NAME
		zone=charstring::duplicate(tmstruct->tm_name);
	#else
		#ifdef RUDIMENTS_HAS_THREADS
			if (timemutex && !acquireLock()) {
				return false;
			}
		#endif
		tzset();
		zone=charstring::duplicate((tzname[0] && tzname[0][0])?
							tzname[0]:"UCT");
		#ifdef RUDIMENTS_HAS_THREADS
			releaseLock();
		#endif
	#endif
	#ifdef HAS___TM_GMTOFF
		gmtoff=tmstruct->__tm_gmtoff;
	#elif HAS_TM_GMTOFF
		gmtoff=tmstruct->tm_gmtoff;
	#elif HAS_TM_TZADJ
		gmtoff=-tmstruct->tm_tzadj;
	#else
		#ifdef RUDIMENTS_HAS_THREADS
			if (timemutex && !acquireLock()) {
				return false;
			}
		#endif
		tzset();
		#ifdef HAS_TIMEZONE
			gmtoff=-timezone;
		#else
			gmtoff=-_timezone;
		#endif
		#ifdef RUDIMENTS_HAS_THREADS
			releaseLock();
		#endif
	#endif
	return normalizeBrokenDownTime(true);
}

int datetime::getHour() const {
	return hour;
}

int datetime::getMinutes() const {
	return min;
}

int datetime::getSeconds() const {
	return sec;
}

int datetime::getMonth() const {
	return mon+1;
}

int datetime::getDayOfMonth() const {
	return mday;
}

int datetime::getDayOfWeek() const {
	return wday+1;
}

int datetime::getDayOfYear() const {
	return yday+1;
}

int datetime::getYear() const {
	return year+1900;
}

bool datetime::isDaylightSavingsTime() const {
	return isdst;
}

char	*datetime::getTimeZoneString() const {
	return zone;
}

long datetime::getTimeZoneOffset() const {
	return gmtoff;
}

time_t datetime::getEpoch() const {
	return epoch;
}

struct tm *datetime::getTm() {

	#ifdef RUDIMENTS_HAS_THREADS
		if (timemutex && !acquireLock()) {
			return NULL;
		}
	#endif

	delete structtm;
	structtm=new struct tm;
	structtm->tm_mon=mon;
	structtm->tm_mday=mday;
	structtm->tm_year=year;
	structtm->tm_hour=hour;
	structtm->tm_min=min;
	structtm->tm_sec=sec;
	structtm->tm_isdst=isdst;

	char	*oldzone;
	if ((zone && zone[0] && !setTimeZoneEnvVar(zone,&oldzone)) ||
		(mktime(structtm)==-1) ||
		(zone && zone[0] && !restoreTimeZoneEnvVar(oldzone))) {
		return NULL;
	}

	#ifdef RUDIMENTS_HAS_THREADS
		releaseLock();
	#endif
	return structtm;
}

void datetime::addSeconds(int seconds) {
	sec=sec+seconds;
	normalizeBrokenDownTime(true);
}

void datetime::addMinutes(int minutes) {
	min=min+minutes;
	normalizeBrokenDownTime(true);
}

void datetime::addHours(int hours) {
	hour=hour+hours;
	normalizeBrokenDownTime(true);
}

void datetime::addDays(int days) {
	hour=yday+days;
	normalizeBrokenDownTime(true);
}

void datetime::addMonths(int months) {
	hour=mon+months;
	normalizeBrokenDownTime(true);
}

void datetime::addYears(int years) {
	hour=year+years;
	normalizeBrokenDownTime(true);
}

#ifdef RUDIMENTS_HAS_THREADS
void datetime::setTimeMutex(pthread_mutex_t *mutex) {
	timemutex=mutex;
}
#endif

char *datetime::getString() {
	delete[] timestring;
	timestring=new char[100];
	sprintf(timestring,"%02d/%02d/%d %02d:%02d:%02d %s",
			getMonth(),getDayOfMonth(),getYear(),
			getHour(),getMinutes(),getSeconds(),
			getTimeZoneString());
	return timestring;
}


bool datetime::getSystemDateAndTime() {
	return initialize(time(NULL));
}

bool datetime::setSystemDateAndTime() {
	timeval	tv;
	tv.tv_sec=epoch;
	tv.tv_usec=0;
	// FIXME: should set /etc/localtime (or /etc/TZ) and TZ env var too...
	return !settimeofday(&tv,NULL);
}

bool datetime::getHardwareDateAndTime(const char *hwtz) {

	#ifdef HAVE_RTC
		// open the rtc
		file	devrtc;
		if (!devrtc.open("/dev/rtc",O_RDONLY)) {
			return false;
		}

		// get the time from the rtc
		rtc_time rt;
		if (devrtc.ioctl(RTC_RD_TIME,&rt)==-1) {
			devrtc.close();
			return false;
		}
		devrtc.close();

		// set the local values
		mon=rt.tm_mon;
		mday=rt.tm_mday;
		year=rt.tm_year;
		hour=rt.tm_hour;
		min=rt.tm_min;
		sec=rt.tm_sec;
		isdst=rt.tm_isdst;
		zone=charstring::duplicate(hwtz);

		return normalizeBrokenDownTime(true);
	#else
		return false;
	#endif
}

bool datetime::getAdjustedHardwareDateAndTime(const char *hwtz) {
	return (getHardwareDateAndTime(hwtz) && adjustTimeZone(NULL));
}

bool datetime::setHardwareDateAndTime(const char *hwtz) {

	#ifdef HAVE_RTC
		// open the rtc
		file	devrtc;
		if (!devrtc.open("/dev/rtc",O_WRONLY)) {
			return false;
		}

		// adjust the time zone
		if (!adjustTimeZone(hwtz)) {
			devrtc.close();
			return false;
		}

		// set the values to be stored in the rtc
		rtc_time rt;
		rt.tm_mon=mon;
		rt.tm_mday=mday;
		rt.tm_year=year;
		rt.tm_hour=hour;
		rt.tm_min=min;
		rt.tm_sec=sec;
		rt.tm_wday=wday;
		rt.tm_yday=yday;
		rt.tm_isdst=isdst;

		// set the rtc and clean up
		bool	retval=(devrtc.ioctl(RTC_SET_TIME,&rt)!=-1);
		devrtc.close();

		return retval;
	#else
		return false;
	#endif
}

bool datetime::adjustTimeZone(const char *newtz) {

	#ifdef RUDIMENTS_HAS_THREADS
		if (!acquireLock()) {
			return false;
		}
	#endif

	// Clear out the zone so getBrokenDownTimeFromEpoch() won't try to
	// preserve it.
	delete[] zone;
	zone=NULL;

	// Change the time zone, get the broken down time relative to the
	// current epoch, in the new time zone.
	char	*oldzone;
	bool	retval=true;
	if ((newtz && newtz[0] && !setTimeZoneEnvVar(newtz,&oldzone)) ||
		!getBrokenDownTimeFromEpoch(false) ||
		(newtz && newtz[0] && !restoreTimeZoneEnvVar(oldzone))) {
		retval=false;
	}

	#ifdef RUDIMENTS_HAS_THREADS
		releaseLock();
	#endif
	return retval;
}

char *datetime::getString(time_t seconds) {
	datetime	dt;
	return ((dt.initialize(seconds))?
		charstring::duplicate(dt.getString()):NULL);
}

char *datetime::getString(const struct tm *tmstruct) {
	datetime	dt;
	return ((dt.initialize(tmstruct))?
		charstring::duplicate(dt.getString()):NULL);
}

time_t datetime::getEpoch(const char *datestring) {
	datetime	dt;
	return ((dt.initialize(datestring))?dt.getEpoch():-1);
}

time_t datetime::getEpoch(const struct tm *tmstruct) {
	datetime	dt;
	return ((dt.initialize(tmstruct))?dt.getEpoch():-1);
}

bool datetime::setTimeZoneEnvVar(const char *zone, char **oldzone) {
	char	*tz=env.getValue("TZ");
	if (tz) {
		*oldzone=charstring::duplicate(tz);
	} else {
		*oldzone=NULL;
	}
	return env.setValue("TZ",zone);
}

bool datetime::restoreTimeZoneEnvVar(const char *oldzone) {
	if (oldzone) {
		bool	retval=env.setValue("TZ",oldzone);
		delete[] oldzone;
		return retval;
	}
	env.remove("TZ");
	return true;
}

bool datetime::getBrokenDownTimeFromEpoch(bool needmutex) {

	// I'm using localtime here instead of localtime_r because
	// localtime_r doesn't appear to handle the timezone properly,
	// at least, not in glibc-2.3
	#ifdef RUDIMENTS_HAS_THREADS
		if (needmutex && !acquireLock()) {
			return false;
		}
	#endif
	bool	retval=false;
	struct tm	*tms;
	if ((tms=localtime(&epoch))) {
		sec=tms->tm_sec;
		min=tms->tm_min;
		hour=tms->tm_hour;
		mday=tms->tm_mday;
		mon=tms->tm_mon;
		year=tms->tm_year;
		isdst=tms->tm_isdst;
		retval=normalizeBrokenDownTime(false);
	}
	#ifdef RUDIMENTS_HAS_THREADS
		if (needmutex) {
			releaseLock();
		}
	#endif
	return retval;
}

bool datetime::normalizeBrokenDownTime(bool needmutex) {

	#ifdef RUDIMENTS_HAS_THREADS
		if (needmutex && !acquireLock()) {
			return false;
		}
	#endif

	// If a time zone was passed in, use it.
	char	*oldzone=NULL;
	if (zone && zone[0] && !setTimeZoneEnvVar(zone,&oldzone)) {
		#ifdef RUDIMENTS_HAS_THREADS
			if (needmutex) {
				releaseLock();
			}
		#endif
		return false;
	}

	int	retval=true;

	// copy relevent values into a struct tm
	struct tm	tms;
	tms.tm_sec=sec;
	tms.tm_min=min;
	tms.tm_hour=hour;
	tms.tm_mday=mday;
	tms.tm_mon=mon;
	tms.tm_year=year;
	tms.tm_isdst=isdst;

	// mktime() will get the epoch, set wday
	// and yday and normalize other values
	time_t	ep=mktime(&tms);
	if (ep==-1) {
		retval=false;
	} else {

		// store the epoch
		epoch=ep;

		// copy values back out of struct tm
		sec=tms.tm_sec;
		min=tms.tm_min;
		hour=tms.tm_hour;
		mday=tms.tm_mday;
		mon=tms.tm_mon;
		year=tms.tm_year;
		isdst=tms.tm_isdst;
		wday=tms.tm_wday;
		yday=tms.tm_yday;
	
		// Use tzset to get the timezone name
		tzset();
		delete[] zone;
		zone=charstring::duplicate(tzname[0]);

		// Get the offset from the struct tm if we can, otherwise get
		// it from the value set by tzset()
		#ifdef HAS___TM_GMTOFF
			gmtoff=tms.__tm_gmtoff;
		#elif HAS_TM_GMTOFF
			gmtoff=tms.tm_gmtoff;
		#elif HAS_TM_TZADJ
			gmtoff=-tms.tm_tzadj;
		#elif HAS_TIMEZONE
			gmtoff=-timezone;
		#else
			gmtoff=-_timezone;
		#endif
	}

	if (oldzone) {
		retval=(retval && restoreTimeZoneEnvVar(oldzone));
	}

	#ifdef RUDIMENTS_HAS_THREADS
		if (needmutex) {
			releaseLock();
		}
	#endif

	return retval;
}

#ifdef RUDIMENTS_HAS_THREADS
bool datetime::acquireLock() {
	return !(timemutex && pthread_mutex_lock(timemutex));
}

bool datetime::releaseLock() {
	return !(timemutex && pthread_mutex_unlock(timemutex));
}
#endif
