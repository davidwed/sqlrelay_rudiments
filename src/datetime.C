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
	// FIXME: assumes GMT unless specified in environment
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

char *datetime::getMonthName() const {
	return (char *)monthname[mon];
}

char *datetime::getMonthAbbreviation() const {
	return (char *)monthabbr[mon];
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

char *datetime::getTimeZoneString() const {
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
	// FIXME: assumes GMT unless specified in environment
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
	return env.setValue("TZ",lookupActualTimeZone(zone));
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
printf("normalizeBrokenDownTime()\n");

	#ifdef RUDIMENTS_HAS_THREADS
		if (needmutex && !acquireLock()) {
			return false;
		}
	#endif

printf("zone=%s\n",zone);
	// If a time zone was passed in, use it.
	char	*oldzone=NULL;
printf("set TZ...\n");
	if (zone && zone[0] && !setTimeZoneEnvVar(zone,&oldzone)) {
		#ifdef RUDIMENTS_HAS_THREADS
			if (needmutex) {
				releaseLock();
			}
		#endif
		return false;
	}
environment	env;
printf("TZ=%s\n",env.getValue("TZ"));

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
printf("mktime()...\n");
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
printf("tzset()...\n");
		tzset();
		delete[] zone;
		zone=charstring::duplicate(tzname[isdst]);
printf("isdst=%d tzname[0]=%s  tzname[1]=%s  zone=%s\n",isdst,tzname[0],tzname[1],zone);

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

printf("restore TZ...\n");
	if (oldzone) {
		retval=(retval && restoreTimeZoneEnvVar(oldzone));
	}
printf("TZ=%s\n",env.getValue("TZ"));

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

static char timezones[][15]={

	"A",	// Alpha Time Zone	UTC + 1 hour
	"A",	// Alpha Time Zone	UTC + 1 hour
	"A",

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

	"AWST",	// Australian Western Standard Time	UTC + 8 hours
	"AWST",	// Australian Western Standard Time	UTC + 8 hours
	"AWST",

	"B",	// Bravo Time Zone	UTC + 2 hours
	"B",	// Bravo Time Zone	UTC + 2 hours
	"B",

	"C",	// Charlie Time Zone	UTC + 3 hours
	"C",	// Charlie Time Zone	UTC + 3 hours
	"C",

	"CST",	// Central Standard Time	UTC - 6 hours
	"CDT",	// Central Daylight Time	UTC - 5 hours
	"CST6CDT",

	"CET",	// Central European Time	UTC + 1 hour
	"CEST",	// Central European Summer Time	UTC + 2 hours
	"CET-1CST",

	"CXT",	// Christmas Island Time	UTC + 7 hours
	"CXT",	// Christmas Island Time	UTC + 7 hours
	"CXT",

	"D",	// Delta Time Zone	UTC + 4 hours
	"D",	// Delta Time Zone	UTC + 4 hours
	"D",

	"E",	// Echo Time Zone	UTC + 5 hours
	"E",	// Echo Time Zone	UTC + 5 hours
	"E",

	"EST",	// Eastern Standard Time	UTC - 5 hours
	"EDT",	// Eastern Daylight Time	UTC - 4 hours
	"EST5EDT",

	"EET",	// Eastern European Time	UTC + 2 hours
	"EEST",	// Eastern European Summer Time	UTC + 3 hours
	"EET-2EEST",

	"F",	// Foxtrot Time Zone	UTC + 6 hours
	"F",	// Foxtrot Time Zone	UTC + 6 hours
	"F",

	"G",	// Golf Time Zone	UTC + 7 hours
	"G",	// Golf Time Zone	UTC + 7 hours
	"G",

	"GMT",	// Greenwich Mean Time	UTC
	"BST",	// British Summer Time	UTC + 1 hour
	"GMT0BST",

	"H",	// Hotel Time Zone	UTC + 8 hours
	"H",	// Hotel Time Zone	UTC + 8 hours
	"H",

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

	"I",	// India Time Zone	UTC + 9 hours
	"I",	// India Time Zone	UTC + 9 hours
	"I",

	"IST",	// Irish Standard Time	UTC + 0 hours
	"IST",	// Irish Standard Time	UTC + 0 hours
	"IST",

	"K",	// Kilo Time Zone	UTC + 10 hours
	"K",	// Kilo Time Zone	UTC + 10 hours
	"K",

	"L",	// Lima Time Zone	UTC + 11 hours
	"L",	// Lima Time Zone	UTC + 11 hours
	"L",

	"M",	// Mike Time Zone	UTC + 12 hours
	"M",	// Mike Time Zone	UTC + 12 hours
	"M",

	"MST",	// Mountain Standard Time	UTC - 7 hours
	"MDT",	// Mountain Daylight Time	UTC - 6 hours
	"MST7MDT",

	"MEZ",	// Mitteleuropäische Zeit	UTC + 1 hour
	"MESZ",	// Mitteleuropäische Sommerzeit	UTC + 2 hours
	"MEZ-1MESZ"

	"N",	// November Time Zone		UTC - 1 hour
	"N",	// November Time Zone		UTC - 1 hour
	"N",

	"NST",	// Newfoundland Standard Time	UTC - 3:30 hours
	"NDT",	// Newfoundland Daylight Time	UTC - 2:30 hours
	"NST3:30NDT",

	"NFT",	// Norfolk (Island) Time	UTC + 11:30 hours
	"NFT",	// Norfolk (Island) Time	UTC + 11:30 hours
	"NFT",

	"O",	// Oscar Time Zone		UTC - 2 hours
	"O",	// Oscar Time Zone		UTC - 2 hours
	"O",

	"P",	// Papa Time Zone		UTC - 3 hours
	"P",	// Papa Time Zone		UTC - 3 hours
	"P",

	"PST",	// Pacific Standard Time	UTC - 8 hours
	"PDT",	// Pacific Daylight Time	UTC - 7 hours
	"PST8PDT",

	"Q",	// Quebec Time Zone	UTC - 4 hours
	"Q",	// Quebec Time Zone	UTC - 4 hours
	"Q",

	"R",	// Romeo Time Zone	UTC - 5 hours
	"R",	// Romeo Time Zone	UTC - 5 hours
	"R",

	"S",	// Sierra Time Zone	UTC - 6 hours
	"S",	// Sierra Time Zone	UTC - 6 hours
	"S",

	"T",	// Tango Time Zone	UTC - 7 hours
	"T",	// Tango Time Zone	UTC - 7 hours
	"T",

	"U",	// Uniform Time Zone	UTC - 8 hours
	"U",	// Uniform Time Zone	UTC - 8 hours
	"U",

	"UTC",	// Coordinated Universal Time	UTC
	"UTC",	// Coordinated Universal Time	UTC
	"UTC",

	"V",	// Victor Time Zone	UTC - 9 hours
	"V",	// Victor Time Zone	UTC - 9 hours
	"V",

	"W",	// Whiskey Time Zone	UTC - 10 hours
	"W",	// Whiskey Time Zone	UTC - 10 hours
	"W",

	"WET",	// Western European Time	UTC
	"WEST",	// Western European Summer Time	UTC + 1 hour
	"WET-1WEST",

	"WST",	// Western Standard Time	UTC + 8 hours
	"WST",	// Western Standard Time	UTC + 8 hours
	"WST",

	"X",	// X-ray Time Zone	UTC - 11 hours
	"X",	// X-ray Time Zone	UTC - 11 hours
	"X",

	"Y",	// Yankee Time Zone	UTC - 12 hours
	"Y",	// Yankee Time Zone	UTC - 12 hours
	"Y",

	"Z",	// Zulu Time Zone	UTC
	"Z",	// Zulu Time Zone	UTC
	"Z",

	"",
	"",
	""
};

// FIXME: this is kind of lame.  There must be a better way to do this than
// looking up values in a table embedded in the class.  I guess I could look
// through every zoneinfo file (on platforms that support them), but I've tried
// that before and I get multiple hits for a given zone.  If anyone reads this
// comment and knows the answer, please let me know.
char *datetime::lookupActualTimeZone(const char *zn) {
	for (int index=0; timezones[index][0]; index=index+3) {
		if (!charstring::compare(zn,timezones[index]) ||
			!charstring::compare(zn,timezones[index+1])) {
			return timezones[index+2];
		}
	}
	return NULL;
}
