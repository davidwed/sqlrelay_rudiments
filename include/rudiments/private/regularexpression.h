// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:
		long	compiledexpression;
		#define RUDIMENTS_REGEX_MATCHES 128
		int	matches[RUDIMENTS_REGEX_MATCHES];
		int	matchcount;
		char	*str;
