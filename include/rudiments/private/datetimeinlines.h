// Copyright (c) 2002 David Muse
// See the COPYING file for more information

inline datetime::~datetime() {
	delete[] timestring;
}

inline int datetime::getHour() const {
	return timestruct.tm_hour;
}

inline int datetime::getMinutes() const {
	return timestruct.tm_min;
}

inline int datetime::getSeconds() const {
	return timestruct.tm_sec;
}

inline int datetime::getMonth() const {
	return timestruct.tm_mon+1;
}

inline int	datetime::getDayOfMonth() const {
	return timestruct.tm_mday;
}

inline int	datetime::getDayOfWeek() const {
	return timestruct.tm_wday;
}

inline int	datetime::getDayOfYear() const {
	return timestruct.tm_yday+1;
}

inline int	datetime::getYear() const {
	return timestruct.tm_year+1900;
}

inline int	datetime::isDaylightSavingsTime() const {
	return timestruct.tm_isdst;
}

inline char	*datetime::getTimeZone() const {
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

inline long	datetime::getTimeZoneOffset() const {
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

inline time_t	datetime::getEpoch() const {
	return epoch;
}

inline tm	*datetime::getTm() {
	return &timestruct;
}
