// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>
#if defined(HAVE_REGEX_H)
	#include <sys/types.h>
	#include <regex.h>
#elif defined(HAVE_REGEXP_H)
	#include <regexp.h>
#elif defined(HAVE_LIBGEN_H)
	#include <libgen.h>
#elif defined(HAVE_REGEXPR_H)
	#include <regexpr.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE regularexpression::regularexpression() {
	compiledexpression=0;
}

RUDIMENTS_INLINE regularexpression::regularexpression(const char *pattern) {
	compiledexpression=0;
	compile(pattern);
}

RUDIMENTS_INLINE regularexpression::~regularexpression() {
	if (compiledexpression) {
		#if defined(HAVE_REGEX_H)
			regfree((regex_t *)compiledexpression);
		#elif defined(HAVE_REGEXP_H)
			delete (regexp *)compiledexpression;
		#elif defined(HAVE_REGCMP)
			delete (char *)compiledexpression;
		#elif defined(HAVE_RE_COMP)
			delete (char *)compiledexpression;
		#elif defined(HAVE_REGEXPR_H)
			delete (char *)compiledexpression;
		#endif
	}
}

RUDIMENTS_INLINE bool regularexpression::match(const char *str) {

	if (compiledexpression) {
		#if defined(HAVE_REGEX_H)
			return !regexec((regex_t *)compiledexpression,str,0,
						(regmatch_t *)NULL,0);
		#elif defined(HAVE_REGEXP_H)
			return regexec((regexp *)compiledexpression,str);
		#elif defined(HAVE_REGCMP)
			return regex((char *)compiledexpression,str,NULL);
		#elif defined(HAVE_RE_COMP)
			return !re_exec((char *)compiledexpression);
		#elif defined(HAVE_REGEXPR_H)
			return step((char *)compiledexpression,expbuf);
		#endif
	}

	return false;
}
