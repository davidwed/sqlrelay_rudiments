// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/regularexpression.h>
#ifndef RUDIMENTS_HAS_PCRE
	#include <rudiments/rawbuffer.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#if defined(RUDIMENTS_HAS_PCRE)
	#include <rudiments/charstring.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

regularexpression::regularexpression() {
	init();
}

regularexpression::regularexpression(const char *pattern) {
	init();
	compile(pattern);
}

void regularexpression::init() {
	#ifdef RUDIMENTS_HAS_PCRE
		expr=NULL;
		extra=NULL;
	#else
		rawbuffer::zero(&expr,sizeof(expr));
	#endif
}

regularexpression::~regularexpression() {
	#ifdef RUDIMENTS_HAS_PCRE
		if (expr) {
			pcre_free(expr);
		}
		if (extra) {
			delete extra;
		}
	#else
		regfree(&expr);
	#endif
}

bool regularexpression::compile(const char *pattern) {

	#ifdef RUDIMENTS_HAS_PCRE
		if (expr) {
			pcre_free(expr);
		}
		if (extra) {
			delete extra;
			extra=NULL;
		}
		const char	*error;
		int		erroroffset;
		return (expr=pcre_compile(pattern,0,&error,
						&erroroffset,NULL))!=NULL;
	#else
		regfree(&expr);
		return !regcomp(&expr,pattern,REG_EXTENDED);
	#endif
}

bool regularexpression::study() {
	#ifdef RUDIMENTS_HAS_PCRE
		const char	*error;
		if (extra) {
			delete extra;
		}
		extra=pcre_study(expr,0,&error);
		return (!extra && error)?false:true;
	#else
		return true;
	#endif
}

bool regularexpression::match(const char *str) {

	if (!str) {
		return false;
	}

	#ifdef RUDIMENTS_HAS_PCRE
		this->str=str;
		return (expr && (matchcount=pcre_exec(expr,extra,
						str,charstring::length(str),
						0,0,matches,
						RUDIMENTS_REGEX_MATCHES*3))>-1);
	#else
		this->str=str;
		matchcount=-1;
		return (!regexec(&expr,str,RUDIMENTS_REGEX_MATCHES,matches,0));
	#endif
}

int regularexpression::getSubstringCount() {
	#ifdef RUDIMENTS_HAS_PCRE
		return matchcount;
	#else
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
	#ifdef RUDIMENTS_HAS_PCRE
		return matches[index*2];
	#else
		return matches[index].rm_so;
	#endif
}

int regularexpression::getSubstringEndOffset(int index) {
	if (index<0 || index>matchcount) {
		return -1;
	}
	#ifdef RUDIMENTS_HAS_PCRE
		return matches[index*2+1];
	#else
		return matches[index].rm_eo;
	#endif
}

char *regularexpression::getSubstringStart(int index) {
	int	offset=getSubstringStartOffset(index);
	return (offset>-1)?(const_cast<char *>(str+offset)):NULL;
}

char *regularexpression::getSubstringEnd(int index) {
	int	offset=getSubstringEndOffset(index);
	return (offset>-1)?(const_cast<char *>(str+offset)):NULL;
}

bool regularexpression::match(const char *str, const char *pattern) {
	regularexpression	re;
	return (re.compile(pattern) && re.match(str));
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
