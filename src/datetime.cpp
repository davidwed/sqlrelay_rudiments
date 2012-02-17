// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#ifdef RUDIMENTS_HAVE_RTC
	#include <rudiments/file.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef RUDIMENTS_HAVE_RTC
	#ifdef RUDIMENTS_HAVE_LINUX_RTC_H
		#include <linux/rtc.h>
	#endif
	#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
		#include <sys/ioctl.h>
	#endif
#endif

#ifdef RUDIMENTS_HAVE_OS_KERNEL_OS_H
	#include <os/kernel/OS.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class datetimeprivate {
	friend class datetime;
	private:
		#if defined(RUDIMENTS_HAVE_MKTIME)
			int32_t	_sec;
			int32_t	_min;
			int32_t	_hour;
			int32_t	_mday;
			int32_t	_mon;
			int32_t	_year;
			int32_t	_wday;
			int32_t	_yday;
			int32_t	_isdst;

			char	*_zone;
			int32_t	_gmtoff;

			char		*_timestring;
			struct tm	*_structtm;

			time_t	_epoch;

			#if defined(RUDIMENTS_HAS__GET_TZNAME)
				char	_timezonename[16];
			#endif

			environment	_env;
		#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
			SYSTEMTIME		_st;
			TIME_ZONE_INFORMATION	_tzi;
		#endif
};

static mutex	*_timemutex;

static const char _monthname[][10]={
	"January","February","March",
	"April","May","June",
	"July","August","September",
	"October","November","December"
};

static const char _monthabbr[][4]={
	"Jan","Feb","Mar",
	"Apr","May","Jun",
	"Jul","Aug","Sep",
	"Oct","Nov","Dec"
};

datetime::datetime() {
	pvt=new datetimeprivate;
	#if defined(RUDIMENTS_HAVE_MKTIME)
		pvt->_sec=0;
		pvt->_min=0;
		pvt->_hour=0;
		pvt->_mday=0;
		pvt->_mon=0;
		pvt->_year=0;
		pvt->_wday=0;
		pvt->_yday=0;
		pvt->_isdst=0;
		pvt->_zone=NULL;
		pvt->_gmtoff=0;
		pvt->_timestring=NULL;
		pvt->_structtm=NULL;
		pvt->_epoch=0;
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
		rawbuffer::zero(pvt->_st,sizeof(SYSTEMTIME));
		rawbuffer::zero(pvt->_tzi,sizeof(TIME_ZONE_INFORMATION));
	#endif
	_timemutex=NULL;
}

datetime::~datetime() {
	#if defined(RUDIMENTS_HAVE_MKTIME)
		delete[] pvt->_zone;
		delete[] pvt->_timestring;
		delete pvt->_structtm;
	#endif
	delete pvt;
}

bool datetime::initialize(const char *tmstring) {

	// get the date
	const char	*ptr=tmstring;
	#if defined(RUDIMENTS_HAVE_MKTIME)
	pvt->_mon=charstring::toInteger(ptr)-1;
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
	pvt->_st.wMonth=charstring::toShort(ptr);
	#endif
	ptr=charstring::findFirst(ptr,'/');
	if (!ptr || !ptr[0]) {
		return false;
	}
	ptr=ptr+sizeof(char);
	#if defined(RUDIMENTS_HAVE_MKTIME)
	pvt->_mday=charstring::toInteger(ptr);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
	pvt->_st.wDay=charstring::toShort(ptr);
	#endif
	ptr=charstring::findFirst(ptr,'/');
	if (!ptr || !ptr[0]) {
		return false;
	}
	ptr=ptr+sizeof(char);
	#if defined(RUDIMENTS_HAVE_MKTIME)
	pvt->_year=charstring::toInteger(ptr)-1900;
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
	pvt->_st.wYear=charstring::toShort(ptr);
	#endif

	// get the time
	ptr=charstring::findFirst(ptr,' ');
	if (!ptr || !ptr[0]) {
		return false;
	}
	ptr=ptr+sizeof(char);
	#if defined(RUDIMENTS_HAVE_MKTIME)
	pvt->_hour=charstring::toInteger(ptr);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
	pvt->_st.wHour=charstring::toShort(ptr);
	#endif
	ptr=charstring::findFirst(ptr,':');
	if (!ptr || !ptr[0]) {
		return false;
	}
	ptr=ptr+sizeof(char);
	#if defined(RUDIMENTS_HAVE_MKTIME)
	pvt->_min=charstring::toInteger(ptr);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
	pvt->_st.wMinute=charstring::toShort(ptr);
	#endif
	ptr=charstring::findFirst(ptr,':');
	if (!ptr || !ptr[0]) {
		return false;
	}
	ptr=ptr+sizeof(char);
	#if defined(RUDIMENTS_HAVE_MKTIME)
	pvt->_sec=charstring::toInteger(ptr);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIME)
	pvt->_st.wSecond=charstring::toShort(ptr);
	#endif

	// initialize the daylight savings time flag
	pvt->_isdst=-1;

	// get the time zone if it was provided
	delete[] pvt->_zone;
	if ((ptr=charstring::findFirst(ptr,' '))) {
		ptr=ptr+sizeof(char);
		pvt->_zone=(ptr && ptr[0])?charstring::duplicate(ptr):NULL;
	} else {
		pvt->_zone=NULL;
	}

	// normalize, string and epoch
	return normalizeBrokenDownTime(true);
}

