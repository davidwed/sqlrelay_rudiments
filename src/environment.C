// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#define EXCLUDE_RUDIMENTS_TEMPLATE_IMPLEMENTATIONS
#include <rudiments/environment.h>
#include <rudiments/charstring.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_ENVIRON
	#include <unistd.h>
#else
	#ifdef HAVE_NSGETENVIRON
		#include <crt_externs.h>
		#define environ (*_NSGetEnviron())
	#else
		extern	char	**environ;
	#endif
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && defined(__GNUC__)
pthread_mutex_t	*environment::envmutex;
#endif

#if defined(HAVE_PUTENV) && !defined(HAVE_SETENV)

environment::~environment() {
	for (namevaluepairslistnode *node=
			(namevaluepairslistnode *)
				envstrings.getList()->getNodeByIndex(0);
			node;
			node=(namevaluepairslistnode *)node->getNext()) {
		delete[] node->getData()->getData();
	}
}

bool environment::setValue(const char *variable, const char *value) {
	bool	retval=false;
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex && pthread_mutex_lock(envmutex)) {
			return retval;
		}
	#endif
	char	*pestr;
	if (envstrings.getData(const_cast<char *>(variable),&pestr)) {
		delete[] pestr;
	}
	pestr=new char[charstring::length(variable)+
			charstring::length(value)+2];
	sprintf(pestr,"%s=%s",variable,value);
	if (putenv(pestr)!=-1) {
		envstrings.setData(const_cast<char *>(variable),pestr);
		retval=true;
	} else {
		delete[] pestr;
		envstrings.removeData(const_cast<char *>(variable));
		retval=false;
	}
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex) {
			pthread_mutex_unlock(envmutex);
		}
	#endif
	return retval;
}
#endif


#ifdef HAVE_SETENV
environment::~environment() {
}
#endif

const char *environment::getValue(const char *variable) const {
	char	*retval=NULL;
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex && pthread_mutex_lock(envmutex)) {
			return retval;
		}
	#endif
	retval=getenv(variable);
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex) {
			pthread_mutex_unlock(envmutex);
		}
	#endif
	return retval;
}

#ifdef HAVE_SETENV
bool environment::setValue(const char *variable, const char *value) {
	bool	retval=false;
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex && pthread_mutex_lock(envmutex)) {
			return retval;
		}
	#endif
	retval=(setenv(variable,value,1)!=-1);
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex) {
			pthread_mutex_unlock(envmutex);
		}
	#endif
	return retval;
}
#endif

bool environment::remove(const char *variable) {
#ifdef HAVE_UNSETENV
	bool	retval=false;
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex && pthread_mutex_lock(envmutex)) {
			return retval;
		}
	#endif
	unsetenv(variable);
	retval=true;
	#ifdef RUDIMENTS_HAS_THREADS
		if (envmutex) {
			pthread_mutex_unlock(envmutex);
		}
	#endif
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

#ifdef RUDIMENTS_HAS_THREADS
void environment::setMutex(pthread_mutex_t *mutex) {
	envmutex=mutex;
}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
