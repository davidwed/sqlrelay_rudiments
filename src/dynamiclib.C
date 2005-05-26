// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/dynamiclib.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#include <dlfcn.h>
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#ifdef RUDIMENTS_HAS_THREADS
mutex	*dynamiclib::errormutex;
#endif

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
	do {
		handle=dlopen(library,flag);
	} while (!handle && error::getErrorNumber()==EINTR);
	return (handle!=NULL);
}

bool dynamiclib::close() {
	int	result;
	do {
		result=!dlclose(handle);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

void *dynamiclib::getSymbol(const char *symbol) const {
	void	*symhandle;
	do {
		symhandle=dlsym(handle,symbol);
	} while (!symhandle && error::getErrorNumber()==EINTR);
	return (handle)?symhandle:NULL;
}

char *dynamiclib::getError() const {
#ifdef RUDIMENTS_HAS_THREADS
	if (errormutex && !errormutex->lock()) {
		return NULL;
	}
#endif
	const char	*err;
	do {
		err=dlerror();
	} while (!err && error::getErrorNumber()==EINTR);
	char		*retval=NULL;
	if (err) {
		retval=charstring::duplicate(err);
	}
#ifdef RUDIMENTS_HAS_THREADS
	if (errormutex) {
		errormutex->unlock();
	}
#endif
	return retval;
}

#ifdef RUDIMENTS_HAS_THREADS
void dynamiclib::setErrorMutex(mutex *mtx) {
	errormutex=mtx;
}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