bool datetime::initialize(time_t seconds) {
	pvt->_epoch=seconds;
	return getBrokenDownTimeFromEpoch(true);
}

bool datetime::initialize(const void *tmstruct) {

	const struct tm	*tms=(const struct tm *)tmstruct;

	pvt->_sec=tms->tm_sec;
	pvt->_min=tms->tm_min;
	pvt->_hour=tms->tm_hour;
	pvt->_mday=tms->tm_mday;
	pvt->_mon=tms->tm_mon;
	pvt->_year=tms->tm_year;
	pvt->_wday=tms->tm_wday;
	pvt->_yday=tms->tm_yday;
	pvt->_isdst=tms->tm_isdst;
	// FIXME: what if the zone/offset are garbage, is there a good way to
	// tell?
	#if defined(RUDIMENTS_HAS___TM_ZONE)
		pvt->_zone=charstring::duplicate(tms->__tm_zone);
	#elif defined(RUDIMENTS_HAS_TM_ZONE)
		pvt->_zone=charstring::duplicate(tms->tm_zone);
	#elif defined(RUDIMENTS_HAS_TM_NAME)
		pvt->_zone=charstring::duplicate(tms->tm_name);
	#else
		if (_timemutex && !acquireLock()) {
			return false;
		}
		#if defined(RUDIMENTS_HAS__TZSET)
			_tzset();
		#elif defined(RUDIMENTS_HAS_TZSET)
			tzset();
		#else
			#error no tzset or anything like it
		#endif
		const char	*tzn=getTzName(pvt->_isdst);
		pvt->_zone=charstring::duplicate((tzn && tzn[0])?tzn:"UCT");
		releaseLock();
	#endif

	#if defined(RUDIMENTS_HAS___TM_GMTOFF)
		pvt->_gmtoff=tms->__tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_GMTOFF)
		pvt->_gmtoff=tms->tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_TZADJ)
		pvt->_gmtoff=-tms->tm_tzadj;
	#else
		if (_timemutex && !acquireLock()) {
			return false;
		}
		#if defined(RUDIMENTS_HAS__TZSET)
			_tzset();
		#elif defined(RUDIMENTS_HAS_TZSET)
			tzset();
		#else
			#error no tzset or anything like it
		#endif
		#if defined(RUDIMENTS_HAS__GET_TIMEZONE)
			long	seconds;
			_get_timezone(&seconds);
			pvt->_gmtoff=seconds;
		#elif defined(RUDIMENTS_HAS_TIMEZONE)
			pvt->_gmtoff=-timezone;
		#elif defined(RUDIMENTS_HAS_TIMEZONE)
			pvt->_gmtoff=-_timezone;
		#else
			#error no timezone or anything like it
		#endif
		releaseLock();
	#endif
	return normalizeBrokenDownTime(true);
}

