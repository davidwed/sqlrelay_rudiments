// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_TIMEZONEFILE_H
#define RUDIMENTS_TIMEZONEFILE_H

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
		long		getIsGMTCount();
		long		getIsSTDCount();
		long		getLeapCount();
		long		getTimeCount();
		long		getTypeCount();
		long		getCharacterCount();
		long		*getTransitionTimes();
		long		getTransitionTime(int index);
		unsigned char	*getLocalTimes();
		unsigned char	getLocalTimes(int index);
		ttinfo		**getTimeTypeInfos();
		ttinfo		*getTimeTypeInfo(int index);
		long		*getLeapSecondTimes();
		long		getLeapSecondTime(int index);
		long		*getTotalLeapSeconds();
		long		getTotalLeapSeconds(int index);
		unsigned char	*getStandardOrWallIndicators();
		unsigned char	getStandardOrWallIndicator(int index);
		unsigned char	*getUTCOrLocalIndicators();
		unsigned char	getUTCOrLocalIndicator(int index);
		unsigned char	*getRawTimeZoneString();
		unsigned char	**getTimeZoneStrings();
		unsigned char	*getTimeZoneString(int index);
		void		print();

	#include <rudiments/private/timezonefile.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/timezonefileinlines.h>
#endif

#endif
