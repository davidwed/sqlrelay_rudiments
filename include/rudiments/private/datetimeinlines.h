// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE datetime::datetime() {
	timestruct=NULL;
	timestring=NULL;
	epoch=0;
}

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

RUDIMENTS_INLINE bool datetime::isDaylightSavingsTime() const {
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
	updateTimePreservingTimeZone();
}

RUDIMENTS_INLINE void datetime::addMinutes(int minutes) {
	timestruct->tm_min=timestruct->tm_min+minutes;
	updateTimePreservingTimeZone();
}

RUDIMENTS_INLINE void datetime::addHours(int hours) {
	timestruct->tm_hour=timestruct->tm_hour+hours;
	updateTimePreservingTimeZone();
}

RUDIMENTS_INLINE void datetime::addDays(int days) {
	timestruct->tm_hour=timestruct->tm_yday+days;
	updateTimePreservingTimeZone();
}

RUDIMENTS_INLINE void datetime::addMonths(int months) {
	timestruct->tm_hour=timestruct->tm_mon+months;
	updateTimePreservingTimeZone();
}

RUDIMENTS_INLINE void datetime::addYears(int years) {
	timestruct->tm_hour=timestruct->tm_year+years;
	updateTimePreservingTimeZone();
}

RUDIMENTS_INLINE void datetime::setLocalTimeMutex(pthread_mutex_t *mutex) {
	ltmutex=mutex;
}

RUDIMENTS_INLINE void datetime::setEnvironmentMutex(pthread_mutex_t *mutex) {
	envmutex=mutex;
}

RUDIMENTS_INLINE bool datetime::updateTime() {
	return ((epoch=mktime(timestruct))!=-1);
}
