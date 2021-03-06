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

#include <stdio.h>

class dynamiclibprivate {
	friend class dynamiclib;
	private:
		#if defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
			HMODULE	_handle;
		#else
			void	*_handle;
		#endif
};

// LAME: not in the class
static	threadmutex	*_errormutex=NULL;

dynamiclib::dynamiclib() {
	pvt=new dynamiclibprivate;
	pvt->_handle=NULL;
}

dynamiclib::~dynamiclib() {

	// see NOTE in ~threadmutex()

	if (!pvt) {
		return;
	}

	close();

	dynamiclibprivate	*tmppvt=pvt;
	pvt=NULL;
	delete tmppvt;
}

bool dynamiclib::open(const char *library, bool loaddependencies, bool global) {
	#if defined(RUDIMENTS_HAVE_DLOPEN)
		int32_t	flag=(loaddependencies)?RTLD_NOW:RTLD_LAZY;
		#ifdef RTLD_GLOBAL
		if (global) {
			flag|=RTLD_GLOBAL;
		}
		#endif
		error::clearError();
		do {
			dlerror();
			pvt->_handle=dlopen(library,flag);
		} while (!pvt->_handle && error::getErrorNumber()==EINTR);
		return (pvt->_handle!=NULL);
	#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
		pvt->_handle=LoadLibraryEx(library,NULL,
			(loaddependencies)?0:DONT_RESOLVE_DLL_REFERENCES);
		return (pvt->_handle)?true:false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool dynamiclib::close() {
	if (!pvt->_handle) {
		return true;
	}
	bool	retval=false;
	#if defined(RUDIMENTS_HAVE_DLOPEN)
		int32_t	result;
		error::clearError();
		do {
			dlerror();
			result=dlclose(pvt->_handle);
		} while (result!=0 && error::getErrorNumber()==EINTR);
		retval=!result;
	#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
		retval=(FreeLibrary(pvt->_handle)==TRUE);
	#else
		RUDIMENTS_SET_ENOSYS
	#endif
	pvt->_handle=NULL;
	return retval;
}

void *dynamiclib::getSymbol(const char *symbol) const {
	#if defined(RUDIMENTS_HAVE_DLOPEN)
		void	*symhandle;
		error::clearError();
		do {
			dlerror();
			// What's this char * cast all about?
			// Really old versions of dlsym (linux libc5) define
			// dlsym with char * argument rather than const char *.
			// This works with old and new implementations.
			symhandle=dlsym(pvt->_handle,(char *)symbol);
		} while (!symhandle && error::getErrorNumber()==EINTR);
		return (pvt->_handle)?symhandle:NULL;
	#elif defined(RUDIMENTS_HAVE_LOADLIBRARYEX)
		return (void *)GetProcAddress(pvt->_handle,symbol);
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

char *dynamiclib::getError() const {
	#if defined(RUDIMENTS_HAVE_DLOPEN)
		if (_errormutex && !_errormutex->lock()) {
			return NULL;
		}
		const char	*err;
		error::clearError();
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
		char	*buffer;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|
				FORMAT_MESSAGE_IGNORE_INSERTS|
				FORMAT_MESSAGE_MAX_WIDTH_MASK|
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL,
				GetLastError(),
				MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				(char *)&buffer,
				0,
				NULL);

		// copy the buffer and return that so the message can be
		// freed using delete[] rather than having to use LocalFree
		char	*retval=NULL;
		if (charstring::compare(buffer,
				"The operation completed successfully. ")) {
			retval=charstring::duplicate(buffer);
		}
		LocalFree(buffer);
		return retval;
	#else
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

void dynamiclib::setErrorMutex(threadmutex *mtx) {
	_errormutex=mtx;
}
