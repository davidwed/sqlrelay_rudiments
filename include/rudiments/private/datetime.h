// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		#ifdef HAVE_GETSYSTEMTIME
			SYSTEMTIME		st;
			TIME_ZONE_INFORMATION	tzi;
		#else
			int32_t	sec;
			int32_t	min;
			int32_t	hour;
			int32_t	mday;
			int32_t	mon;
			int32_t	year;
			int32_t	wday;
			int32_t	yday;
			int32_t	isdst;

			char	*zone;
			int32_t	gmtoff;

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
