// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_TIMEZONEFILE_H
#define RUDIMENTS_TIMEZONEFILE_H

#include <rudiments/private/timezonefileincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class timezonefileprivate;

struct ttinfo {
	int64_t		tt_gmtoff;
	int32_t		tt_isdst;
	uint32_t	tt_abbrind;
};

/** The timezonefile class provides methods for parsing and accessing timezone
 *  files such as are typically found in /usr/share/zoneinfo on unix. */
class RUDIMENTS_DLLSPEC timezonefile {
	public:

		/** Creates an instance of the timezonefile class. */
		timezonefile();

		/** Creates an instance of the timezonefile class
		 *  that is a copy of "t". */
		timezonefile(const timezonefile &t);

		/** Makes this instance of the timezonefile class
		 *  identical to "t". */
		timezonefile	&operator=(const timezonefile &t);

		/** Delets this instance of the timezonefile class. */
		~timezonefile();

		/** Parses "filename".  Returns true on success
		 *  and false on failure. */
		bool		parseFile(const char *filename);

		/** Returns the number of transition times or
		 *  local times stored in the file. */
		uint64_t	getTimeCount();

		/** Returns an array of transition times. */
		uint64_t	*getTransitionTimes();

		/** Returns the transition time at "index". */
		uint64_t	getTransitionTime(int32_t index);

		/** Returns an array of local times. */
		unsigned char	*getLocalTimes();

		/** Returns the local time at "index". */
		unsigned char	getLocalTimes(int32_t index);

		/** Returns the number of time type info's
		 *  stored in the file. */
		uint64_t	getTypeCount();

		/** Returns an array of time type info's. */
		ttinfo		**getTimeTypeInfos();

		/** Returns the time type info at "index". */
		ttinfo		*getTimeTypeInfo(int32_t index);

		/** Returns the number of leap second times or
		 *  total leap seconds in the file. */
		uint64_t	getLeapCount();

		/** Returns an array of leap second times. */
		uint64_t	*getLeapSecondTimes();

		/** Returns the leap second time at "index". */
		uint64_t	getLeapSecondTime(int32_t index);

		/** Returns an array of total leap seconds. */
		uint64_t	*getTotalLeapSeconds();

		/** Returns the total leap seconds at "index". */
		uint64_t	getTotalLeapSeconds(int32_t index);

		/** Returns the number of standard or wall
		 *  indicators in the file. */
		uint64_t	getIsSTDCount();

		/** Returns an array of standard or wall indicators. */
		unsigned char	*getStandardOrWallIndicators();

		/** Returns the standard or wall indicator at "index". */
		unsigned char	getStandardOrWallIndicator(int32_t index);

		/** Returns the number of GMT/UTC or local
		 *  indicators in the file. */
		uint64_t	getIsGMTCount();

		/** Returns an array of UTC or local indicators. */
		unsigned char	*getUTCOrLocalIndicators();

		/** Returns the UTC or local indicator at "index". */
		unsigned char	getUTCOrLocalIndicator(int32_t index);

		/** Returns the number of characters in the
		 *  raw time zone string. */
		uint64_t	getCharacterCount();

		/** Returns the raw time zone string. */
		unsigned char	*getRawTimeZoneString();

		/** Returns an array of time zone strings. */
		unsigned char	**getTimeZoneStrings();

		/** Returns the time zone string at "index". */
		unsigned char	*getTimeZoneString(int32_t index);

		/** Prints out a representation of the timezone file. */
		void	print();

	#include <rudiments/private/timezonefile.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
