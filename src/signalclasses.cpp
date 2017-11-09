// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#include <rudiments/error.h>
#include <rudiments/bytestring.h>
#if !defined(RUDIMENTS_HAVE_SIGACTION)
	#include <rudiments/linkedlist.h>
#endif
#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
	#include <rudiments/process.h>
#endif

#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#include <stdio.h>

class signalsetprivate {
	friend class signalset;
	friend class signalmanager;
	friend class signalhandler;
	private:
		#if defined(RUDIMENTS_HAVE_SIGACTION)
			sigset_t	_sigset;
		#else
			linkedlist< int32_t >	_siglist;
		#endif
};

class signalhandlerprivate {
	friend class signalmanager;
	friend class signalhandler;
	private:
		const signalset	*_sset;
		int32_t		_flags;
		void		(*_handler)(int32_t);
		#if defined(RUDIMENTS_HAVE_SIGACTION)
			struct sigaction	_handlerstruct;
		#endif
		#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
			void		(*_siginthandler)(int32_t);
			static signalhandlerprivate	*_ctrlinst;
			static BOOL	_ctrlHandler(DWORD ctrltype);
			static signalhandlerprivate	*_sigsegvinst;
			static LONG	_sigsegvFilter(
						struct _EXCEPTION_POINTERS *ei);
			static HANDLE 	_timer;
			static signalhandlerprivate	*_alarminst;
			static VOID CALLBACK	_alarmHandler(PVOID aPrameter,
						BOOLEAN timerorwaitfired);
		#endif
};

// signalset methods
signalset::signalset() {
	pvt=new signalsetprivate;
	removeAllSignals();
}

signalset::~signalset() {

	// set NOTE in ~threadmutex()
	if (!pvt) {
		return;
	}

	#if !defined(RUDIMENTS_HAVE_SIGACTION)
		pvt->_siglist.clear();
	#endif

	signalsetprivate	*tmppvt=pvt;
	pvt=NULL;
	delete tmppvt;
}

bool signalset::addSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigaddset(&pvt->_sigset,signum);
	#else
		if (!pvt->_siglist.find(signum)) {
			pvt->_siglist.append(signum);
		}
		return true;
	#endif
}

