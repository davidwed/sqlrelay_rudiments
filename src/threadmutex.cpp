// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/threadmutex.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAVE_PTHREAD_MUTEX_T)
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
	error::clearError();
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

	/* NOTE:
	 * 
	 * There are various ways that a destructor can be called twice, due to
	 * the involvement of atexit, on_exit, and the dynamic linker finalizer.
	 *
	 * It can also happen as part of the classic shutdown loop:
	 *
	 *  a signal triggers shutdown
	 *  the shutdown handler begins deleting objects
	 *  another signal re-triggers shutdown
	 *  loop () {
	 *      the shutdown handler re-deletes the same objects
	 *      this causes a SIGSEGV
	 *      this triggers another shutdown
	 *  }
	 *
	 * Perfect application-level code might not show these kinds of
	 * behaviors, but it helps to take the following prophylactic measures,
	 * suggested by George Carrette:
	 *
	 *   * use a proxy to delete each object
	 *   * set the primary object to NULL before deleting the proxy, in
	 *     case deleting the proxy somehow causes a SIGSEGV and re-triggers
	 *     a shutdown handler
	 *   * if the process is involved then bail if the primary object has
	 *     already been set to NULL
	 */

	if (!pvt) {
		return;
	}
	error::clearError();

	pthread_mutex_t	*tmpmut=pvt->_mut;
	pvt->_mut = NULL;
	if (pvt->_destroy && tmpmut) {
		do {} while (pthread_mutex_destroy(tmpmut)==-1 &&
					error::getErrorNumber()==EINTR);
		delete tmpmut;
	}

	threadmutexprivate	*tmppvt=pvt;
	pvt=NULL;
	delete tmppvt;
}

bool threadmutex::lock() {
	int32_t	result;
	error::clearError();
	do {
		result=pthread_mutex_lock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool threadmutex::tryLock() {
	int32_t	result;
	error::clearError();
	do {
		result=pthread_mutex_trylock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool threadmutex::unlock() {
	int32_t	result;
	error::clearError();
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

	// see NOTE in non-Windows ~threadmutex()
	if (!pvt) {
		return;
	}

	HANDLE	tmpmut=pvt->_mut;
	pvt->_mut=NULL;
	if (pvt->_destroy && tmpmut) {
		CloseHandle(tmpmut);
	}

	threadmutexprivate	*tmppvt=pvt;
	pvt=NULL;
	delete tmppvt;
}

bool threadmutex::lock() {
	return WaitForSingleObject(pvt->_mut,INFINITE)==WAIT_OBJECT_0;
}

bool threadmutex::tryLock() {
	return WaitForSingleObject(pvt->_mut,0)==WAIT_OBJECT_0;
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
	return true;
}

bool threadmutex::tryLock() {
	return true;
}

bool threadmutex::unlock() {
	return true;
}

void *threadmutex::getInternalMutexStructure() {
	return NULL;
}
#endif
