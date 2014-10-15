// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/threadmutex.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAVE_PTHREAD_MUTEX_T)
	// to fix an odd situation on SCO with FSU pthreads
	//#define _TIMESTRUC_T
	#include <pthread.h>
#elif defined(RUDIMENTS_HAVE_CREATE_MUTEX)
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
#else
	#ifdef RUDIMENTS_HAVE_STDLIB_H
		#include <stdlib.h>
	#endif
#endif

class threadmutexprivate {
	friend class threadmutex;
	private:
		#if defined(RUDIMENTS_HAVE_PTHREAD_MUTEX_T)
		pthread_mutex_t	*_mut;
		#elif defined(RUDIMENTS_HAVE_CREATE_MUTEX)
		HANDLE		_mut;
		#endif
		bool		_destroy;
};

#if defined(RUDIMENTS_HAVE_PTHREAD_MUTEX_T)
threadmutex::threadmutex() {

	pvt=new threadmutexprivate;
	pvt->_mut=new pthread_mutex_t;
	do {} while (pthread_mutex_init(pvt->_mut,NULL)==-1 &&
				error::getErrorNumber()==EINTR);
	pvt->_destroy=true;
}

threadmutex::threadmutex(void *mut) {
	pvt=new threadmutexprivate;
	pvt->_mut=(pthread_mutex_t *)mut;
	pvt->_destroy=false;
}

threadmutex::~threadmutex() {
	do {} while (pthread_mutex_destroy(pvt->_mut)==-1 &&
				error::getErrorNumber()==EINTR);
	if (pvt->_destroy) {
		delete pvt->_mut;
	}
	delete pvt;
}

bool threadmutex::lock() {
	int32_t	result;
	do {
		result=pthread_mutex_lock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool threadmutex::tryLock() {
	int32_t	result;
	do {
		result=pthread_mutex_trylock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool threadmutex::unlock() {
	int32_t	result;
	do {
		result=pthread_mutex_unlock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

void *threadmutex::getInternalMutexStructure() {
	return (void *)pvt->_mut;
}

#elif defined(RUDIMENTS_HAVE_CREATE_MUTEX)

threadmutex::threadmutex() {
	pvt=new threadmutexprivate;
	pvt->_mut=CreateMutex(NULL,FALSE,NULL);
	pvt->_destroy=true;
}

threadmutex::threadmutex(void *mut) {
	pvt=new threadmutexprivate;
	pvt->_mut=(HANDLE)mut;
	pvt->_destroy=false;
}

threadmutex::~threadmutex() {
	if (pvt->_destroy) {
		CloseHandle(pvt->_mut);
	}
	delete pvt;
}

bool threadmutex::lock() {
	return WaitForSingleObject(pvt->_mut,INFINITE)!=0;
}

bool threadmutex::tryLock() {
	return WaitForSingleObject(pvt->_mut,0)!=0;
}

bool threadmutex::unlock() {
	return ReleaseMutex(pvt->_mut)!=0;
}

void *threadmutex::getInternalMutexStructure() {
	return (void *)(pvt->_mut);
}

#else

threadmutex::threadmutex() {
}

threadmutex::threadmutex(void *mut) {
}

threadmutex::~threadmutex() {
}

bool threadmutex::lock() {
	error::setErrorNumber(ENOSYS);
	return false;
}

bool threadmutex::tryLock() {
	error::setErrorNumber(ENOSYS);
	return false;
}

bool threadmutex::unlock() {
	error::setErrorNumber(ENOSYS);
	return false;
}

void *threadmutex::getInternalMutexStructure() {
	return NULL;
}
#endif
