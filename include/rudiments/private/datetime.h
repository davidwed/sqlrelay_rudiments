// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		bool	getBrokenDownTimeFromEpoch(bool needmutex);
		bool	normalizeBrokenDownTime(bool needmutex);
		void	processTZ(void *tms);

		bool	setTimeZoneEnvVar(const char *zone,
						char **oldzone,
						bool ignoredst);
		bool	restoreTimeZoneEnvVar(char *oldzone);

		const char	*lookupCombinedTimeZone(const char *zone) const;
		bool		daylightZone(const char *zone) const;

		const char	*getTzName(uint8_t index);

		bool	acquireLock();
		bool	releaseLock();

		datetimeprivate	*pvt;
