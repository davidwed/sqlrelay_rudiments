// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_TIMEZONEFILE_H
#define RUDIMENTS_TIMEZONEFILE_H

// The timezonefile class provides methods for parsing and accessing timezone
// files such as are typically found in /usr/share/zoneinfo.

struct ttinfo {
	long		tt_gmtoff;
	int		tt_isdst;
	unsigned int	tt_abbrind;
};

class timezonefile {
	public:
			timezonefile();
			~timezonefile();

		int		parseFile(const char *filename);
				// Parses "filename".  Returns 1 on success and
				// 0 on failure.

		long		getTimeCount();
				// Returns the number of transition times or
				// local times stored in the file.
		long		*getTransitionTimes();
				// Returns an array of transition times.
		long		getTransitionTime(int index);
				// Returns the transition time at "index".
		unsigned char	*getLocalTimes();
				// Returns an array of local times.
		unsigned char	getLocalTimes(int index);
				// Returns the local time at "index".

		long		getTypeCount();
				// Returns the number of time type info's
				// stored in the file.
		ttinfo		**getTimeTypeInfos();
				// Returns an array of time type info's.
		ttinfo		*getTimeTypeInfo(int index);
				// Returns the time type info at "index".

		long		getLeapCount();
				// Returns the number of leap second times or
				// total leap seconds in the file.
		long		*getLeapSecondTimes();
				// Returns an array of leap second times.
		long		getLeapSecondTime(int index);
				// Returns the leap second time at "index".
		long		*getTotalLeapSeconds();
				// Returns an array of total leap seconds.
		long		getTotalLeapSeconds(int index);
				// Returns the total leap seconds at "index".

		long		getIsSTDCount();
				// Returns the number of standard or wall
				// indicators in the file.
		unsigned char	*getStandardOrWallIndicators();
				// Returns an array of standard or wall
				// indicators.
		unsigned char	getStandardOrWallIndicator(int index);
				// Returns the standard or wall indicator at
				// "index".

		long		getIsGMTCount();
				// Returns the number of GMT/UTC or local
				// indicators in the file.
		unsigned char	*getUTCOrLocalIndicators();
				// Returns an array of UTC or local indicators.
		unsigned char	getUTCOrLocalIndicator(int index);
				// Returns the UTC or local indicator at
				// "index".

		long		getCharacterCount();
				// Returns the number of characters in the
				// raw time zone string.
		unsigned char	*getRawTimeZoneString();
				// Returns the raw time zone string.
		unsigned char	**getTimeZoneStrings();
				// Returns an array of time zone strings.
		unsigned char	*getTimeZoneString(int index);
				// Returns the time zone string at "index".

		void		print();
				// Prints out a representation of the timezone
				// file.

	#include <rudiments/private/timezonefile.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/timezonefileinlines.h>
#endif

#endif
