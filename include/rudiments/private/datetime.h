// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

	private:
		tm	timestruct;
		time_t	epoch;
		char	*timestring;
		tzone	timezone;

		void	initTimeString();
		void	setTimeZone(tzone tz);
		tzone	determineTimeZone(const tm *tmstruct);
		tzone	determineTimeZone(const char *tmstring);

		const static tzone	tzonelist[];
		const static char	*tzonestring[];
		const static long	tzoneoffset[];
