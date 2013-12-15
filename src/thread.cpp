// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/thread.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAVE_PTHREAD_T)
	// to fix an odd situation on SCO with FSU pthreads
	#define _TIMESTRUC_T
	#include <pthread.h>
#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
	#include <rudiments/sys.h>
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
#endif

class threadprivate {
	friend class thread;
	private:
		#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pthread_t	_thr;
		pthread_attr_t	_attr;
		#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		HANDLE		_thr;
		size_t		_stacksize;
		#endif
		void		*(*_function)(void *);
		void		*_arg;
};

thread::thread() {
	pvt=new threadprivate;
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pthread_attr_init(&pvt->_attr);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		pvt->_stacksize=sys::getPageSize();
	#endif
	pvt->_function=NULL;
	pvt->_arg=NULL;
}

thread::~thread() {
	join(NULL);
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pthread_attr_destroy(&pvt->_attr);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		CloseHandle(pvt->_thr);
	#endif
	delete pvt;
}

void thread::setFunction(void *(*function)(void *), void *arg) {
	pvt->_function=function;
	pvt->_arg=arg;
}

bool thread::setStackSize(size_t stacksize) {
	error::setErrorNumber(0);
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		int	result=pthread_attr_setstacksize(&pvt->_attr,stacksize);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		pvt->_stacksize=stacksize;
		return true;
	#endif
}

bool thread::getStackSize(size_t *stacksize) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::setErrorNumber(0);
		int	result=pthread_attr_getstacksize(&pvt->_attr,stacksize);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		*stacksize=pvt->_stacksize;
		return true;
	#endif
}

bool thread::create() {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::setErrorNumber(0);
		int	result=pthread_create(&pvt->_thr,&pvt->_attr,
						pvt->_function,pvt->_arg);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		pvt->_thr=CreateThread(NULL,pvt->_stacksize,
					(LPTHREAD_START_ROUTINE)pvt->_function,
					pvt->_arg,0,NULL);
		return (pvt->_thr!=NULL);
	#endif
}

void thread::exit(int32_t status) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pthread_exit((void *)status);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		ExitThread((DWORD)status);
	#endif
}

bool thread::join(int32_t *status) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::setErrorNumber(0);
		int	result=pthread_join(pvt->_thr,(void **)status);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		if (WaitForSingleObject(pvt->_thr,INFINITE)==WAIT_FAILED) {
			return false;
		}
		DWORD	stat=0;
		if (GetExitCodeThread(pvt->_thr,&stat)==FALSE) {
			return false;
		}
		if (status) {
			*status=(int32_t)stat;
		}
		return true;
	#endif
}

bool thread::detach() {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::setErrorNumber(0);
		int	result=pthread_detach(pvt->_thr);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		return (CloseHandle(pvt->_thr)==TRUE);
	#endif
}
