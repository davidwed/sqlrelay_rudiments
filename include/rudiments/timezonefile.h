// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_TIMEZONEFILE_H
#define RUDIMENTS_TIMEZONEFILE_H

#include <rudiments/private/timezonefileincludes.h>

// The timezonefile class provides methods for parsing and accessing timezone
// files such as are typically found in /usr/share/zoneinfo.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class timezonefileprivate;

struct ttinfo {
	int64_t		tt_gmtoff;
	int32_t		tt_isdst;
	uint32_t	tt_abbrind;
};

class DLLSPEC timezonefile {
	public:
				timezonefile();
				timezonefile(const timezonefile &t);
		timezonefile	&operator=(const timezonefile &t);
				~timezonefile();

		bool		parseFile(const char *filename);
				// Parses "filename".  Returns true on success
				// and false on failure.

		uint64_t	getTimeCount();
				// Returns the number of transition times or
				// local times stored in the file.
		uint64_t	*getTransitionTimes();
				// Returns an array of transition times.
		uint64_t	getTransitionTime(int32_t index);
				// Returns the transition time at "index".
		unsigned char	*getLocalTimes();
				// Returns an array of local times.
		unsigned char	getLocalTimes(int32_t index);
				// Returns the local time at "index".

		uint64_t	getTypeCount();
				// Returns the number of time type info's
				// stored in the file.
		ttinfo		**getTimeTypeInfos();
				// Returns an array of time type info's.
		ttinfo		*getTimeTypeInfo(int32_t index);
				// Returns the time type info at "index".

		uint64_t	getLeapCount();
				// Returns the number of leap second times or
				// total leap seconds in the file.
		uint64_t	*getLeapSecondTimes();
				// Returns an array of leap second times.
		uint64_t	getLeapSecondTime(int32_t index);
				// Returns the leap second time at "index".
		uint64_t	*getTotalLeapSeconds();
				// Returns an array of total leap seconds.
		uint64_t	getTotalLeapSeconds(int32_t index);
				// Returns the total leap seconds at "index".

		uint64_t	getIsSTDCount();
				// Returns the number of standard or wall
				// indicators in the file.
		unsigned char	*getStandardOrWallIndicators();
				// Returns an array of standard or wall
				// indicators.
		unsigned char	getStandardOrWallIndicator(int32_t index);
				// Returns the standard or wall indicator at
				// "index".

		uint64_t	getIsGMTCount();
				// Returns the number of GMT/UTC or local
				// indicators in the file.
		unsigned char	*getUTCOrLocalIndicators();
				// Returns an array of UTC or local indicators.
		unsigned char	getUTCOrLocalIndicator(int32_t index);
				// Returns the UTC or local indicator at
				// "index".

		uint64_t	getCharacterCount();
				// Returns the number of characters in the
				// raw time zone string.
		unsigned char	*getRawTimeZoneString();
				// Returns the raw time zone string.
		unsigned char	**getTimeZoneStrings();
				// Returns an array of time zone strings.
		unsigned char	*getTimeZoneString(int32_t index);
				// Returns the time zone string at "index".

		void		print();
				// Prints out a representation of the timezone
				// file.

	#include <rudiments/private/timezonefile.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
