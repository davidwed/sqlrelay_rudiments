// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/datetime.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/error.h>
#if defined(RUDIMENTS_HAVE_RTC_SET_TIME) || \
		defined(RUDIMENTS_HAVE_RTC_GET_TIME)
	#include <rudiments/file.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	// SCO OSR 5.0.0 appears to need this extern "C"
	extern "C" {
	#include <sys/time.h>
	}
#endif
#if defined(RUDIMENTS_HAVE_RTC_SET_TIME) || \
		defined(RUDIMENTS_HAVE_RTC_GET_TIME)
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

class datetimeprivate {
	friend class datetime;
	private:
		int32_t	_usec;

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

		char	*_timestring;

		time_t	_epoch;

		#if defined(RUDIMENTS_HAS__GET_TZNAME)
			char	_timezonename[16];
		#endif
};

static threadmutex	*_timemutex=NULL;

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
	init();
}

datetime::~datetime() {
	clear();
	delete pvt;
}

void datetime::init() {
	pvt->_usec=0;
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
	pvt->_epoch=0;
}

void datetime::clear() {
	delete[] pvt->_zone;
	delete[] pvt->_timestring;
}

bool datetime::initialize(const char *tmstring) {

	clear();
	init();

	// parse out the date
	const char	*ptr=tmstring;
	pvt->_mon=charstring::toInteger(ptr)-1;
	ptr=charstring::findFirst(ptr,'/');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_mday=charstring::toInteger(ptr);
	ptr=charstring::findFirst(ptr,'/');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_year=charstring::toInteger(ptr)-1900;

	// parse out the time
	ptr=charstring::findFirst(ptr,' ');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_hour=charstring::toInteger(ptr);
	ptr=charstring::findFirst(ptr,':');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_min=charstring::toInteger(ptr);
	ptr=charstring::findFirst(ptr,':');
	if (charstring::isNullOrEmpty(ptr)) {
		return false;
	}
	ptr=ptr+sizeof(char);
	pvt->_sec=charstring::toInteger(ptr);

	// parse out microseconds, if provided
	pvt->_usec=0;
	const char *usecptr=charstring::findFirst(ptr,':');
	if (usecptr) {
		pvt->_usec=charstring::toInteger(usecptr+1);
	}

	// initialize the daylight savings time flag
	pvt->_isdst=-1;

	// parse out the time zone if it was provided
	ptr=charstring::findFirst(ptr,' ');
	if (ptr) {
		ptr=ptr+sizeof(char);
		pvt->_zone=(ptr && ptr[0])?charstring::duplicate(ptr):NULL;
	} else {
		pvt->_zone=NULL;
	}

	// normalize
	return normalize();
}

bool datetime::initialize(time_t seconds) {
	return initialize(seconds,0);
}

bool datetime::initialize(time_t seconds, time_t microseconds) {

	clear();
	init();

	pvt->_epoch=seconds;
	pvt->_usec=microseconds;
	if (!acquireLock()) {
		return false;
	}
	bool	retval=getBrokenDownTimeFromEpoch();
	releaseLock();
	return retval;
}

bool datetime::initialize(const void *tmstruct) {

	clear();
	init();

	const struct tm	*tms=(const struct tm *)tmstruct;

	pvt->_usec=0;

	pvt->_sec=tms->tm_sec;
	pvt->_min=tms->tm_min;
	pvt->_hour=tms->tm_hour;
	pvt->_mday=tms->tm_mday;
	pvt->_mon=tms->tm_mon;
	pvt->_year=tms->tm_year;
	pvt->_wday=tms->tm_wday;
	pvt->_yday=tms->tm_yday;
	pvt->_isdst=tms->tm_isdst;

	#if defined(RUDIMENTS_HAS___TM_ZONE)
		pvt->_zone=charstring::duplicate(tms->__tm_zone);
	#elif defined(RUDIMENTS_HAS_TM_ZONE)
		pvt->_zone=charstring::duplicate(tms->tm_zone);
	#elif defined(RUDIMENTS_HAS_TM_NAME)
		pvt->_zone=charstring::duplicate(tms->tm_name);
	#else
		if (!acquireLock()) {
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
		#if defined(RUDIMENTS_HAS__GET_TIMEZONE)
			long	seconds;
			_get_timezone(&seconds);
			pvt->_gmtoff=-seconds;
		#elif defined(RUDIMENTS_HAS_TIMEZONE)
			pvt->_gmtoff=-timezone;
		#elif defined(RUDIMENTS_HAS_TIMEZONE)
			pvt->_gmtoff=-_timezone;
		#else
			#error no timezone or anything like it
		#endif
		releaseLock();
	#endif

	#if defined(RUDIMENTS_HAS___TM_GMTOFF)
		pvt->_gmtoff=tms->__tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_GMTOFF)
		pvt->_gmtoff=tms->tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_TZADJ)
		pvt->_gmtoff=-tms->tm_tzadj;
	#endif

	return normalize();
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

