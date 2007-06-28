// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/environment.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef RUDIMENTS_HAVE_ENVIRON
	#ifdef HAVE_UNISTD_H
		#include <unistd.h>
	#endif
#else
	#ifdef RUDIMENTS_HAVE_NSGETENVIRON
		#include <crt_externs.h>
		#define environ (*_NSGetEnviron())
	#else
		extern	char	**environ;
	#endif
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// LAME: not in the class
static mutex	*_envmutex;
#if defined(RUDIMENTS_HAVE_PUTENV) && !defined(RUDIMENTS_HAVE_SETENV)
static namevaluepairs	_envstrings;
#endif

#if defined(RUDIMENTS_HAVE_PUTENV) && !defined(RUDIMENTS_HAVE_SETENV)
bool environment::setValue(const char *variable, const char *value) {
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	char	*pestr;
	if (_envstrings.getData(const_cast<char *>(variable),&pestr)) {
		delete[] pestr;
	}
	size_t	pestrlen=charstring::length(variable)+
				charstring::length(value)+2;
	pestr=new char[pestrlen];
	snprintf(pestr,pestrlen,"%s=%s",variable,value);
	int	result;
	do {
		result=putenv(pestr);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	if (result!=-1) {
		_envstrings.setData(const_cast<char *>(variable),pestr);
		retval=true;
	} else {
		delete[] pestr;
		_envstrings.removeData(const_cast<char *>(variable));
		retval=false;
	}
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}
#endif


const char *environment::getValue(const char *variable) {
	char	*retval=NULL;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	do {
		retval=getenv(variable);
	} while (!retval && error::getErrorNumber()==EINTR);
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}

#ifdef RUDIMENTS_HAVE_SETENV
bool environment::setValue(const char *variable, const char *value) {
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	do {
		retval=!setenv(variable,value,1);
	} while (!retval && error::getErrorNumber()==EINTR);
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}
#endif

bool environment::remove(const char *variable) {
#ifdef RUDIMENTS_HAVE_UNSETENV
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	unsetenv(variable);
	retval=true;
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
#else
	// I know this isn't the same as calling unsetenv, but as far as I
	// know, it's all that can be done.
	return setValue(variable,"");
#endif
}

const char * const *environment::variables() {
	return environ;
}

void environment::print() {
	const char * const *env=variables();
	for (uint64_t index=0; env && env[index]; index++) {
		printf("%s\n",env[index]);
	}
}

bool environment::clear() {
	#ifdef RUDIMENTS_HAVE_CLEARENV
		return !clearenv();
	#else
		environ[0]=NULL;
		return true;
	#endif
}

void environment::setMutex(mutex *mtx) {
	_envmutex=mtx;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
