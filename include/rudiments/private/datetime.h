// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		struct tm	*timestruct;
		time_t		epoch;
		char		*timestring;

		void	initTimeString();
		void	initTimeStruct();
		bool	setTimeZone(const char *tz);
		bool	updateTime();
		bool	updateTimePreservingTimeZone();
		bool	copyStructTm(const struct tm *oldtm, struct tm *newtm);

		bool	setTimeZoneEnvVar(const char *zone, char **oldzone);
		bool	restoreTimeZoneEnvVar(const char *oldzone);

		environment	env;

		static	pthread_mutex_t	*ltmutex;
		static	pthread_mutex_t	*envmutex;
