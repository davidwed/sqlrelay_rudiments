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

RUDIMENTS_INLINE char	*datetime::getTimeZone() const {
	#ifdef HAS___TM_ZONE
		return (char *)timestruct.__tm_zone;
	#elif HAS_TM_ZONE
		return (char *)timestruct.tm_zone;
	#elif HAS_TM_NAME
		return timestruct.tm_name;
	#else
		return "";
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

RUDIMENTS_INLINE char *datetime::getString(time_t epoch) {
	datetime	dt(epoch);
	return strdup(dt.getString());
}

RUDIMENTS_INLINE time_t datetime::getEpoch(const char *datestring) {
	datetime	dt(datestring);
	return dt.getEpoch();
}
