// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/regularexpression.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/regularexpressioninlines.h>
#endif

int	regularexpression::compile(const char *pattern) {

	#if defined(HAVE_REGEX_H) || defined(HAVE__USR_LOCAL_FIRSTWORKS_INCLUDE_REGEX_H) || defined(HAVE__FIRSTWORKS_INCLUDE_REGEX_H)
		if (compiledexpression) {
			regfree((regex_t *)compiledexpression);
			compiledexpression=0;
		}
		regex_t	*preg=new regex_t;
		if (!regcomp(preg,pattern,REG_EXTENDED|REG_NOSUB)) {
			compiledexpression=(long)preg;
			return 1;
		}
	#elif defined(HAVE_REGEXP_H)
		if (compiledexpression) {
			delete (regexp *)compiledexpression;
			compiledexpression=0;
		}
		regexp *prog=regcomp(pattern);
		if (prog) {
			compiledexpression=(long)prog;
			return 1;
		}
	#elif defined(HAVE_REGCMP)
		if (compiledexpression) {
			delete (char *)compiledexpression;
			compiledexpression=0;
		}
		char	*re=regcmp(pattern,NULL);
		if (re) {
			compiledexpression=(long)re;
			return 1;
		}
	#elif defined(HAVE_RE_COMP)
		if (compiledexpression) {
			delete (char *)compiledexpression;
			compiledexpression=0;
		}
		char	*re=re_comp(pattern);
		if (re) {
			compiledexpression=(long)re;
			return 1;
		}
	#elif defined(HAVE_REGEXPR_H)
		if (compiledexpression) {
			delete (char *)compiledexpression;
			compiledexpression=0;
		}
		char	*expbuf=compile(pattern,NULL,NULL);
		if (expbuf) {
			compiledexpression=(long)expbuf;
			return 1;
		}
	#endif

	return 0;
}

int	regularexpression::match(const char *str, const char *pattern) {

	int	retval=0;

	#if defined(HAVE_REGEX_H) || defined(HAVE__USR_LOCAL_FIRSTWORKS_INCLUDE_REGEX_H) || defined(HAVE__FIRSTWORKS_INCLUDE_REGEX_H)
		regex_t	preg;
		if (!regcomp(&preg,pattern,REG_EXTENDED|REG_NOSUB)) {
			retval=!regexec(&preg,str,0,
					(regmatch_t *)NULL,0);
		}
		regfree(&preg);
	#elif defined(HAVE_REGEXP_H)
		regexp *prog=regcomp(pattern);
		if (prog) {
			retval=regexec(prog,str);
		}
		delete prog;
	#elif defined(HAVE_REGCMP)
		char	*re=regcmp(pattern,NULL);
		if (re && regex(re,str,NULL)) {
			retval=1;
		}
		delete re;
	#elif defined(HAVE_RE_COMP)
		char	*re=re_comp(pattern);
		retval=!re_exec(re);
		delete re;
	#elif defined(HAVE_REGEXPR_H)
		char	*expbuf=compile(pattern,NULL,NULL);
		if (expbuf && step(str,expbuf)) {
			retval=1;
		}
		delete expbuf;
	#endif

	return retval;
}
