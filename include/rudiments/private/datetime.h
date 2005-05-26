// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		#ifdef HAVE_GETSYSTEMTIME
			SYSTEMTIME		st;
			TIME_ZONE_INFORMATION	tzi;
		#else
			long	sec;
			long	min;
			long	hour;
			long	mday;
			long	mon;
			long	year;
			long	wday;
			long	yday;
			long	isdst;

			char	*zone;
			long	gmtoff;

			char		*timestring;
			struct tm	*structtm;

			time_t	epoch;

			bool	getBrokenDownTimeFromEpoch(bool needmutex);
			bool	normalizeBrokenDownTime(bool needmutex);

			bool	setTimeZoneEnvVar(const char *zone,
							char **oldzone,
							bool ignoredst);
			bool	restoreTimeZoneEnvVar(const char *oldzone);

			const char	*lookupCombinedTimeZone(
							const char *zone) const;
			bool		daylightZone(const char *zone) const;

			environment	env;
		#endif

		#ifdef RUDIMENTS_HAS_THREADS
			bool	acquireLock();
			bool	releaseLock();

			static	mutex	*timemutex;
		#endif
