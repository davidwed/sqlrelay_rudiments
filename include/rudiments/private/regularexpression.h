// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:

		void	init();

		#ifdef RUDIMENTS_HAS_PCRE
			pcre		*expr;
			pcre_extra	*extra;
		#else
			regex_t	expr;
		#endif
	
		#define RUDIMENTS_REGEX_MATCHES 128
		int		matchcount;
		const char	*str;

		#ifdef RUDIMENTS_HAS_PCRE
		int		matches[RUDIMENTS_REGEX_MATCHES*3];
		#else
		regmatch_t	matches[RUDIMENTS_REGEX_MATCHES];
		#endif
