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

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__)
mutex	*datetime::timemutex;
#endif

static const char monthname[][10]={
	"January","February","March",
	"April","May","June",
	"July","August","September",
	"October","November","December"
};

static const char monthabbr[][4]={
	"Jan","Feb","Mar",
	"Apr","May","Jun",
	"Jul","Aug","Sep",
	"Oct","Nov","Dec"
};

datetime::datetime() {
	#ifdef HAVE_GETSYSTEMTIME
		rawbuffer::zero(st,sizeof(SYSTEMTIME));
		rawbuffer::zero(tzi,sizeof(TIME_ZONE_INFORMATION));
	#else
		sec=0;
		min=0;
		hour=0;
		mday=0;
		mon=0;
		year=0;
		wday=0;
		yday=0;
		isdst=0;
		zone=NULL;
		gmtoff=0;
		timestring=NULL;
		structtm=NULL;
		epoch=0;
	#endif
	#ifdef RUDIMENTS_HAS_THREADS
	timemutex=NULL;
	#endif
}

datetime::~datetime() {
	#ifndef HAVE_GETSYSTEMTIME
		delete[] zone;
		delete[] timestring;
		delete structtm;
	#endif
}

bool datetime::initialize(const char *tmstring) {

	// get the date
	const char	*ptr=tmstring;
	#ifdef HAVE_GETSYSTEMTIME
	st.wMonth=charstring::toShort(ptr);
	#else
	mon=charstring::toInteger(ptr)-1;
	#endif
	ptr=charstring::findFirst(ptr,'/')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	#ifdef HAVE_GETSYSTEMTIME
	st.wDay=charstring::toShort(ptr);
	#else
	mday=charstring::toInteger(ptr);
	#endif
	ptr=charstring::findFirst(ptr,'/')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	#ifdef HAVE_GETSYSTEMTIME
	st.wYear=charstring::toShort(ptr);
	#else
	year=charstring::toInteger(ptr)-1900;
	#endif

	// get the time
	ptr=charstring::findFirst(ptr,' ')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	#ifdef HAVE_GETSYSTEMTIME
	st.wHour=charstring::toShort(ptr);
	#else
	hour=charstring::toInteger(ptr);
	#endif
	ptr=charstring::findFirst(ptr,':')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	#ifdef HAVE_GETSYSTEMTIME
	st.wMinute=charstring::toShort(ptr);
	#else
	min=charstring::toInteger(ptr);
	#endif
	ptr=charstring::findFirst(ptr,':')+sizeof(char);
	if (!ptr || !ptr[0]) {
		return false;
	}
	#ifdef HAVE_GETSYSTEMTIME
	st.wSecond=charstring::toShort(ptr);
	#else
	sec=charstring::toInteger(ptr);
	#endif

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
		zone=charstring::duplicate((tzname[isdst] && tzname[isdst][0])?
							tzname[isdst]:"UCT");
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

int32_t datetime::getHour() const {
	return hour;
}

int32_t datetime::getMinutes() const {
	return min;
}

int32_t datetime::getSeconds() const {
	return sec;
}

int32_t datetime::getMonth() const {
	return mon+1;
}

const char *datetime::getMonthName() const {
	return monthname[mon];
}

const char *datetime::getMonthAbbreviation() const {
	return monthabbr[mon];
}

int32_t datetime::getDayOfMonth() const {
	return mday;
}

int32_t datetime::getDayOfWeek() const {
	return wday+1;
}

int32_t datetime::getDayOfYear() const {
	return yday+1;
}

int32_t datetime::getYear() const {
	return year+1900;
}

bool datetime::isDaylightSavingsTime() const {
	return isdst;
}

const char *datetime::getTimeZoneString() const {
	return zone;
}

int32_t datetime::getTimeZoneOffset() const {
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

	// FIXME: lookup combined zone here too
	char	*oldzone;
	if ((zone && zone[0] && !setTimeZoneEnvVar(zone,&oldzone,false)) ||
		(mktime(structtm)==-1) ||
		(zone && zone[0] && !restoreTimeZoneEnvVar(oldzone))) {
		return NULL;
	}

	#ifdef RUDIMENTS_HAS_THREADS
		releaseLock();
	#endif
	return structtm;
}

bool datetime::addSeconds(int32_t seconds) {
	sec=sec+seconds;
	return normalizeBrokenDownTime(true);
}

bool datetime::addMinutes(int32_t minutes) {
	min=min+minutes;
	return normalizeBrokenDownTime(true);
}

bool datetime::addHours(int32_t hours) {
	hour=hour+hours;
	return normalizeBrokenDownTime(true);
}

bool datetime::addDays(int32_t days) {
	hour=yday+days;
	return normalizeBrokenDownTime(true);
}

bool datetime::addMonths(int32_t months) {
	hour=mon+months;
	return normalizeBrokenDownTime(true);
}

bool datetime::addYears(int32_t years) {
	hour=year+years;
	return normalizeBrokenDownTime(true);
}

#ifdef RUDIMENTS_HAS_THREADS
void datetime::setTimeMutex(mutex *mtx) {
	timemutex=mtx;
}
#endif

const char *datetime::getString() {
	delete[] timestring;
	timestring=new char[2+1+2+1+charstring::integerLength(getYear())+1+
				2+1+2+1+2+1+
				charstring::length(getTimeZoneString())];
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
	return adjustTimeZone(newtz,false);
}

bool datetime::adjustTimeZone(const char *newtz, bool ignoredst) {

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
	if ((newtz && newtz[0] &&
			!setTimeZoneEnvVar(newtz,&oldzone,ignoredst)) ||
		!getBrokenDownTimeFromEpoch(false) ||
			(newtz && newtz[0] &&
				!restoreTimeZoneEnvVar(oldzone))) {
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

bool datetime::setTimeZoneEnvVar(const char *zone, char **oldzone,
							bool ignoredst) {

	// If a daylight timezone was passed in, override it with the combined
	// timezone.
	//
	// If a standard timezone was passed in, but we want to account for
	// daylight savings time (ie. EST was passed in, but daylight savings
	// time is currently in effect, so we want to use EDT instead) then
	// use the combined timezone.
	const char	*combinedzone=lookupCombinedTimeZone(zone);
	const char	*realzone=zone;
	if (daylightZone(zone) || !ignoredst) {
		realzone=combinedzone;
	}

	const char	*tz=env.getValue("TZ");
	if (tz) {
		*oldzone=charstring::duplicate(tz);
	} else {
		*oldzone=NULL;
	}
	return env.setValue("TZ",realzone);
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

	if (zone && zone[0] && !setTimeZoneEnvVar(zone,&oldzone,false)) {
		#ifdef RUDIMENTS_HAS_THREADS
			if (needmutex) {
				releaseLock();
			}
		#endif
		return false;
	}

	bool	retval=true;

	// copy relevent values into a struct tm
	struct tm	tms;
	tms.tm_sec=sec;
	tms.tm_min=min;
	tms.tm_hour=hour;
	tms.tm_mday=mday;
	tms.tm_mon=mon;
	tms.tm_year=year;
	tms.tm_isdst=isdst;

	// mktime() will get the epoch, set wday, yday, isdst
	// and normalize other values
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
		zone=charstring::duplicate(tzname[isdst]);

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
	return !(timemutex && !timemutex->lock());
}

bool datetime::releaseLock() {
	return !(timemutex && !timemutex->unlock());
}
#endif

static const char * const timezones[]={

	"ACST",	// Australian Central Standard Time	UTC + 9:30 hours
	"ACDT",	// Australian Central Daylight Time	UTC + 10:30 hours
	"ACST-10:30ACDT",

	"AST",	// Atlantic Standard Time		UTC - 4 hours
	"ADT",	// Atlantic Daylight Time		UTC - 3 hours
	"AST4ADT",

	"AEST",	// Australian Eastern Standard Time	UTC + 10 hours
	"AEDT",	// Australian Eastern Daylight Time	UTC + 11 hours
	"AEST10AEDT",

	"AKST",	// Alaska Standard Time	UTC - 9 hours
	"AKDT",	// Alaska Daylight Time	UTC - 8 hours
	"AKST-9AKDT",

	"CST",	// Central Standard Time	UTC - 6 hours
	"CDT",	// Central Daylight Time	UTC - 5 hours
	"CST6CDT",

	"CET",	// Central European Time	UTC + 1 hour
	"CEST",	// Central European Summer Time	UTC + 2 hours
	"CET-1CST",

	"EST",	// Eastern Standard Time	UTC - 5 hours
	"EDT",	// Eastern Daylight Time	UTC - 4 hours
	"EST5EDT",

	"EET",	// Eastern European Time	UTC + 2 hours
	"EEST",	// Eastern European Summer Time	UTC + 3 hours
	"EET-2EEST",

	"GMT",	// Greenwich Mean Time	UTC
	"BST",	// British Summer Time	UTC + 1 hour
	"GMT0BST",

	"HNA",	// Heure Normale de l'Atlantique	UTC - 4 hours
	"HAA",	// Heure Avancée de l'Atlantique	UTC - 3 hours
	"HNA4HAA",

	"HNC",	// Heure Normale du Centre		UTC - 6 hours
	"HAC",	// Heure Avancée du Centre		UTC - 5 hours
	"HNC6HAC",

	"HAST",	// Hawaii-Aleutian Standard Time	UTC - 10 hours
	"HADT",	// Hawaii-Aleutian Daylight Time	UTC - 9 hours
	"HAST10HADT",

	"HNE",	// Heure Normale de l'Est	UTC - 5 hours
	"HAE",	// Heure Avancée de l'Est	UTC - 4 hours
	"HNE5HAE",

	"HNP",	// Heure Normale du Pacifique	UTC - 8 hours
	"HAP",	// Heure Avancée du Pacifique	UTC - 7 hours
	"HNP8HAP",

	"HNR",	// Heure Normale des Rocheuses	UTC - 7 hours
	"HAR",	// Heure Avancée des Rocheuses	UTC - 6 hours
	"HNR7HAR",

	"HNT",	// Heure Normale de Terre-Neuve	UTC - 3:30 hours
	"HAT",	// Heure Avancée de Terre-Neuve	UTC - 2:30 hours
	"HNT3:30HAT",

	"HNY",	// Heure Normale du Yukon	UTC - 9 hours
	"HAY",	// Heure Avancée du Yukon	UTC - 8 hours
	"HNY9HAY",

	"MST",	// Mountain Standard Time	UTC - 7 hours
	"MDT",	// Mountain Daylight Time	UTC - 6 hours
	"MST7MDT",

	"MEZ",	// Mitteleuropäische Zeit	UTC + 1 hour
	"MESZ",	// Mitteleuropäische Sommerzeit	UTC + 2 hours
	"MEZ-1MESZ"

	"NST",	// Newfoundland Standard Time	UTC - 3:30 hours
	"NDT",	// Newfoundland Daylight Time	UTC - 2:30 hours
	"NST3:30NDT",

	"PST",	// Pacific Standard Time	UTC - 8 hours
	"PDT",	// Pacific Daylight Time	UTC - 7 hours
	"PST8PDT",

	"WET",	// Western European Time	UTC
	"WEST",	// Western European Summer Time	UTC + 1 hour
	"WET-1WEST",

	"",
	"",
	""
};

static const int32_t	timezoneoffsets[]={

	34200,	// Australian Central Standard Time	UTC + 9:30 hours
	37800,	// Australian Central Daylight Time	UTC + 10:30 hours
	34200,

	-14400,	// Atlantic Standard Time		UTC - 4 hours
	-10800,	// Atlantic Daylight Time		UTC - 3 hours
	-14400,

	36000,	// Australian Eastern Standard Time	UTC + 10 hours
	39600,	// Australian Eastern Daylight Time	UTC + 11 hours
	36000,

	-32400,	// Alaska Standard Time	UTC - 9 hours
	-28800,	// Alaska Daylight Time	UTC - 8 hours
	-32400,

	-21600,	// Central Standard Time	UTC - 6 hours
	-18000,	// Central Daylight Time	UTC - 5 hours
	-21600,

	3600,	// Central European Time	UTC + 1 hour
	7200,	// Central European Summer Time	UTC + 2 hours
	3600,

	-18000,	// Eastern Standard Time	UTC - 5 hours
	-14400,	// Eastern Daylight Time	UTC - 4 hours
	-18000,

	7200,	// Eastern European Time	UTC + 2 hours
	10800,	// Eastern European Summer Time	UTC + 3 hours
	7200,

	0,	// Greenwich Mean Time	UTC
	3600,	// British Summer Time	UTC + 1 hour
	0,

	-14400,	// Heure Normale de l'Atlantique	UTC - 4 hours
	-10800,	// Heure Avancée de l'Atlantique	UTC - 3 hours
	-14400,

	-21600,	// Heure Normale du Centre		UTC - 6 hours
	-18000,	// Heure Avancée du Centre		UTC - 5 hours
	-21600,

	-36000,	// Hawaii-Aleutian Standard Time	UTC - 10 hours
	-32400,	// Hawaii-Aleutian Daylight Time	UTC - 9 hours
	-36000,

	-18000,	// Heure Normale de l'Est	UTC - 5 hours
	-14400,	// Heure Avancée de l'Est	UTC - 4 hours
	-18000,

	-28800,	// Heure Normale du Pacifique	UTC - 8 hours
	-25200,	// Heure Avancée du Pacifique	UTC - 7 hours
	-28800,

	-25200,	// Heure Normale des Rocheuses	UTC - 7 hours
	-21600,	// Heure Avancée des Rocheuses	UTC - 6 hours
	-25200,

	-12600,	// Heure Normale de Terre-Neuve	UTC - 3:30 hours
	-9000,	// Heure Avancée de Terre-Neuve	UTC - 2:30 hours
	-12600,

	-32400,	// Heure Normale du Yukon	UTC - 9 hours
	-28800,	// Heure Avancée du Yukon	UTC - 8 hours
	-32400,

	-25200,	// Mountain Standard Time	UTC - 7 hours
	-21600,	// Mountain Daylight Time	UTC - 6 hours
	-25200,

	3600,	// Mitteleuropäische Zeit	UTC + 1 hour
	7200,	// Mitteleuropäische Sommerzeit	UTC + 2 hours
	3600,

	-12600,	// Newfoundland Standard Time	UTC - 3:30 hours
	-9000,	// Newfoundland Daylight Time	UTC - 2:30 hours
	-12600,

	-28800,	// Pacific Standard Time	UTC - 8 hours
	-25200,	// Pacific Daylight Time	UTC - 7 hours
	-28800,

	0,	// Western European Time	UTC
	3600,	// Western European Summer Time	UTC + 1 hour
	0,

	0,
	0,
	0
};

const char * const *datetime::getTimeZoneAbbreviations() {
	return timezones;
}

const int32_t *datetime::getTimeZoneOffsets() {
	return timezoneoffsets;
}

// FIXME: this is kind of lame.  There must be a better way to do this than
// looking up values in a table embedded in the class.  I guess I could look
// through every zoneinfo file (on platforms that support them), but I've tried
// that before and I get multiple hits for a given zone.  If anyone reads this
// comment and knows the answer, please let me know.
const char *datetime::lookupCombinedTimeZone(const char *zn) const {

	// run through the list of timezones that observe daylight
	// savings time, if "zn" is in that list, return the
	// combined zone name, otherwise just return "zn"
	for (int index=0; timezones[index][0]; index=index+3) {
		if (!charstring::compare(zn,timezones[index]) ||
			!charstring::compare(zn,timezones[index+1])) {
			return timezones[index+2];
		}
	}
	return zn;
}

bool datetime::daylightZone(const char *zn) const {

	// run through the list of timezones that observe daylight
	// savings time, if "zn" is in that list, return true,
	// otherwise return false
	for (int index=0; timezones[index][0]; index=index+3) {
		if (!charstring::compare(zn,timezones[index+1])) {
			return true;
		}
	}
	return false;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
