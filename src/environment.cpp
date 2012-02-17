// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/environment.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#include <stdio.h>

#ifdef RUDIMENTS_HAVE_ENVIRON
	#ifdef RUDIMENTS_HAVE_UNISTD_H
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
#ifdef RUDIMENTS_HAVE_PUTENV
static namevaluepairs	*_envstrings;
#endif

#ifdef RUDIMENTS_HAVE_PUTENV
void environment::init() {
	_envstrings=new namevaluepairs;

	// On windows, the entire rudiments dll appears to have been unloaded
	// before atexit functions are called, and it leads to all kinds of
	// chaos.  Ideally this should be called when the dll is unloaded but
	// that's tricky to do too.  For now it's just disabled.
	#ifndef _WIN32
		atexit((void (*)(void))environment::exit);
	#endif
}

void environment::exit() {
	for (namevaluepairslistnode *node=
			(namevaluepairslistnode *)
				_envstrings->getList()->getNodeByIndex(0);
			node;
			node=(namevaluepairslistnode *)node->getNext()) {
		free((void *)node->getData()->getData());
	}
	delete _envstrings;
}

bool environment::setValue(const char *variable, const char *value) {
	if (!_envstrings) {
		init();
	}
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	size_t	pestrlen=charstring::length(variable)+
				charstring::length(value)+2;
	char	*pestr=(char *)malloc(pestrlen*sizeof(char));
	charstring::copy(pestr,variable);
	charstring::append(pestr,"=");
	charstring::append(pestr,value);
	int32_t	result;
	do {
		#if defined(RUDIMENTS_HAVE__PUTENV)
			result=_putenv(pestr);
		#elif defined(RUDIMENTS_HAVE_PUTENV)
			result=putenv(pestr);
		#else
			#error no putenv or anything like it
		#endif
	} while (result && error::getErrorNumber()==EINTR);
	if (!result) {
		char	*oldpestr;
		if (_envstrings->getData(
				const_cast<char *>(variable),&oldpestr)) {
			free((void *)oldpestr);
		}
		_envstrings->setData(const_cast<char *>(variable),pestr);
		retval=true;
	} else {
		free((void *)pestr);
		retval=false;
	}
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}
#else
void environment::exit() {
	// do nothing
}

void environment::init() {
	// do nothing
}
#endif

#ifdef RUDIMENTS_HAVE__DUPENV_S
	static	char	*envval=NULL;
#endif

const char *environment::getValue(const char *variable) {
	char	*retval=NULL;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	do {
		#if defined(RUDIMENTS_HAVE__DUPENV_S)
			// FIXME: _dupenv_s is meant to be thread-safe and this
			// usage certainly isn't.  This just emulates the
			// behavior of getenv and suppresses compiler warnings.
			free((void *)envval);
			size_t	len;
			if (!_dupenv_s(&envval,&len,variable)) {
				retval=envval;
			}
		#elif defined(RUDIMENTS_HAVE_GETENV)
			retval=getenv(variable);
		#else
			#error no getenv or anything like it
		#endif
	} while (!retval && error::getErrorNumber()==EINTR);
	if (_envmutex) {
		_envmutex->unlock();
	}
	return retval;
}

#if defined(RUDIMENTS_HAVE_SETENV) && !defined(RUDIMENTS_HAVE_PUTENV)
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
#ifdef HAVE_PUTENV
	if (!_envstrings) {
		init();
	}
#endif
#ifdef RUDIMENTS_HAVE_UNSETENV
	bool	retval=false;
	if (_envmutex && !_envmutex->lock()) {
		return retval;
	}
	unsetenv(variable);
	#ifdef HAVE_PUTENV
		char *pestr;
		if (envstrings->getData(const_cast<char *>(variable),&pestr)) {
			free((void *)pestr);
		}
		envstrings->removeData(const_cast<char *>(variable));
	#endif
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
