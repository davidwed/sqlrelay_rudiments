// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/regularexpression.h>
#include <stdlib.h>
#include <stdio.h>
#if defined(RUDIMENTS_HAS_PCRE)
	#include <string.h>
	#ifdef HAVE_STRINGS_H
		#include <strings.h>
	#endif
	#include <pcre.h>
#elif defined(HAVE_REGEX_H)
	#include <sys/types.h>
	#include <regex.h>
#elif defined(HAVE_REGEXP_H)
	#include <regexp.h>
#elif defined(HAVE_LIBGEN_H)
	#include <libgen.h>
#elif defined(HAVE_REGEXPR_H)
	#include <regexpr.h>
#endif

regularexpression::regularexpression() {
	compiledexpression=0;
}

regularexpression::regularexpression(const char *pattern) {
	compiledexpression=0;
	compile(pattern);
}

regularexpression::~regularexpression() {
	if (compiledexpression) {
		#if defined(RUDIMENTS_HAS_PCRE)
			pcre_free((pcre *)compiledexpression);
		#elif defined(HAVE_REGEX_H)
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

bool regularexpression::compile(const char *pattern) {

	#if defined(RUDIMENTS_HAS_PCRE)
		if (compiledexpression) {
			pcre_free((pcre *)compiledexpression);
			compiledexpression=0;
		}
		const char	*error;
		int		erroroffset;
		pcre	*expr=pcre_compile(pattern,0,&error,&erroroffset,NULL);
		// FIXME: pcre_study here...
		if (expr) {
			compiledexpression=(long)expr;
			return true;
		}
	#elif defined(HAVE_REGEX_H)
		if (compiledexpression) {
			regfree((regex_t *)compiledexpression);
			compiledexpression=0;
		}
		regex_t	*preg=new regex_t;
		if (!regcomp(preg,pattern,REG_EXTENDED)) {
			compiledexpression=(long)preg;
			return true;
		}
	#elif defined(HAVE_REGEXP_H)
		if (compiledexpression) {
			delete (regexp *)compiledexpression;
			compiledexpression=0;
		}
		regexp *prog=regcomp(pattern);
		if (prog) {
			compiledexpression=(long)prog;
			return true;
		}
	#elif defined(HAVE_REGCMP)
		if (compiledexpression) {
			delete (char *)compiledexpression;
			compiledexpression=0;
		}
		char	*re=regcmp(pattern,NULL);
		if (re) {
			compiledexpression=(long)re;
			return true;
		}
	#elif defined(HAVE_RE_COMP)
		if (compiledexpression) {
			delete (char *)compiledexpression;
			compiledexpression=0;
		}
		char	*re=re_comp(pattern);
		if (re) {
			compiledexpression=(long)re;
			return true;
		}
	#elif defined(HAVE_REGEXPR_H)
		if (compiledexpression) {
			delete (char *)compiledexpression;
			compiledexpression=0;
		}
		char	*expbuf=compile(pattern,NULL,NULL);
		if (expbuf) {
			compiledexpression=(long)expbuf;
			return true;
		}
	#endif

	return false;
}

bool regularexpression::match(const char *str) {

	this->str=(char *)str;

	#ifndef HAVE_REGEX_H
	for (int i=0; i<RUDIMENTS_REGEX_MATCHES; i++) {
		matches[i]=-1;
	}
	#endif

	if (compiledexpression) {
		#if defined(RUDIMENTS_HAS_PCRE)
			int	tempmatches[RUDIMENTS_REGEX_MATCHES*3];
			matchcount=pcre_exec((pcre *)compiledexpression,NULL,
					str,strlen(str),0,0,tempmatches,
					RUDIMENTS_REGEX_MATCHES*3);
			if (matchcount<0) {
				matchcount=0;
			} else {
				for (int i=0; i<matchcount*2; i=i+2) {
					matches[i/2]=tempmatches[i];
				}
			}
			return matchcount>-1;
		#elif defined(HAVE_REGEX_H)
			regmatch_t	tempmatches[RUDIMENTS_REGEX_MATCHES];
			matchcount=0;
			if (!regexec((regex_t *)compiledexpression,
						str,RUDIMENTS_REGEX_MATCHES,
						tempmatches,0)) {
				for (int i=0; i<RUDIMENTS_REGEX_MATCHES; i++) {
					if (tempmatches[i].rm_so==-1) {
						matchcount=i;
						break;
					}
					matches[i]=tempmatches[i].rm_so;
				}
				return true;
			}
			return false;
		#elif defined(HAVE_REGEXP_H)
			matchcount=0;
			return regexec((regexp *)compiledexpression,str);
		#elif defined(HAVE_REGCMP)
			matchcount=0;
			return regex((char *)compiledexpression,str,NULL);
		#elif defined(HAVE_RE_COMP)
			matchcount=0;
			return !re_exec((char *)compiledexpression);
		#elif defined(HAVE_REGEXPR_H)
			matchcount=0;
			return step((char *)compiledexpression,expbuf);
		#endif
	}

	return false;
}

int regularexpression::getMatchingSubstringCount() {
	return matchcount;
}

int regularexpression::getMatchingSubstringOffset(unsigned short index) {
	if (index>matchcount) {
		return -1;
	}
	return matches[index];
}

char *regularexpression::getMatchingSubstring(unsigned short index) {
	if (index>matchcount) {
		return NULL;
	}
	return str+matches[index];
}

bool regularexpression::match(const char *str, const char *pattern) {

	bool	retval=false;

	#if defined(RUDIMENTS_HAS_PCRE)
		const char	*error;
		int		erroroffset;
		pcre	*expr=pcre_compile(pattern,0,&error,&erroroffset,NULL);
		// FIXME: pcre_study here...
		retval=(pcre_exec(expr,NULL,str,strlen(str),0,0,NULL,0)>-1);
		pcre_free(expr);
	#elif defined(HAVE_REGEX_H)
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
			retval=true;
		}
		delete re;
	#elif defined(HAVE_RE_COMP)
		char	*re=re_comp(pattern);
		retval=!re_exec(re);
		delete re;
	#elif defined(HAVE_REGEXPR_H)
		char	*expbuf=compile(pattern,NULL,NULL);
		if (expbuf && step(str,expbuf)) {
			retval=true;
		}
		delete expbuf;
	#endif

	return retval;
}