bool signalset::addAllSignals() {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigfillset(&pvt->_sigset);
	#else
		#ifdef SIGHUP
			addSignal(SIGHUP);
		#endif
		#ifdef SIGINT
			addSignal(SIGINT);
		#endif
		#ifdef SIGQUIT
			addSignal(SIGQUIT);
		#endif
		#ifdef SIGILL
			addSignal(SIGILL);
		#endif
		#ifdef SIGTRAP
			addSignal(SIGTRAP);
		#endif
		#ifdef SIGABRT
			addSignal(SIGABRT);
		#endif
		#ifdef SIGIOT
			addSignal(SIGIOT);
		#endif
		#ifdef SIGBUS
			addSignal(SIGBUS);
		#endif
		#ifdef SIGFPE
			addSignal(SIGFPE);
		#endif
		#ifdef SIGUSR1
			addSignal(SIGUSR1);
		#endif
		#ifdef SIGSEGV
			addSignal(SIGSEGV);
		#endif
		#ifdef SIGUSR2
			addSignal(SIGUSR2);
		#endif
		#ifdef SIGPIPE
			addSignal(SIGPIPE);
		#endif
		#ifdef SIGALRM
			addSignal(SIGALRM);
		#endif
		#ifdef SIGTERM
			addSignal(SIGTERM);
		#endif
		#ifdef SIGSTKFLT
			addSignal(SIGSTKFLT);
		#endif
		#ifdef SIGCHLD
			addSignal(SIGCHLD);
		#endif
		#ifdef SIGCONT
			addSignal(SIGCONT);
		#endif
		#ifdef SIGSTOP
			addSignal(SIGSTOP);
		#endif
		#ifdef SIGTSTP
			addSignal(SIGTSTP);
		#endif
		#ifdef SIGTTIN
			addSignal(SIGTTIN);
		#endif
		#ifdef SIGTTOU
			addSignal(SIGTTOU);
		#endif
		#ifdef SIGURG
			addSignal(SIGURG);
		#endif
		#ifdef SIGXCPU
			addSignal(SIGXCPU);
		#endif
		#ifdef SIGXFSZ
			addSignal(SIGXFSZ);
		#endif
		#ifdef SIGVTALRM
			addSignal(SIGVTALRM);
		#endif
		#ifdef SIGPROF
			addSignal(SIGPROF);
		#endif
		#ifdef SIGWINCH
			addSignal(SIGWINCH);
		#endif
		#ifdef SIGIO
			addSignal(SIGIO);
		#endif
		#ifdef SIGPOLL
			addSignal(SIGPOLL);
		#endif
		#ifdef SIGPWR
			addSignal(SIGPWR);
		#endif
		#ifdef SIGUNUSED
			addSignal(SIGUNUSED);
		#endif
		#ifdef SIGEMT
			addSignal(SIGEMT);
		#endif
		#ifdef SIGSYS
			addSignal(SIGSYS);
		#endif
		#ifdef SIGWAITING
			addSignal(SIGWAITING);
		#endif
		#ifdef SIGLWP
			addSignal(SIGLWP);
		#endif
		#ifdef SIGFREEZE
			addSignal(SIGFREEZE);
		#endif
		#ifdef SIGTHAW
			addSignal(SIGTHAW);
		#endif
		#ifdef SIGCANCEL
			addSignal(SIGCANCEL);
		#endif
		#ifdef SIGLOST
			addSignal(SIGLOST);
		#endif
		#ifdef _SIGRTMIN
			addSignal(_SIGRTMIN);
		#endif
		#ifdef _SIGRTMAX
			addSignal(_SIGRTMAX);
		#endif
		#ifdef SIGRTMIN
			addSignal(SIGRTMIN);
		#endif
		#ifdef SIGRTMAX
			addSignal(SIGRTMAX);
		#endif
		return true;
	#endif
}

bool signalset::removeSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigdelset(&pvt->_sigset,signum);
	#else
		pvt->_siglist.remove(signum);
		return true;
	#endif
}

bool signalset::removeShutDownSignals() {
	return removeSignal(SIGINT) &&
		removeSignal(SIGTERM)
		#ifdef SIGQUIT
		&& removeSignal(SIGQUIT)
		#endif
		#ifdef SIGHUP
		&& removeSignal(SIGHUP)
		#endif
		;
}

bool signalset::removeCrashSignals() {
	return removeSignal(SIGABRT) &&
		removeSignal(SIGFPE) &&
		removeSignal(SIGILL) &&
		removeSignal(SIGSEGV)
		#ifdef SIGBUS
		&& removeSignal(SIGBUS)
		#endif
		#ifdef SIGBUS
		&& removeSignal(SIGBUS)
		#endif
		#ifdef SIGIOT
		&& removeSignal(SIGIOT)
		#endif
		#ifdef SIGEMT
		&& removeSignal(SIGEMT)
		#endif
		#ifdef SIGSYS
		&& removeSignal(SIGSYS)
		#endif
		;
}

bool signalset::removeAllSignals() {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigemptyset(&pvt->_sigset);
	#else
		pvt->_siglist.clear();
		return true;
	#endif
}

int32_t signalset::signalIsInSet(int32_t signum) const {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return sigismember(&pvt->_sigset,signum);
	#else
		return (pvt->_siglist.find(signum)!=NULL);
	#endif
}

#ifdef RUDIMENTS_HAVE_CREATETIMERQUEUETIMER
HANDLE			signalhandlerprivate::_timer=INVALID_HANDLE_VALUE;
signalhandlerprivate	*signalhandlerprivate::_alarminst=NULL;

VOID signalhandlerprivate::_alarmHandler(PVOID parameter,
						BOOLEAN timerorwaitfired) {
	if (_alarminst && _alarminst->_handler) {
		_alarminst->_handler(SIGALRM);
	}
}
#endif