int32_t datetime::getHour() const {
	return pvt->_hour;
}

int32_t datetime::getMinutes() const {
	return pvt->_min;
}

int32_t datetime::getSeconds() const {
	return pvt->_sec;
}

int32_t datetime::getMonth() const {
	return pvt->_mon+1;
}

const char *datetime::getMonthName() const {
	return _monthname[pvt->_mon];
}

const char *datetime::getMonthAbbreviation() const {
	return _monthabbr[pvt->_mon];
}

int32_t datetime::getDayOfMonth() const {
	return pvt->_mday;
}

int32_t datetime::getDayOfWeek() const {
	return pvt->_wday+1;
}

int32_t datetime::getDayOfYear() const {
	return pvt->_yday+1;
}

int32_t datetime::getYear() const {
	return pvt->_year+1900;
}

bool datetime::isDaylightSavingsTime() const {
	return pvt->_isdst!=0;
}

const char *datetime::getTimeZoneString() const {
	return pvt->_zone;
}

int32_t datetime::getTimeZoneOffset() const {
	return pvt->_gmtoff;
}

time_t datetime::getEpoch() const {
	return pvt->_epoch;
}

const void *datetime::getInternalTimeStructure() {

	if (_timemutex && !acquireLock()) {
		return NULL;
	}

	delete pvt->_structtm;
	pvt->_structtm=new struct tm;
	pvt->_structtm->tm_mon=pvt->_mon;
	pvt->_structtm->tm_mday=pvt->_mday;
	pvt->_structtm->tm_year=pvt->_year;
	pvt->_structtm->tm_hour=pvt->_hour;
	pvt->_structtm->tm_min=pvt->_min;
	pvt->_structtm->tm_sec=pvt->_sec;
	pvt->_structtm->tm_isdst=pvt->_isdst;

	// FIXME: lookup combined zone here too
	char	*oldzone;
	if ((pvt->_zone && pvt->_zone[0] &&
			!setTimeZoneEnvVar(pvt->_zone,&oldzone,false)) ||
		(mktime(pvt->_structtm)==-1) ||
		(pvt->_zone && pvt->_zone[0] &&
			!restoreTimeZoneEnvVar(oldzone))) {
		return NULL;
	}

	releaseLock();
	return (void *)pvt->_structtm;
}

bool datetime::setSeconds(int32_t seconds) {
	pvt->_sec=seconds;
	return normalizeBrokenDownTime(true);
}

bool datetime::setMinutes(int32_t minutes) {
	pvt->_min=minutes;
	return normalizeBrokenDownTime(true);
}

bool datetime::setHours(int32_t hours) {
	pvt->_hour=hours;
	return normalizeBrokenDownTime(true);
}

bool datetime::setDays(int32_t days) {
	pvt->_yday=days;
	return normalizeBrokenDownTime(true);
}

bool datetime::setMonths(int32_t months) {
	pvt->_mon=months;
	return normalizeBrokenDownTime(true);
}

bool datetime::setYears(int32_t years) {
	pvt->_year=years;
	return normalizeBrokenDownTime(true);
}

bool datetime::addSeconds(int32_t seconds) {
	pvt->_sec=pvt->_sec+seconds;
	return normalizeBrokenDownTime(true);
}

bool datetime::addMinutes(int32_t minutes) {
	pvt->_min=pvt->_min+minutes;
	return normalizeBrokenDownTime(true);
}

bool datetime::addHours(int32_t hours) {
	pvt->_hour=pvt->_hour+hours;
	return normalizeBrokenDownTime(true);
}

bool datetime::addDays(int32_t days) {
	pvt->_yday=pvt->_yday+days;
	return normalizeBrokenDownTime(true);
}

bool datetime::addMonths(int32_t months) {
	pvt->_mon=pvt->_mon+months;
	return normalizeBrokenDownTime(true);
}

bool datetime::addYears(int32_t years) {
	pvt->_year=pvt->_year+years;
	return normalizeBrokenDownTime(true);
}

void datetime::setTimeMutex(mutex *mtx) {
	_timemutex=mtx;
}