int32_t datetime::getMicroseconds() const {
	return pvt->_usec;
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

bool datetime::setMicroseconds(int32_t microseconds) {
	pvt->_usec=microseconds;
	return normalize();
}

bool datetime::setSeconds(int32_t seconds) {
	pvt->_sec=seconds;
	return normalize();
}

bool datetime::setMinutes(int32_t minutes) {
	pvt->_min=minutes;
	return normalize();
}

bool datetime::setHours(int32_t hours) {
	pvt->_hour=hours;
	return normalize();
}

bool datetime::setDays(int32_t days) {
	pvt->_yday=days;
	return normalize();
}

bool datetime::setMonths(int32_t months) {
	pvt->_mon=months;
	return normalize();
}

bool datetime::setYears(int32_t years) {
	pvt->_year=years;
	return normalize();
}

bool datetime::addMicroseconds(int32_t microseconds) {
	pvt->_usec=pvt->_usec+microseconds;
	return normalize();
}

bool datetime::addSeconds(int32_t seconds) {
	pvt->_sec=pvt->_sec+seconds;
	return normalize();
}

bool datetime::addMinutes(int32_t minutes) {
	pvt->_min=pvt->_min+minutes;
	return normalize();
}

bool datetime::addHours(int32_t hours) {
	pvt->_hour=pvt->_hour+hours;
	return normalize();
}

bool datetime::addDays(int32_t days) {
	pvt->_yday=pvt->_yday+days;
	return normalize();
}

bool datetime::addMonths(int32_t months) {
	pvt->_mon=pvt->_mon+months;
	return normalize();
}

bool datetime::addYears(int32_t years) {
	pvt->_year=pvt->_year+years;
	return normalize();
}

void datetime::setTimeMutex(threadmutex *mtx) {
	_timemutex=mtx;
}

const char *datetime::getString() {
	return getString(false);
}

const char *datetime::getString(bool microseconds) {
	delete[] pvt->_timestring;
	stringbuffer	timestr;
	timestr.append(getMonth(),2)->append('/');
	timestr.append(getDayOfMonth(),2)->append('/');
	timestr.append(getYear())->append(' ');
	timestr.append(getHour(),2)->append(':');
	timestr.append(getMinutes(),2)->append(':');
	timestr.append(getSeconds(),2);
	if (microseconds) {
		timestr.append(':')->append(getMicroseconds(),3);
	}
	timestr.append(' ');
	timestr.append(getTimeZoneString());
	pvt->_timestring=timestr.detachString();
	return pvt->_timestring;
}

bool datetime::getSystemDateAndTime() {
	#if defined(RUDIMENTS_HAVE_GETTIMEOFDAY)
		struct timeval	tv;
		if (gettimeofday(&tv,NULL)) {
			return false;
		}
		return initialize(tv.tv_sec,tv.tv_usec);
	#elif defined(RUDIMENTS_HAVE_GETSYSTEMTIMEASFILETIME)

		#ifdef RUDIMENTS_HAVE_LONG_LONG
			// FILETIME contains the number of 100 nanosecond
			// intervals since Jan 1, 1601 UTC.
			FILETIME	ft;
			GetSystemTimeAsFileTime(&ft);

			// convert to a single 64-bit number
			uint64_t	t=ft.dwHighDateTime;
			t<<=32;
			t|=ft.dwLowDateTime;

			// convert to microseconds
			t/=10;

			// subtract microseconds between 1601 and 1970
			t-=11644473600000000ULL;

			return initialize(t/1000000,t%1000000);
		#else
			SYSTEMTIME	st;
			GetLocalTime(&st);

			stringbuffer	str;
			str.append(st.wMonth)->append('/');
			str.append(st.wDay)->append('/');
			str.append(st.wYear)->append(' ');
			str.append(st.wHour)->append(':');
			str.append(st.wMinute)->append(':');
			str.append(st.wSecond)->append('.');
			str.append(st.wMilliseconds*1000);

			return initialize(str.getString());
		#endif
	#else
		return initialize(time(NULL));
	#endif
}

bool datetime::setSystemDateAndTime() {
	// FIXME: should set /etc/localtime (or /etc/TZ) and TZ env var too...
	#if defined(RUDIMENTS_HAVE_SETSYSTEMTIME)
		SYSTEMTIME	st;
		st.wYear=pvt->_year+1900;
		st.wMonth=pvt->_mon+1;
		st.wDayOfWeek=pvt->_wday;
		st.wDay=pvt->_mday;
		st.wHour=pvt->_hour;
		st.wMinute=pvt->_min;
		st.wSecond=pvt->_sec;
		st.wMilliseconds=pvt->_usec/1000;
		return SetSystemTime(&st)!=0;
	#elif defined(RUDIMENTS_HAVE_SETTIMEOFDAY)
		timeval	tv;
		tv.tv_sec=pvt->_epoch;
		tv.tv_usec=pvt->_usec;
		return !settimeofday(&tv,NULL);
	#elif defined(RUDIMENTS_HAVE_CLOCK_SETTIME)
		timespec	ts;
		ts.tv_sec=pvt->_epoch;
		ts.tv_nsec=pvt->_usec*1000;
		return !clock_settime(CLOCK_REALTIME,&ts);
	#elif defined(RUDIMENTS_HAVE_SET_REAL_TIME_CLOCK)
		set_real_time_clock(pvt->_epoch);
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool datetime::getHardwareDateAndTime(const char *hwtz) {

	#ifdef RUDIMENTS_HAVE_RTC_SET_TIME
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
		pvt->_usec=0;
		pvt->_isdst=rt.tm_isdst;
		pvt->_zone=charstring::duplicate(hwtz);

		return normalize();
	#else
		return false;
	#endif
}

bool datetime::getAdjustedHardwareDateAndTime(const char *hwtz) {
	return (getHardwareDateAndTime(hwtz) && adjustTimeZone(NULL));
}

bool datetime::setHardwareDateAndTime(const char *hwtz) {

	#ifdef RUDIMENTS_HAVE_RTC_SET_TIME
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

	// Change the time zone, get the broken down time relative to the
	// current epoch, in the new time zone.
	char	*oldzone=NULL;
	bool	retval=true;
	if (!charstring::isNullOrEmpty(newtz)) {
		retval=setTZ(newtz,&oldzone,ignoredst);
	}
	if (retval) {
		retval=getBrokenDownTimeFromEpoch();
	}
	if (!charstring::isNullOrEmpty(newtz)) {
		retval=restoreTZ(oldzone);
	}

	releaseLock();
	return retval;
}

char *datetime::getString(time_t seconds) {
	datetime	dt;
	return ((dt.initialize(seconds))?
		charstring::duplicate(dt.getString()):NULL);
}

char *datetime::getString(time_t seconds, time_t microseconds) {
	datetime	dt;
	return ((dt.initialize(seconds,microseconds))?
		charstring::duplicate(dt.getString(true)):NULL);
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

bool datetime::setTZ(const char *zone, char **oldzone, bool ignoredst) {

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

	const char	*tz=environment::getValue("TZ");
	if (tz) {
		*oldzone=charstring::duplicate(tz);
	} else {
		*oldzone=NULL;
	}
	return environment::setValue("TZ",realzone);
}

bool datetime::restoreTZ(char *oldzone) {
	if (oldzone) {
		bool	retval=environment::setValue("TZ",oldzone);
		delete[] oldzone;
		return retval;
	}
	environment::remove("TZ");
	return true;
}

bool datetime::getBrokenDownTimeFromEpoch() {

	struct tm	*tms=NULL;
	#ifdef RUDIMENTS_HAVE_LOCALTIME_S
		// localtime_s appears to rely on one or more of:
		// _daylight, _timezone or _tzname rather than just on TZ,
		// so we have to call _tzset() before calling localtime_s.
		_tzset();
		struct tm	tm;
		if (localtime_s(&tm,&pvt->_epoch)) {
			return false;
		}
		tms=&tm;
	#else
		// I'm using localtime here instead of localtime_r because
		// localtime_r doesn't appear to handle the timezone properly,
		// at least, not in glibc-2.3
		tms=localtime(&pvt->_epoch);
		if (!tms) {
			return false;
		}
	#endif
	pvt->_epoch=mktime(tms);
	pvt->_sec=tms->tm_sec;
	pvt->_min=tms->tm_min;
	pvt->_hour=tms->tm_hour;
	pvt->_mday=tms->tm_mday;
	pvt->_mon=tms->tm_mon;
	pvt->_year=tms->tm_year;
	pvt->_isdst=tms->tm_isdst;
	pvt->_wday=tms->tm_wday;
	pvt->_yday=tms->tm_yday;
	processTZ((void *)tms);
	return (pvt->_epoch!=-1);
}

bool datetime::normalize() {

	if (!acquireLock()) {
		return false;
	}

	// If a time zone is set then use it
	char	*oldzone=NULL;
	if (!charstring::isNullOrEmpty(pvt->_zone) &&
			!setTZ(pvt->_zone,&oldzone,false)) {
		releaseLock();
		return false;
	}

	// normalize microseconds
	pvt->_sec+=pvt->_usec/1000000;
	pvt->_usec%=1000000;

	// copy relevent values into a struct tm
	struct tm	tms;
	tms.tm_sec=pvt->_sec;
	tms.tm_min=pvt->_min;
	tms.tm_hour=pvt->_hour;
	tms.tm_mday=pvt->_mday;
	tms.tm_mon=pvt->_mon;
	tms.tm_year=pvt->_year;
	tms.tm_isdst=pvt->_isdst;

	// call mktime() to get the epoch, set wday, yday and isdst
	// and normalize other values
	pvt->_epoch=mktime(&tms);

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
	processTZ((void *)&tms);

	bool	retval=(pvt->_epoch!=-1);

	restoreTZ(oldzone);

	releaseLock();

	return retval;
}

void datetime::processTZ(void *tms) {

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
		pvt->_gmtoff=((struct tm *)tms)->__tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_GMTOFF)
		pvt->_gmtoff=((struct tm *)tms)->tm_gmtoff;
	#elif defined(RUDIMENTS_HAS_TM_TZADJ)
		pvt->_gmtoff=-((struct tm *)tms)->tm_tzadj;
	#elif defined(RUDIMENTS_HAS__GET_TIMEZONE)
		long	seconds;
		_get_timezone(&seconds);
		pvt->_gmtoff=-seconds;
	#elif defined(RUDIMENTS_HAS_TIMEZONE)
		pvt->_gmtoff=-timezone;
	#elif defined(RUDIMENTS_HAS__TIMEZONE)
		pvt->_gmtoff=-_timezone;
	#else
		#error no timezone or anything like it
	#endif
}

const char *datetime::getTzName(uint8_t index) {
	#if defined(RUDIMENTS_HAS__GET_TZNAME)
		size_t	timezonenamelen;
		_get_tzname(&timezonenamelen,
				pvt->_timezonename,
				sizeof(pvt->_timezonename),
				index);
		return pvt->_timezonename;
	#elif defined(RUDIMENTS_HAS__TZNAME)
		return _tzname[index];
	#elif defined(RUDIMENTS_HAS_TZNAME)
		return tzname[index];
	#else
		#error no tzname or anything like it
	#endif
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
