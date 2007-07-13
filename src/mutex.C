// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/mutex.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class mutexprivate {
	friend class mutex;
	private:
		#if defined(RUDIMENTS_HAVE_PTHREAD_MUTEX_T)
		pthread_mutex_t	*_mut;
		#elif defined(RUDIMENTS_HAVE_CREATE_MUTEX)
		HANDLE		_mut;
		#endif
		bool			_destroy;
};

#if defined(RUDIMENTS_HAVE_PTHREAD_MUTEX_T)
mutex::mutex() {
	pvt=new mutexprivate;
	pvt->_mut=new pthread_mutex_t;
	do {} while (pthread_mutex_init(pvt->_mut,NULL)==-1 &&
				error::getErrorNumber()==EINTR);
	pvt->_destroy=true;
}

mutex::mutex(pthread_mutex_t *mut) {
	pvt=new mutexprivate;
	pvt->_mut=mut;
	pvt->_destroy=false;
}

mutex::~mutex() {
	do {} while (pthread_mutex_destroy(pvt->_mut)==-1 &&
				error::getErrorNumber()==EINTR);
	if (pvt->_destroy) {
		delete pvt->_mut;
	}
	delete pvt;
}

bool mutex::lock() {
	int	result;
	do {
		result=pthread_mutex_lock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool mutex::tryLock() {
	int	result;
	do {
		result=pthread_mutex_trylock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool mutex::unlock() {
	int	result;
	do {
		result=pthread_mutex_unlock(pvt->_mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

pthread_mutex_t *mutex::getMutex() {
	return pvt->_mut;
}

#elif defined(RUDIMENTS_HAVE_CREATE_MUTEX)

mutex::mutex() {
	pvt=new mutexprivate;
	pvt->_mut=CreateMutex(NULL,FALSE,NULL);
	pvt->_destroy=true;
}

mutex::mutex(HANDLE mut) {
	pvt=new mutexprivate;
	pvt->_mut=mut;
	pvt->_destroy=false;
}

mutex::~mutex() {
	if (pvt->_destroy) {
		CloseHandle(pvt->_mut);
	}
	delete pvt;
}

bool mutex::lock() {
	return WaitForSingleObject(pvt->_mut,INFINITE);
}

bool mutex::tryLock() {
	return WaitForSingleObject(pvt->_mut,0);
}

bool mutex::unlock() {
	return ReleaseMutex(pvt->_mut);
}

HANDLE mutex::getMutex() {
	return pvt->_mut;
}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