const char *datetime::getString() {
	delete[] pvt->_timestring;
	stringbuffer	timestr;
	timestr.append(getMonth(),2)->append('/');
	timestr.append(getDayOfMonth(),2)->append('/');
	timestr.append(getYear())->append(' ');
	timestr.append(getHour(),2)->append(':');
	timestr.append(getMinutes(),2)->append(':');
	timestr.append(getSeconds())->append(' ');
	timestr.append(getTimeZoneString());
	pvt->_timestring=timestr.detachString();
	return pvt->_timestring;
}


bool datetime::getSystemDateAndTime() {
	return initialize(time(NULL));
}

bool datetime::setSystemDateAndTime() {
	// FIXME: should set /etc/localtime (or /etc/TZ) and TZ env var too...
	#if defined(RUDIMENTS_HAVE_SETTIMEOFDAY)
		timeval	tv;
		tv.tv_sec=pvt->_epoch;
		tv.tv_usec=0;
		return !settimeofday(&tv,NULL);
	#elif defined(RUDIMENTS_HAVE_SET_REAL_TIME_CLOCK)
		set_real_time_clock(pvt->_epoch);
		return true;
	#elif defined(RUDIMENTS_HAVE_SETSYSTEMTIME)
		SYSTEMTIME	st;
		st.wYear=pvt->_year+1900;
		st.wMonth=pvt->_mon+1;
		st.wDayOfWeek=pvt->_wday;
		st.wDay=pvt->_mday;
		st.wHour=pvt->_hour;
		st.wMinute=pvt->_min;
		st.wSecond=pvt->_sec;
		st.wMilliseconds=0;
		return SetSystemTime(&st)!=0;
	#else
		#error no settimeofday or anything like it
	#endif
}

bool datetime::getHardwareDateAndTime(const char *hwtz) {

	#ifdef RUDIMENTS_HAVE_RTC
		// open the rtc
		file	devrtc;
		if (!devrtc.open("/dev/rtc",O_RDONLY)) {
			return false;
		}

		// get the time from the rtc
		rtc_time rt;
		if (devrtc.ioCtl(RTC_RD_TIME,&rt)==-1) {
			devrtc.close();
			return false;
		}
		devrtc.close();

		// set the local values
		pvt->_mon=rt.tm_mon;
		pvt->_mday=rt.tm_mday;
		pvt->_year=rt.tm_year;
		pvt->_hour=rt.tm_hour;
		pvt->_min=rt.tm_min;
		pvt->_sec=rt.tm_sec;
		pvt->_isdst=rt.tm_isdst;
		pvt->_zone=charstring::duplicate(hwtz);

		return normalizeBrokenDownTime(true);
	#else
		return false;
	#endif
}

bool datetime::getAdjustedHardwareDateAndTime(const char *hwtz) {
	return (getHardwareDateAndTime(hwtz) && adjustTimeZone(NULL));
}

bool datetime::setHardwareDateAndTime(const char *hwtz) {

	#ifdef RUDIMENTS_HAVE_RTC
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
		rt.tm_mon=pvt->_mon;
		rt.tm_mday=pvt->_mday;
		rt.tm_year=pvt->_year;
		rt.tm_hour=pvt->_hour;
		rt.tm_min=pvt->_min;
		rt.tm_sec=pvt->_sec;
		rt.tm_wday=pvt->_wday;
		rt.tm_yday=pvt->_yday;
		rt.tm_isdst=pvt->_isdst;

		// set the rtc and clean up
		bool	retval=(devrtc.ioCtl(RTC_SET_TIME,&rt)!=-1);
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

	if (!acquireLock()) {
		return false;
	}

	// Clear out the zone so getBrokenDownTimeFromEpoch() won't try to
	// preserve it.
	delete[] pvt->_zone;
	pvt->_zone=NULL;

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

	releaseLock();
	return retval;
}

char *datetime::getString(time_t seconds) {
	datetime	dt;
	return ((dt.initialize(seconds))?
		charstring::duplicate(dt.getString()):NULL);
}

