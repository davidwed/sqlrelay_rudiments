// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

	private:

		void	init();

		#if defined(RUDIMENTS_HAS_PCRE)
			pcre	*expr;
		#elif defined(HAVE_REGEX_H)
			regex_t	expr;
		#elif defined(HAVE_REGEXP_H)
			regexp	*expr;
		#elif defined(HAVE_REGCMP) || defined(HAVE_RE_COMP) \
			defined(HAVE_REGEXPR_H)
			char	*expr;
		#endif
	
		#if defined(RUDIMENTS_HAS_PCRE) || defined(HAVE_REGEX_H)
			#define RUDIMENTS_REGEX_MATCHES 128
			int	matchcount;
			char	*str;
		#endif

		#if defined(RUDIMENTS_HAS_PCRE)
		int		matches[RUDIMENTS_REGEX_MATCHES*3];
		#elif defined(HAVE_REGEX_H)
		regmatch_t	matches[RUDIMENTS_REGEX_MATCHES];
		#endif
