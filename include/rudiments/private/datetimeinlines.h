// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <string.h>
#ifdef HAVE_STRINGS
	#include <strings.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE datetime::~datetime() {
	delete[] timestring;
}

RUDIMENTS_INLINE void datetime::initTimeString() {
	delete[] timestring;
	timestring=NULL;
}

RUDIMENTS_INLINE int datetime::getHour() const {
	return timestruct.tm_hour;
}

RUDIMENTS_INLINE int datetime::getMinutes() const {
	return timestruct.tm_min;
}

RUDIMENTS_INLINE int datetime::getSeconds() const {
	return timestruct.tm_sec;
}

RUDIMENTS_INLINE int datetime::getMonth() const {
	return timestruct.tm_mon+1;
}

RUDIMENTS_INLINE int datetime::getDayOfMonth() const {
	return timestruct.tm_mday;
}

RUDIMENTS_INLINE int datetime::getDayOfWeek() const {
	return timestruct.tm_wday;
}

RUDIMENTS_INLINE int datetime::getDayOfYear() const {
	return timestruct.tm_yday+1;
}

RUDIMENTS_INLINE int datetime::getYear() const {
	return timestruct.tm_year+1900;
}

RUDIMENTS_INLINE int datetime::isDaylightSavingsTime() const {
	return timestruct.tm_isdst;
}

RUDIMENTS_INLINE datetime::tzone datetime::getTimeZone() const {
	return timezone;
}

RUDIMENTS_INLINE char	*datetime::getTimeZoneString() const {
	#ifdef HAS___TM_ZONE
		return (char *)timestruct.__tm_zone;
	#elif HAS_TM_ZONE
		return (char *)timestruct.tm_zone;
	#elif HAS_TM_NAME
		return (char *)timestruct.tm_name;
	#else
		return "";
	#endif
}

RUDIMENTS_INLINE void datetime::setTimeZone(tzone tz) {
	timezone=tz;
	#ifdef HAS___TM_ZONE
		timestruct.__tm_zone=tzonestring[tz];
	#elif HAS_TM_ZONE
		timestruct.tm_zone=tzonestring[tz];
	#elif HAS_TM_NAME
		timestruct.tm_name=tzonestring[tz];
	#endif
	#ifdef HAS___TM_GMTOFF
		timestruct.__tm_gmtoff=tzoneoffset[tz];
	#elif HAS_TM_GMTOFF
		timestruct.tm_gmtoff=tzoneoffset[tz];
	#elif HAS_TM_TZADJ
		timestruct.tm_tzadj=tzoneoffset[tz];
	#endif
}

RUDIMENTS_INLINE long datetime::getTimeZoneOffset() const {
	#ifdef HAS___TM_GMTOFF
		return timestruct.__tm_gmtoff;
	#elif HAS_TM_GMTOFF
		return timestruct.tm_gmtoff;
	#elif HAS_TM_TZADJ
		return timestruct.tm_tzadj;
	#else
		return 0;
	#endif
}

RUDIMENTS_INLINE time_t datetime::getEpoch() const {
	return epoch;
}

RUDIMENTS_INLINE tm *datetime::getTm() {
	return &timestruct;
}

RUDIMENTS_INLINE void datetime::addSeconds(int seconds) {
	timestruct.tm_sec=timestruct.tm_sec+seconds;
	epoch=mktime(&timestruct);
}

RUDIMENTS_INLINE void datetime::addMinutes(int minutes) {
	timestruct.tm_min=timestruct.tm_min+minutes;
	epoch=mktime(&timestruct);
}

RUDIMENTS_INLINE void datetime::addHours(int hours) {
	timestruct.tm_hour=timestruct.tm_hour+hours;
	epoch=mktime(&timestruct);
}

RUDIMENTS_INLINE void datetime::addDays(int days) {
	timestruct.tm_hour=timestruct.tm_yday+days;
	epoch=mktime(&timestruct);
}

RUDIMENTS_INLINE void datetime::addMonths(int months) {
	timestruct.tm_hour=timestruct.tm_mon+months;
	epoch=mktime(&timestruct);
}

RUDIMENTS_INLINE void datetime::addYears(int years) {
	timestruct.tm_hour=timestruct.tm_year+years;
	epoch=mktime(&timestruct);
}

RUDIMENTS_INLINE char *datetime::getString(time_t epoch) {
	datetime	dt;
	return ((dt.initialize(epoch))?strdup(dt.getString()):NULL);
}

RUDIMENTS_INLINE char *datetime::getString(const tm *timestruct) {
	datetime	dt;
	return ((dt.initialize(timestruct))?strdup(dt.getString()):NULL);
}

RUDIMENTS_INLINE time_t datetime::getEpoch(const char *datestring) {
	datetime	dt;
	return ((dt.initialize(datestring))?dt.getEpoch():-1);
}

RUDIMENTS_INLINE time_t datetime::getEpoch(const tm *timestruct) {
	datetime	dt;
	return ((dt.initialize(timestruct))?dt.getEpoch():-1);
}

RUDIMENTS_INLINE char **datetime::getTimeZoneAbbreviations() {
	return (char **)tzonestring;
}

RUDIMENTS_INLINE long *datetime::getTimeZoneOffsets() {
	return (long *)tzoneoffset;
}
