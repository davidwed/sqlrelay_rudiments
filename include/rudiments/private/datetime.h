// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		#ifndef HAVE_GETSYSTEMTIME
			bool	getBrokenDownTimeFromEpoch(bool needmutex);
			bool	normalizeBrokenDownTime(bool needmutex);

			bool	setTimeZoneEnvVar(const char *zone,
							char **oldzone,
							bool ignoredst);
			bool	restoreTimeZoneEnvVar(const char *oldzone);

			const char	*lookupCombinedTimeZone(
							const char *zone) const;
			bool		daylightZone(const char *zone) const;
		#endif

		#ifdef RUDIMENTS_HAS_THREADS
			bool	acquireLock();
			bool	releaseLock();
		#endif

		datetimeprivate	*pvt;
