// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		struct tm	*timestruct;
		time_t		epoch;
		char		*timestring;

		void	initTimeString();
		void	initTimeStruct();
		int	setTimeZone(const char *tz);
		int	updateTime();
		int	updateTimePreservingTimeZone();
		int	copyStructTm(const struct tm *oldtm, struct tm *newtm);

		int	setTimeZoneEnvVar(const char *zone, char **oldzone);
		int	restoreTimeZoneEnvVar(const char *oldzone);

		environment	env;

		static	pthread_mutex_t	*ltmutex;
		static	pthread_mutex_t	*envmutex;
