// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/regularexpression.h>
#include <rudiments/rawbuffer.h>

#ifdef RUDIMENTS_HAS_PCRE
	#include <pcre.h>
#else
	#include <sys/types.h>
	#include <regex.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef RUDIMENTS_HAS_PCRE
	#include <rudiments/charstring.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class regularexpressionprivate {
	friend class regularexpression;
	private:
		#ifdef RUDIMENTS_HAS_PCRE
			pcre		*_expr;
			pcre_extra	*_extra;
		#else
			regex_t	_expr;
		#endif
	
		#define RUDIMENTS_REGEX_MATCHES 128
		int		_matchcount;
		const char	*_str;

		#ifdef RUDIMENTS_HAS_PCRE
		int		_matches[RUDIMENTS_REGEX_MATCHES*3];
		#else
		regmatch_t	_matches[RUDIMENTS_REGEX_MATCHES];
		#endif
};

regularexpression::regularexpression() {
	regularexpressionInit();
}

regularexpression::regularexpression(const char *pattern) {
	regularexpressionInit();
	compile(pattern);
}

void regularexpression::regularexpressionInit() {
	pvt=new regularexpressionprivate;
	#ifdef RUDIMENTS_HAS_PCRE
		pvt->_expr=NULL;
		pvt->_extra=NULL;
	#else
		rawbuffer::zero(&pvt->_expr,sizeof(pvt->_expr));
	#endif
	pvt->_matchcount=0;
	pvt->_str=NULL;
	rawbuffer::zero(pvt->_matches,sizeof(pvt->_matches));
}

regularexpression::~regularexpression() {
	#ifdef RUDIMENTS_HAS_PCRE
		if (pvt->_expr) {
			pcre_free(pvt->_expr);
		}
		if (pvt->_extra) {
			delete pvt->_extra;
		}
	#else
		regfree(&pvt->_expr);
	#endif
	delete pvt;
}

bool regularexpression::compile(const char *pattern) {
	#ifdef RUDIMENTS_HAS_PCRE
		if (pvt->_expr) {
			pcre_free(pvt->_expr);
		}
		if (pvt->_extra) {
			delete pvt->_extra;
			pvt->_extra=NULL;
		}
		const char	*error;
		int		erroroffset;
		return (pvt->_expr=pcre_compile(pattern,0,&error,
						&erroroffset,NULL))!=NULL;
	#else
		regfree(&pvt->_expr);
		return !regcomp(&pvt->_expr,pattern,REG_EXTENDED);
	#endif
}

bool regularexpression::study() {
	#ifdef RUDIMENTS_HAS_PCRE
		const char	*error;
		if (pvt->_extra) {
			delete pvt->_extra;
			pvt->_extra=NULL;
		}
		pvt->_extra=pcre_study(pvt->_expr,0,&error);
		return (!pvt->_extra && error)?false:true;
	#else
		return true;
	#endif
}

bool regularexpression::match(const char *str) {
	if (!str) {
		return false;
	}
	#ifdef RUDIMENTS_HAS_PCRE
		pvt->_str=str;
		pvt->_matchcount=-1;
		return (pvt->_expr &&
			(pvt->_matchcount=pcre_exec(pvt->_expr,
						pvt->_extra,
						pvt->_str,
						charstring::length(pvt->_str),
						0,0,pvt->_matches,
						RUDIMENTS_REGEX_MATCHES*3))>-1);
	#else
		pvt->_str=str;
		for (int i=0; i<pvt->_matchcount; i++) {
			pvt->_matches[i].rm_so=-1;
		}
		pvt->_matchcount=-1;
		bool	retval=!regexec(&pvt->_expr,pvt->_str,
				RUDIMENTS_REGEX_MATCHES,pvt->_matches,0);
		getSubstringCount();
		return retval;
	#endif
}

int regularexpression::getSubstringCount() {
	#ifndef RUDIMENTS_HAS_PCRE
		if (pvt->_matchcount==-1) {
			for (int i=0; i<RUDIMENTS_REGEX_MATCHES; i++) {
				if (pvt->_matches[i].rm_so==-1) {
					pvt->_matchcount=i;
					break;
				}
			}
		}
	#endif
	if (pvt->_matchcount==-1) {
		return 0;
	}
	return pvt->_matchcount;
}

int regularexpression::getSubstringStartOffset(int index) {
	if (index<0 || index>pvt->_matchcount) {
		return -1;
	}
	#ifdef RUDIMENTS_HAS_PCRE
		return pvt->_matches[index*2];
	#else
		return pvt->_matches[index].rm_so;
	#endif
}

int regularexpression::getSubstringEndOffset(int index) {
	if (index<0 || index>pvt->_matchcount) {
		return -1;
	}
	#ifdef RUDIMENTS_HAS_PCRE
		return pvt->_matches[index*2+1];
	#else
		return pvt->_matches[index].rm_eo;
	#endif
}

char *regularexpression::getSubstringStart(int index) {
	int	offset=getSubstringStartOffset(index);
	return (offset>-1)?(const_cast<char *>(pvt->_str+offset)):NULL;
}

char *regularexpression::getSubstringEnd(int index) {
	int	offset=getSubstringEndOffset(index);
	return (offset>-1)?(const_cast<char *>(pvt->_str+offset)):NULL;
}

bool regularexpression::match(const char *str, const char *pattern) {
	regularexpression	re;
	return (re.compile(pattern) && re.match(str));
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
