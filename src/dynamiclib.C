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

class dynamiclibprivate {
	friend class dynamiclib;
	private:
		void	*_handle;
};

// LAME: not in the class
static	mutex	*_errormutex=NULL;

dynamiclib::dynamiclib() {
	pvt=new dynamiclibprivate;
	pvt->_handle=NULL;
}

dynamiclib::~dynamiclib() {
	close();
	delete pvt;
}

bool dynamiclib::open(const char *library, bool loaddependencies, bool global) {
	int	flag=(loaddependencies)?RTLD_NOW:RTLD_LAZY;
	if (global) {
		flag|=RTLD_GLOBAL;
	}
	do {
		pvt->_handle=dlopen(library,flag);
	} while (!pvt->_handle && error::getErrorNumber()==EINTR);
	return (pvt->_handle!=NULL);
}

bool dynamiclib::close() {
	int	result;
	do {
		result=!dlclose(pvt->_handle);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

void *dynamiclib::getSymbol(const char *symbol) const {
	void	*symhandle;
	do {
		symhandle=dlsym(pvt->_handle,symbol);
	} while (!symhandle && error::getErrorNumber()==EINTR);
	return (pvt->_handle)?symhandle:NULL;
}

char *dynamiclib::getError() const {
	if (_errormutex && !_errormutex->lock()) {
		return NULL;
	}
	const char	*err;
	do {
		err=dlerror();
	} while (!err && error::getErrorNumber()==EINTR);
	char		*retval=NULL;
	if (err) {
		retval=charstring::duplicate(err);
	}
	if (_errormutex) {
		_errormutex->unlock();
	}
	return retval;
}

void dynamiclib::setErrorMutex(mutex *mtx) {
	_errormutex=mtx;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
