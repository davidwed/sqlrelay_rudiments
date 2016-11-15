// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/thread.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>

// for pthread_kill
#include <signal.h>
#ifdef RUDIMENTS_HAVE_SYS_SIGNAL_H
	#include <sys/signal.h>
#endif

#if defined(RUDIMENTS_HAVE_PTHREAD_T)
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
		bool		_needtowait;
		bool		_retry;
};

thread::thread() {
	pvt=new threadprivate;
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pvt->_thr=0;
		pthread_attr_init(&pvt->_attr);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		pvt->_thr=INVALID_HANDLE_VALUE;
		pvt->_stacksize=sys::getMinThreadStackSize();
	#endif
	pvt->_needtowait=false;
	pvt->_retry=true;
}

thread::~thread() {
	wait(NULL);
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pthread_attr_destroy(&pvt->_attr);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		if (pvt->_thr!=INVALID_HANDLE_VALUE) {
			CloseHandle(pvt->_thr);
		}
	#endif
	delete pvt;
}

bool thread::setStackSize(size_t stacksize) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_ATTR_SETSTACKSIZE)
		error::clearError();
		int	result=pthread_attr_setstacksize(&pvt->_attr,stacksize);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		pvt->_stacksize=stacksize;
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool thread::getStackSize(size_t *stacksize) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_ATTR_SETSTACKSIZE)
		error::clearError();
		int	result=pthread_attr_getstacksize(&pvt->_attr,stacksize);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		*stacksize=pvt->_stacksize;
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool thread::spawn(void *(*function)(void *), void *arg, bool detached) {
	pvt->_needtowait=false;
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pvt->_thr=0;
		error::clearError();
		if (detached && pthread_attr_setdetachstate(&pvt->_attr,
						PTHREAD_CREATE_DETACHED)) {
			return false;
		}
		int	result=0;
		do {
			result=pthread_create(&pvt->_thr,
						&pvt->_attr,
						function,arg);
			if (!result) {
				pvt->_needtowait=!detached;
				return true;
			}
			snooze::macrosnooze(1);
			pvt->_thr=0;
		} while (result==EAGAIN && pvt->_retry);
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		pvt->_thr=INVALID_HANDLE_VALUE;
		pvt->_thr=CreateThread(NULL,pvt->_stacksize,
					(LPTHREAD_START_ROUTINE)function,
					arg,0,NULL);
		if (pvt->_thr==NULL || pvt->_thr==INVALID_HANDLE_VALUE) {
			pvt->_thr=NULL;
			return false;
		}
		if (detached && !detach()) {
			int32_t	status=1;
			exit(&status);
			pvt->_thr=NULL;
			return false;
		}
		pvt->_needtowait=!detached;
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void thread::exit(int32_t *status) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pthread_exit((void *)status);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		ExitThread((DWORD)*status);
	#else
		RUDIMENTS_SET_ENOSYS
	#endif
}

bool thread::wait(int32_t *status) {
	if (!pvt->_needtowait) {
		return true;
	}
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::clearError();
		if (pvt->_thr) {
			int32_t	*st=NULL;
			int	result=pthread_join(pvt->_thr,(void **)&st);
			if (result) {
				error::setErrorNumber(result);
				return false;
			}
			if (status) {
				*status=*st;
			}
		} else {
			if (status) {
				*status=0;
			}
		}
		pvt->_needtowait=false;
		return true;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		if (pvt->_thr!=INVALID_HANDLE_VALUE) {
			if (WaitForSingleObject(pvt->_thr,INFINITE)==
							WAIT_FAILED) {
				return false;
			}
			DWORD	stat=0;
			if (GetExitCodeThread(pvt->_thr,&stat)==FALSE) {
				return false;
			}
			if (status) {
				*status=(int32_t)stat;
			}
		} else {
			if (status) {
				*status=0;
			}
		}
		pvt->_needtowait=false;
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool thread::detach() {
	pvt->_needtowait=false;
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::clearError();
		if (pvt->_thr) {
			int	result=pthread_detach(pvt->_thr);
			if (result) {
				error::setErrorNumber(result);
				return false;
			}
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		bool	result=true;
		if (pvt->_thr!=INVALID_HANDLE_VALUE) {
			result=(CloseHandle(pvt->_thr)==TRUE);
		}
		pvt->_thr=INVALID_HANDLE_VALUE;
		return result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool thread::raiseSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		if (pvt->_thr) {
			return !pthread_kill(pvt->_thr,signum);
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		// FIXME: implement this for windows
		RUDIMENTS_SET_ENOSYS
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool thread::supportsThreads() {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T) || \
		defined(RUDIMENTS_HAVE_CREATETHREAD)
		return true;
	#else
		return false;
	#endif
}

void thread::retryFailedSpawn() {
	pvt->_retry=true;
}

void thread::dontRetryFailedSpawn() {
	pvt->_retry=false;
}

bool thread::getRetryFailedSpawn() {
	return pvt->_retry;
}
