// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/thread.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

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
		void		*(*_function)(void *);
		void		*_arg;
		bool		_needtojoin;
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
	pvt->_function=NULL;
	pvt->_arg=NULL;
	pvt->_needtojoin=false;
}

thread::~thread() {
	join(NULL);
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		pthread_attr_destroy(&pvt->_attr);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		if (pvt->_thr!=INVALID_HANDLE_VALUE) {
			CloseHandle(pvt->_thr);
		}
	#endif
	delete pvt;
}

void thread::setFunction(void *(*function)(void *)) {
	pvt->_function=function;
}

void thread::setArgument(void *arg) {
	pvt->_arg=arg;
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

bool thread::run() {
	return run(NULL);
}

bool thread::run(void *arg) {
	pvt->_arg=arg;
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::clearError();
		int	result=pthread_create(&pvt->_thr,&pvt->_attr,
						pvt->_function,pvt->_arg);
		if (!result) {
			pvt->_needtojoin=true;
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		pvt->_thr=CreateThread(NULL,pvt->_stacksize,
					(LPTHREAD_START_ROUTINE)pvt->_function,
					pvt->_arg,0,NULL);
		if (pvt->_thr!=NULL && pvt->_thr!=INVALID_HANDLE_VALUE) {
			pvt->_needtojoin=true;
			return true;
		}
		return false;
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

bool thread::join(int32_t *status) {
	if (!pvt->_needtojoin) {
		return true;
	}
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::clearError();
		int32_t	*st=NULL;
		int	result=pthread_join(pvt->_thr,(void **)&st);
		if (!result) {
			if (status) {
				*status=*st;
			}
			pvt->_needtojoin=false;
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
		pvt->_needtojoin=false;
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool thread::detach() {
	pvt->_needtojoin=false;
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		error::clearError();
		int	result=pthread_detach(pvt->_thr);
		if (!result) {
			return true;
		}
		error::setErrorNumber(result);
		return false;
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		bool	result=true;(CloseHandle(pvt->_thr)==TRUE);
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

bool thread::cancel() {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		return !pthread_cancel(pvt->_thr);
	#elif defined(RUDIMENTS_HAVE_CREATETHREAD)
		// FIXME: implement this for windows
		RUDIMENTS_SET_ENOSYS
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool thread::raiseSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_PTHREAD_T)
		return !pthread_kill(pvt->_thr,signum);
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