char *datetime::getString(const void *tmstruct) {
	datetime	dt;
	return ((dt.initialize(tmstruct))?
		charstring::duplicate(dt.getString()):NULL);
}

time_t datetime::getEpoch(const char *datestring) {
	datetime	dt;
	return ((dt.initialize(datestring))?dt.getEpoch():-1);
}

time_t datetime::getEpoch(const void *tmstruct) {
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

	const char	*tz=pvt->_env.getValue("TZ");
	if (tz) {
		*oldzone=charstring::duplicate(tz);
	} else {
		*oldzone=NULL;
	}
	return pvt->_env.setValue("TZ",realzone);
}

bool datetime::restoreTimeZoneEnvVar(const char *oldzone) {
	if (oldzone) {
		bool	retval=pvt->_env.setValue("TZ",oldzone);
		delete[] oldzone;
		return retval;
	}
	pvt->_env.remove("TZ");
	return true;
}

bool datetime::getBrokenDownTimeFromEpoch(bool needmutex) {

	#ifdef RUDIMENTS_HAVE_LOCALTIME_S
		bool	retval=false;
		struct tm	tms;
		if (!localtime_s(&tms,&pvt->_epoch)) {
			pvt->_sec=tms.tm_sec;
			pvt->_min=tms.tm_min;
			pvt->_hour=tms.tm_hour;
			pvt->_mday=tms.tm_mday;
			pvt->_mon=tms.tm_mon;
			pvt->_year=tms.tm_year;
			pvt->_isdst=tms.tm_isdst;
			retval=normalizeBrokenDownTime(false);
		}
		return retval;
	#else
		// I'm using localtime here instead of localtime_r because
		// localtime_r doesn't appear to handle the timezone properly,
		// at least, not in glibc-2.3
		if (needmutex && !acquireLock()) {
			return false;
		}
		bool	retval=false;
		struct tm	*tms;
		if ((tms=localtime(&pvt->_epoch))) {
			pvt->_sec=tms->tm_sec;
			pvt->_min=tms->tm_min;
			pvt->_hour=tms->tm_hour;
			pvt->_mday=tms->tm_mday;
			pvt->_mon=tms->tm_mon;
			pvt->_year=tms->tm_year;
			pvt->_isdst=tms->tm_isdst;
			retval=normalizeBrokenDownTime(false);
		}
		if (needmutex) {
			releaseLock();
		}
		return retval;
	#endif
}

bool datetime::normalizeBrokenDownTime(bool needmutex) {

	if (needmutex && !acquireLock()) {
		return false;
	}

	// If a time zone was passed in, use it.
	char	*oldzone=NULL;

	if (pvt->_zone && pvt->_zone[0] &&
			!setTimeZoneEnvVar(pvt->_zone,&oldzone,false)) {
		if (needmutex) {
			releaseLock();
		}
		return false;
	}

	bool	retval=true;

	// copy relevent values into a struct tm
	struct tm	tms;
	tms.tm_sec=pvt->_sec;
	tms.tm_min=pvt->_min;
	tms.tm_hour=pvt->_hour;
	tms.tm_mday=pvt->_mday;
	tms.tm_mon=pvt->_mon;
	tms.tm_year=pvt->_year;
	tms.tm_isdst=pvt->_isdst;

	// mktime() will get the epoch, set wday, yday, isdst
	// and normalize other values
	time_t	ep=mktime(&tms);
	if (ep==-1) {
		retval=false;
	} else {

		// store the epoch
		pvt->_epoch=ep;

		// copy values back out of struct tm
		pvt->_sec=tms.tm_sec;
		pvt->_min=tms.tm_min;
		pvt->_hour=tms.tm_hour;
		pvt->_mday=tms.tm_mday;
		pvt->_mon=tms.tm_mon;
		pvt->_year=tms.tm_year;
		pvt->_isdst=tms.tm_isdst;
		pvt->_wday=tms.tm_wday;
		pvt->_yday=tms.tm_yday;
	
		// Use tzset to get the timezone name
		#if defined(RUDIMENTS_HAS__TZSET)
			_tzset();
		#elif defined(RUDIMENTS_HAS_TZSET)
			tzset();
		#else
			#error no tzset or anything like it
		#endif
		delete[] pvt->_zone;
		pvt->_zone=charstring::duplicate(getTzName(pvt->_isdst));

		// Get the offset from the struct tm if we can, otherwise get
		// it from the value set by tzset()
		#if defined(RUDIMENTS_HAS___TM_GMTOFF)
			pvt->_gmtoff=tms.__tm_gmtoff;
		#elif defined(RUDIMENTS_HAS_TM_GMTOFF)
			pvt->_gmtoff=tms.tm_gmtoff;
		#elif defined(RUDIMENTS_HAS_TM_TZADJ)
			pvt->_gmtoff=-tms.tm_tzadj;
		#elif defined(RUDIMENTS_HAS__GET_TIMEZONE)
			long	seconds;
			_get_timezone(&seconds);
			pvt->_gmtoff=seconds;
		#elif defined(RUDIMENTS_HAS_TIMEZONE)
			pvt->_gmtoff=-timezone;
		#elif defined(RUDIMENTS_HAS__TIMEZONE)
			pvt->_gmtoff=-_timezone;
		#else
			#error no timezone or anything like it
		#endif
	}

	retval=(retval && restoreTimeZoneEnvVar(oldzone));

	if (needmutex) {
		releaseLock();
	}

	return retval;
}

