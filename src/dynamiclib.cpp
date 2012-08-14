// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/dynamiclib.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_DLFCN_H
	#include <dlfcn.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class dynamiclibprivate {
	friend class dynamiclib;
	private:
		#if defined(RUDIMENTS_HAVE_DLOPEN)
			void		*_handle;
		#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
			HINSTANCE	_handle;
		#endif
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
#if defined(RUDIMENTS_HAVE_DLOPEN)
	int32_t	flag=(loaddependencies)?RTLD_NOW:RTLD_LAZY;
	if (global) {
		flag|=RTLD_GLOBAL;
	}
	do {
		pvt->_handle=dlopen(library,flag);
	} while (!pvt->_handle && error::getErrorNumber()==EINTR);
	return (pvt->_handle!=NULL);
#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
	pvt->_handle=LoadLibraryEx(library,NULL,
			(loaddependencies)?DONT_RESOLVE_DLL_REFERENCES:0);
	return (pvt->_handle)?true:false;
#else
	error::setErrorNumber(ENOSYS);
	return false;
#endif
}

bool dynamiclib::close() {
#if defined(RUDIMENTS_HAVE_DLOPEN)
	int32_t	result;
	do {
		result=dlclose(pvt->_handle);
	} while (result!=0 && error::getErrorNumber()==EINTR);
	return !result;
#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
	return FreeLibrary(pvt->_handle);
#else
	error::setErrorNumber(ENOSYS);
	return false;
#endif
}

void *dynamiclib::getSymbol(const char *symbol) const {
#if defined(RUDIMENTS_HAVE_DLOPEN)
	void	*symhandle;
	do {
		symhandle=dlsym(pvt->_handle,symbol);
	} while (!symhandle && error::getErrorNumber()==EINTR);
	return (pvt->_handle)?symhandle:NULL;
#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
	return (void *)GetProcAddress(pvt->_handle,symbol);
#else
	error::setErrorNumber(ENOSYS);
	return NULL;
#endif
}

char *dynamiclib::getError() const {
#if defined(RUDIMENTS_HAVE_DLOPEN)
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
#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
	char	*retval;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|
			FORMAT_MESSAGE_IGNORE_INSERTS|
			FORMAT_MESSAGE_MAX_WIDTH_MASK|
			FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
			(char *)&retval,
			0,
			NULL);
	return retval;
#else
	error::setErrorNumber(ENOSYS);
	return NULL;
#endif
}

void dynamiclib::setErrorMutex(mutex *mtx) {
	_errormutex=mtx;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
