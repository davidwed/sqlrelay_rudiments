// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		void	init();
		void	clear();

		bool	getBrokenDownTimeFromEpoch();
		bool	normalize();
		void	processTZ(void *tms);

		bool	setTZ(const char *zone, char **oldzone, bool ignoredst);
		bool	restoreTZ(char *oldzone);

		const char	*lookupCombinedTimeZone(const char *zone) const;
		bool		daylightZone(const char *zone) const;

		const char	*getTzName(uint8_t index);

		static bool	acquireLock();
		static bool	releaseLock();

		datetimeprivate	*pvt;