bool datetime::acquireLock() {
	return !(_timemutex && !_timemutex->lock());
}

bool datetime::releaseLock() {
	return !(_timemutex && !_timemutex->unlock());
}

static const char * const _timezones[]={

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
	"MEZ-1MESZ",

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

static const int32_t	_timezoneoffsets[]={

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
	return _timezones;
}

const int32_t *datetime::getTimeZoneOffsets() {
	return _timezoneoffsets;
}

// FIXME: this is kind of lame.  There must be a better way to do this than
// looking up values in a table embedded in the class.  I guess I could look
// through every zoneinfo file (on platforms that support them), but I've tried
// that before and I get multiple hits for a given zone.  If anyone reads this
// comment and knows the answer, please let me know.
const char *datetime::lookupCombinedTimeZone(const char *zn) const {

	// if the zone name is longer than 4 chars, then it's a combined zone
	if (charstring::length(zn)>4) {
		return zn;
	}

	// run through the list of timezones that observe daylight
	// savings time, if "zn" is in that list, return the
	// combined zone name, otherwise just return "zn"
	for (int index=0; _timezones[index][0]; index=index+3) {
		if (!charstring::compare(zn,_timezones[index]) ||
			!charstring::compare(zn,_timezones[index+1])) {
			return _timezones[index+2];
		}
	}
	return zn;
}

bool datetime::daylightZone(const char *zn) const {

	// run through the list of timezones that observe daylight
	// savings time, if "zn" is in that list, return true,
	// otherwise return false
	for (int index=0; _timezones[index][0]; index=index+3) {
		if (!charstring::compare(zn,_timezones[index+1])) {
			return true;
		}
	}
	return false;
}

bool datetime::validDateTime(const char *string) {

	// must at least be 19 chars long (format: "00/00/0000 00:00:00")
	if (charstring::length(string)<19) {
		return false;
	}

	// truncate timezone
	char	*newstring=charstring::duplicate(string,19);

	// Initialize a new instance of datetime using the string, then
	// compare it to the string returned by the instance of datetime
	// (ignoring the timezone).
	// If they're the same then it was a valid date.
	datetime	dt;
	bool	result=(dt.initialize(newstring) &&
			!charstring::compare(newstring,dt.getString(),19));
	delete[] newstring;
	return result;
}

const char *datetime::getTzName(uint8_t index) {
	#if defined(RUDIMENTS_HAS__GET_TZNAME)
		size_t	timezonenamelen;
		_get_tzname(&timezonenamelen,
				pvt->_timezonename,
				sizeof(pvt->_timezonename),
				index);
		return pvt->_timezonename;
	#elif defined(RUDIMENTS_HAS_TZNAME)
		return tzname[index];
	#else
		#error no tzname or anything like it
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