uint32_t signalmanager::alarm(uint32_t seconds) {
	#if defined(RUDIMENTS_HAVE_CREATETIMERQUEUETIMER) && \
						_WIN32_WINNT>=0x0500
		if (signalhandlerprivate::_timer!=INVALID_HANDLE_VALUE) {
			DeleteTimerQueueTimer(
				NULL,signalhandlerprivate::_timer,NULL);
			signalhandlerprivate::_timer=INVALID_HANDLE_VALUE;
		}
		if (seconds) {
			CreateTimerQueueTimer(
					&signalhandlerprivate::_timer,NULL,
					signalhandlerprivate::_alarmHandler,
					NULL,seconds*1000,
					0,WT_EXECUTEONLYONCE);
		}
		return 0;
	#elif defined(RUDIMENTS_HAVE_ALARM)
		return ::alarm(seconds);
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

bool signalmanager::ignoreSignals(const signalset *sset) {
	#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER) || \
		defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
		bool	result=true;
		for (linkedlistnode< int32_t > *node=
				sset->pvt->_siglist.getFirst();
				node; node=node->getNext()) {

			switch (node->getValue()) {
			#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
				case SIGINT:
				case SIGTERM:
					result=(result &&
						SetConsoleCtrlHandler(
							(PHANDLER_ROUTINE)NULL,
							TRUE)==TRUE);
			#endif
			#if defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
				case SIGABRT:
				case SIGFPE:
				case SIGILL:
				case SIGSEGV:
					SetUnhandledExceptionFilter(NULL);
			#endif
			}
		}
		return result;
	#elif defined(RUDIMENTS_HAVE_SIGACTION)
		int32_t	result;
		error::clearError();
		do {
			#ifdef RUDIMENTS_HAVE_PTHREAD_SIGMASK
				result=pthread_sigmask(SIG_SETMASK,
						&sset->pvt->_sigset,NULL);
			#else
				result=sigprocmask(SIG_SETMASK,
						&sset->pvt->_sigset,NULL);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_SIGNAL)
		bool	result=true;
		for (linkedlistnode< int32_t > *node=
				sset->pvt->_siglist.getFirst();
				node; node=node->getNext()) {
			result=(result &&
				(signal(node->getValue(),SIG_IGN)!=SIG_ERR));
		}
		return result;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool signalmanager::waitForSignals(const signalset *sset) {
	#ifdef RUDIMENTS_HAVE_KILL
		if (!sset) {
			signalset	none;
			return (sigsuspend(&none.pvt->_sigset)==-1);
		} else {
			return (sigsuspend(&sset->pvt->_sigset)==-1);
		}
	#else
		// see sendSignal above...
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool signalmanager::examineBlockedSignals(signalset *sset) {
	#ifdef RUDIMENTS_HAVE_KILL
		int32_t	result;
		error::clearError();
		do {
			result=sigpending(&sset->pvt->_sigset);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		// see sendSignal above...
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

// signalhandler methods
signalhandler::signalhandler() {
	pvt=new signalhandlerprivate;
	pvt->_sset=NULL;
	pvt->_flags=0;
	pvt->_handler=NULL;
	#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
		pvt->_siginthandler=NULL;
	#endif
}

signalhandler::~signalhandler() {
	// set NOTE in ~threadmutex()
	signalhandlerprivate	*tmppvt=pvt;
	pvt=NULL;
	delete tmppvt;
}

void signalhandler::setHandler(void (*handler)(int32_t)) {
	pvt->_handler=handler;
}

void (*signalhandler::getHandler())(int32_t) {
	return pvt->_handler;
}

void signalhandler::removeAllFlags() {
	pvt->_flags=0;
}

void signalhandler::addFlag(int32_t flag) {
	pvt->_flags|=flag;
}

void signalhandler::removeFlag(int32_t flag) {
	pvt->_flags&=(~flag);
}

int32_t signalhandler::getFlags() const {
	return pvt->_flags;
}

void signalhandler::setMask(const signalset *sset) {
	pvt->_sset=sset;
}

const signalset *signalhandler::getMask() const {
	return pvt->_sset;
}

bool signalhandler::handleSignal(int32_t signum) {
	return handleSignal(signum,NULL);
}

#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
signalhandlerprivate	*signalhandlerprivate::_ctrlinst=NULL;
BOOL signalhandlerprivate::_ctrlHandler(DWORD ctrltype) {
	switch (ctrltype) {
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:
			if (_ctrlinst->_siginthandler) {
				_ctrlinst->_siginthandler(SIGINT);
			} else {
				process::exit(0);
			}
			break;
		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			break;
	}
	return TRUE;
}

signalhandlerprivate	*signalhandlerprivate::_sigsegvinst=NULL;
LONG signalhandlerprivate::_sigsegvFilter(struct _EXCEPTION_POINTERS *ei) {
	_sigsegvinst->_handler(SIGSEGV);
	return EXCEPTION_CONTINUE_EXECUTION;
}
#endif

bool signalhandler::handleSignal(int32_t signum, signalhandler *oldhandler) {
	#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER) || \
		defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
		if (oldhandler) {
			// FIXME: set this somehow
			oldhandler->pvt->_handler=NULL;
		}
		switch (signum) {
			#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
			case SIGINT:
			case SIGTERM:
				pvt->_siginthandler=pvt->_handler;
				// FIXME: use pointer to member function
				// rather than this silliness
				signalhandlerprivate::_ctrlinst=this->pvt;
				return SetConsoleCtrlHandler(
					(PHANDLER_ROUTINE)
					signalhandlerprivate::_ctrlHandler,
					TRUE)==TRUE;
			#endif
			#if defined(RUDIMENTS_HAVE_SETUNHANDLEDEXCEPTIONFILTER)
			case SIGABRT:
			case SIGFPE:
			case SIGILL:
			case SIGSEGV:
				// FIXME: use pointer to member function
				// rather than this silliness
				signalhandlerprivate::_sigsegvinst=this->pvt;
				SetUnhandledExceptionFilter(
					(LPTOP_LEVEL_EXCEPTION_FILTER)
					signalhandlerprivate::_sigsegvFilter);
				return true;
			#endif
			#ifdef RUDIMENTS_HAVE_CREATETIMERQUEUETIMER
			case SIGALRM:
				// FIXME: use pointer to member function
				// rather than this silliness
				signalhandlerprivate::_alarminst=this->pvt;
				return true;
			#endif
		}
		RUDIMENTS_SET_ENOSYS
		return false;
	#elif defined(RUDIMENTS_HAVE_SIGACTION)
		struct sigaction	oldaction;
		int32_t			result;
		if (pvt->_sset) {
			bytestring::copy(&pvt->_handlerstruct.sa_mask,
						&pvt->_sset->pvt->_sigset,
						sizeof(sigset_t));
		} else {
			sigemptyset(&pvt->_handlerstruct.sa_mask);
		}
		pvt->_handlerstruct.sa_flags=pvt->_flags;
		#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
			pvt->_handlerstruct.sa_handler=
					(void(*)(int))pvt->_handler;
		#else
			pvt->_handlerstruct.sa_handler=
					(void(*)(void))pvt->_handler;
		#endif
		error::clearError();
		do {
			result=sigaction(signum,&pvt->_handlerstruct,
								&oldaction);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (oldhandler) {
			oldhandler->pvt->_handlerstruct=oldaction;
		}
		return !result;
	#elif defined(RUDIMENTS_HAVE_SIGNAL)
		void (*prev)(int)=signal(signum,pvt->_handler);
		if (oldhandler) {
			oldhandler->pvt->_handler=prev;
		}
		return (prev!=SIG_ERR);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool signalhandler::isSignalHandlerIntUsed() {
	#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
		return true;
	#else
		return false;
	#endif
}
