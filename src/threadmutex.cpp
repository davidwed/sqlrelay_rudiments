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
  /* There are various ways that a destructor can be called twice, due to the involvement
     of atexit, on_exit, and the dynamic linker finalizer. You could see these problems
     as either SEGV or more mildly in valgrind. In the debugger you might see
     __cxa_finalize or __run_exit_handlers on the stack (in a glib environment).
     Perfect c++ code might not show these kinds of behaviors, but it does not hurt to take
     prophylactic measures within ones own runtime libraries.
     Due to the fact that this method gets called during shutdown, and there can be faults during
     shutdown that are difficult to debug, be extra careful about replacing pointers with NULL
     when objects are deleted, in fact we can replace the references with NULL before the deletion,
     in case the delete operator causes a fault that brings the code back into the shutdown.
  */
  if (pvt) {
    error::clearError();
    if (pvt->_mut) {
      do {} while (pthread_mutex_destroy(pvt->_mut)==-1 &&
                   error::getErrorNumber()==EINTR);
      pthread_mutex_t *tmp_mut = pvt->_mut;
      pvt->_mut = NULL;
      if (pvt->_destroy) {
        delete tmp_mut;
      }
    }
    threadmutexprivate *tmp_pvt = pvt;
    pvt = NULL;
    delete tmp_pvt;
  }
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
  if (pvt) {
    if (pvt->_destroy && pvt->_mut) {
      CloseHandle(pvt->_mut);
      pvt->_mut = NULL;
    }
    delete pvt;
    pvt = NULL;
  }
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
