// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/dynamiclib.h>
#include <rudiments/charstring.h>

#include <dlfcn.h>

dynamiclib::dynamiclib() {
	handle=NULL;
#ifdef RUDIMENTS_HAS_THREADS
	errormutex=NULL;
#endif
}

dynamiclib::~dynamiclib() {
	close();
}

bool dynamiclib::open(const char *library, bool loaddependencies, bool global) {
	int	flag=(loaddependencies)?RTLD_NOW:RTLD_LAZY;
	if (global) {
		flag|=RTLD_GLOBAL;
	}
	handle=dlopen(library,flag);
	return (handle!=NULL);
}

bool dynamiclib::close() {
	return !dlclose(handle);
}

void *dynamiclib::getSymbol(const char *symbol) {
	return (handle)?dlsym(handle,symbol):NULL;
}

char *dynamiclib::getError() {
#ifdef RUDIMENTS_HAS_THREADS
	if (!(errormutex && pthread_mutex_lock(errormutex))) {
		return NULL;
	}
#endif
	char	*error=dlerror();
	char	*retval=NULL;
	if (error) {
		retval=charstring::duplicate(error);
	}
#ifdef RUDIMENTS_HAS_THREADS
	if (errormutex) {
		pthread_mutex_unlock(errormutex);
	}
#endif
	return retval;
}

#ifdef RUDIMENTS_HAS_THREADS
void dynamiclib::setErrorMutex(pthread_mutex_t *mutex) {
	errormutex=mutex;
}
#endif
