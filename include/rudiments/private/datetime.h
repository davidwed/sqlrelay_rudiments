// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		int	sec;
		int	min;
		int	hour;
		int	mday;
		int	mon;
		int	year;
		int	wday;
		int	yday;
		int	isdst;

		char	*zone;
		long	gmtoff;

		char		*timestring;
		struct tm	*structtm;

		time_t	epoch;

		bool	getBrokenDownTimeFromEpoch(bool needmutex);
		bool	normalizeBrokenDownTime(bool needmutex);

		bool	setTimeZoneEnvVar(const char *zone, char **oldzone);
		bool	restoreTimeZoneEnvVar(const char *oldzone);

		bool	acquireLock();
		bool	releaseLock();

		environment	env;

		static	pthread_mutex_t	*timemutex;
