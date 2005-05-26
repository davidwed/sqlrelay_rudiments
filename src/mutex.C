// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/mutex.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#ifdef HAVE_PTHREAD_MUTEX_T
mutex::mutex() {
	mut=new pthread_mutex_t;
	do {} while (pthread_mutex_init(mut,NULL)==-1 &&
				error::getErrorNumber()==EINTR);
	destroy=true;
}

mutex::mutex(pthread_mutex_t *mut) {
	this->mut=mut;
	destroy=false;
}

mutex::~mutex() {
	do {} while (pthread_mutex_destroy(mut)==-1 &&
				error::getErrorNumber()==EINTR);
	if (destroy) {
		delete mut;
	}
}

bool mutex::lock() {
	int	result;
	do {
		result=pthread_mutex_lock(mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool mutex::tryLock() {
	int	result;
	do {
		result=pthread_mutex_trylock(mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool mutex::unlock() {
	int	result;
	do {
		result=pthread_mutex_unlock(mut);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

pthread_mutex_t *mutex::getMutex() {
	return mut;
}
#endif

#ifdef HAVE_CREATE_MUTEX
mutex::mutex() {
	mut=CreateMutex(NULL,FALSE,NULL);
	destroy=true;
}

mutex::mutex(HANDLE mut) {
	this->mut=mut;
	destroy=false;
}

mutex::~mutex() {
	if (destroy) {
		CloseHandle(mut);
	}
}

bool mutex::lock() {
	return WaitForSingleObject(mut,INFINITE);
}

bool mutex::tryLock() {
	return WaitForSingleObject(mut,0);
}

bool mutex::unlock() {
	return ReleaseMutex(mut);
}

HANDLE mutex::getMutex() {
	return mut;
}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
