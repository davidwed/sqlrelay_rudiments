// Copyright (c) 2003 David Muse
// See the COPYING file for more information

	private:
		long		ttisgmtcnt;
		long		ttisstdcnt;
		long		leapcnt;
		long		timecnt;
		long		typecnt;
		long		charcnt;
		long		*transitiontime;
		unsigned char	*localtime;
		ttinfo		*ti;
		long		*leapsecondtime;
		long		*totalleapseconds;
		unsigned char	*transstdwall;
		unsigned char	*transutclocal;
		unsigned char	*rawtimezonestring;
		unsigned char	**timezonestrings;

		void	timezonefileClean();
		void	timezonefileClone(const timezonefile &t);
