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
#elif defined(HAVE_REGEX_H)
#elif defined(HAVE_REGEXP_H)
	#include <regexp.h>
#elif defined(HAVE_LIBGEN_H)
	#include <libgen.h>
#elif defined(HAVE_REGEXPR_H)
	#include <regexpr.h>
#endif

regularexpression::regularexpression() {
	init();
}

regularexpression::regularexpression(const char *pattern) {
	init();
	compile(pattern);
}

void regularexpression::init() {
	#if defined(RUDIMENTS_HAS_PCRE) || \
		defined(HAVE_REGEXP_H) || \
		defined(HAVE_LIBGEN_H) || \
		defined(HAVE_REGEXPR_H)
		expr=NULL;
	#endif
}

regularexpression::~regularexpression() {
	#if defined(RUDIMENTS_HAS_PCRE)
		if (expr) {
			pcre_free(expr);
		}
	#elif defined(HAVE_REGEX_H)
		regfree(&expr);
	#elif defined(HAVE_REGEXP_H)
		delete expr;
	#elif defined(HAVE_REGCMP) || \
		defined(HAVE_RE_COMP) || \
		defined(HAVE_REGEXPR_H)
		delete[] expr;
	#endif
}

bool regularexpression::compile(const char *pattern) {

	#if defined(RUDIMENTS_HAS_PCRE)
		if (expr) {
			pcre_free(expr);
		}
		const char	*error;
		int		erroroffset;
		return (expr=pcre_compile(pattern,0,&error,
						&erroroffset,NULL))!=NULL;
		// FIXME: pcre_study here...
	#elif defined(HAVE_REGEX_H)
		regfree(&expr);
		return !regcomp(&expr,pattern,REG_EXTENDED);
	#elif defined(HAVE_REGEXP_H)
		delete expr;
		return (expr=regcomp(pattern))!=NULL;
	#elif defined(HAVE_REGCMP)
		delete[] expr;
		return (expr=regcmp(pattern,NULL))!=NULL;
	#elif defined(HAVE_RE_COMP)
		delete[] expr;
		return (expr=re_comp(pattern))!=NULL;
	#elif defined(HAVE_REGEXPR_H)
		delete[] expr;
		return (expr=compile(pattern,NULL,NULL))!=NULL;
	#endif
}

bool regularexpression::match(const char *str) {

	#if defined(RUDIMENTS_HAS_PCRE)
		this->str=(char *)str;
		return (expr && (matchcount=pcre_exec(expr,NULL,
						str,strlen(str),0,0,matches,
						RUDIMENTS_REGEX_MATCHES*3))>-1);
	#elif defined(HAVE_REGEX_H)
		this->str=(char *)str;
		matchcount=-1;
		return (!regexec(&expr,str,RUDIMENTS_REGEX_MATCHES,matches,0));
	#elif defined(HAVE_REGEXP_H)
		return (expr && regexec(expr,str));
	#elif defined(HAVE_REGCMP)
		return (expr && regex(expr,str,NULL));
	#elif defined(HAVE_RE_COMP)
		return (expr && !re_exec(expr));
	#elif defined(HAVE_REGEXPR_H)
		return (expr && step(expr,expbuf));
	#endif
}

#if defined(RUDIMENTS_HAS_PCRE) || defined(HAVE_REGEX_H)
int regularexpression::getSubstringCount() {
	#if defined(RUDIMENTS_HAS_PCRE)
		return matchcount;
	#elif defined(HAVE_REGEX_H)
		if (matchcount==-1) {
			for (int i=0; i<RUDIMENTS_REGEX_MATCHES; i++) {
				if (matches[i].rm_so==-1) {
					matchcount=i;
					break;
				}
			}
		}
		return matchcount;
	#endif
}

int regularexpression::getSubstringStartOffset(int index) {
	if (index<0 || index>matchcount) {
		return -1;
	}
	#if defined(RUDIMENTS_HAS_PCRE)
		return matches[index*2];
	#elif defined(HAVE_REGEX_H)
		return matches[index].rm_so;
	#endif
}

int regularexpression::getSubstringEndOffset(int index) {
	if (index<0 || index>matchcount) {
		return -1;
	}
	#if defined(RUDIMENTS_HAS_PCRE)
		return matches[index*2+1];
	#elif defined(HAVE_REGEX_H)
		return matches[index].rm_eo;
	#endif
}

char *regularexpression::getSubstringStart(int index) {
	int	offset=getSubstringStartOffset(index);
	return (offset>-1)?(str+offset):NULL;
}
char *regularexpression::getSubstringEnd(int index) {
	int	offset=getSubstringEndOffset(index);
	return (offset>-1)?(str+offset):NULL;
}
#endif

bool regularexpression::match(const char *str, const char *pattern) {
	regularexpression	re;
	return (re.compile(pattern) && re.match(str));
}
