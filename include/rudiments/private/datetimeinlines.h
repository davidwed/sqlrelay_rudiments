// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE datetime::~datetime() {
	delete[] timestring;
	delete timestruct;
}

RUDIMENTS_INLINE void datetime::initTimeString() {
	delete[] timestring;
	timestring=NULL;
}

RUDIMENTS_INLINE void datetime::initTimeStruct() {
	delete timestruct;
	timestruct=new struct tm;
}

RUDIMENTS_INLINE int datetime::getHour() const {
	return timestruct->tm_hour;
}

RUDIMENTS_INLINE int datetime::getMinutes() const {
	return timestruct->tm_min;
}

RUDIMENTS_INLINE int datetime::getSeconds() const {
	return timestruct->tm_sec;
}

RUDIMENTS_INLINE int datetime::getMonth() const {
	return timestruct->tm_mon+1;
}

RUDIMENTS_INLINE int datetime::getDayOfMonth() const {
	return timestruct->tm_mday;
}

RUDIMENTS_INLINE int datetime::getDayOfWeek() const {
	return timestruct->tm_wday;
}

RUDIMENTS_INLINE int datetime::getDayOfYear() const {
	return timestruct->tm_yday+1;
}

RUDIMENTS_INLINE int datetime::getYear() const {
	return timestruct->tm_year+1900;
}

RUDIMENTS_INLINE int datetime::isDaylightSavingsTime() const {
	return timestruct->tm_isdst;
}

RUDIMENTS_INLINE char	*datetime::getTimeZoneString() const {
	#ifdef HAS___TM_ZONE
		return (char *)timestruct->__tm_zone;
	#elif HAS_TM_ZONE
		return (char *)timestruct->tm_zone;
	#elif HAS_TM_NAME
		return (char *)timestruct->tm_name;
	#else
		return "";
	#endif
}

RUDIMENTS_INLINE char	*datetime::getTimeZoneString(const struct tm *tmstruct)
							const {
	#ifdef HAS___TM_ZONE
		return (char *)tmstruct->__tm_zone;
	#elif HAS_TM_ZONE
		return (char *)tmstruct->tm_zone;
	#elif HAS_TM_NAME
		return (char *)tmstruct->tm_name;
	#else
		return "";
	#endif
}

RUDIMENTS_INLINE long datetime::getTimeZoneOffset() const {
	#ifdef HAS___TM_GMTOFF
		return timestruct->__tm_gmtoff;
	#elif HAS_TM_GMTOFF
		return timestruct->tm_gmtoff;
	#elif HAS_TM_TZADJ
		return timestruct->tm_tzadj;
	#else
		return 0;
	#endif
}

RUDIMENTS_INLINE time_t datetime::getEpoch() const {
	return epoch;
}

RUDIMENTS_INLINE struct tm *datetime::getTm() {
	return timestruct;
}

RUDIMENTS_INLINE void datetime::addSeconds(int seconds) {
	timestruct->tm_sec=timestruct->tm_sec+seconds;
	updateTime();
}

RUDIMENTS_INLINE void datetime::addMinutes(int minutes) {
	timestruct->tm_min=timestruct->tm_min+minutes;
	updateTime();
}

RUDIMENTS_INLINE void datetime::addHours(int hours) {
	timestruct->tm_hour=timestruct->tm_hour+hours;
	updateTime();
}

RUDIMENTS_INLINE void datetime::addDays(int days) {
	timestruct->tm_hour=timestruct->tm_yday+days;
	updateTime();
}

RUDIMENTS_INLINE void datetime::addMonths(int months) {
	timestruct->tm_hour=timestruct->tm_mon+months;
	updateTime();
}

RUDIMENTS_INLINE void datetime::addYears(int years) {
	timestruct->tm_hour=timestruct->tm_year+years;
	updateTime();
}

RUDIMENTS_INLINE char *datetime::getString(time_t seconds) {
	datetime	dt;
	return ((dt.initialize(seconds))?strdup(dt.getString()):NULL);
}

RUDIMENTS_INLINE char *datetime::getString(const struct tm *tmstruct) {
	datetime	dt;
	return ((dt.initialize(tmstruct))?strdup(dt.getString()):NULL);
}

RUDIMENTS_INLINE time_t datetime::getEpoch(const char *datestring) {
	datetime	dt;
	return ((dt.initialize(datestring))?dt.getEpoch():-1);
}

RUDIMENTS_INLINE time_t datetime::getEpoch(const struct tm *tmstruct) {
	datetime	dt;
	return ((dt.initialize(tmstruct))?dt.getEpoch():-1);
}

RUDIMENTS_INLINE int datetime::setTimeZoneEnvVar(const char *zone,
							char **oldzone) {
	char	*tz=env.getValue("TZ");
	if (tz) {
		*oldzone=strdup(tz);
	} else {
		*oldzone=NULL;
	}
	return env.setValue("TZ",zone);
}

RUDIMENTS_INLINE int datetime::restoreTimeZoneEnvVar(const char *oldzone) {
	if (oldzone) {
		int	retval=env.setValue("TZ",oldzone);
		delete[] oldzone;
		return retval;
	}
	env.remove("TZ");
	return 1;
}
