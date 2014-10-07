// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#include <rudiments/error.h>
#include <rudiments/bytestring.h>
#if !defined(RUDIMENTS_HAVE_SIGACTION)
	#include <rudiments/linkedlist.h>
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

// signalset methods
signalset::signalset() {
	pvt=new signalsetprivate;
	removeAllSignals();
}

signalset::~signalset() {
	#if !defined(RUDIMENTS_HAVE_SIGACTION)
		pvt->_siglist.clear();
	#endif
	delete pvt;
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



// signalmanager methods
bool signalmanager::sendSignal(pid_t processid, int32_t signum) {
	#ifdef RUDIMENTS_HAVE_KILL
		int32_t	result;
		do {
			result=kill(processid,signum);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		// Windows doesn't support sending an arbitrary signal to
		// another process.  In fact, all signal handling is faked
		// in windows.  Windows sends messages between processes,
		// some of which correspond to what would be signals on unix.
		// It might be possible to implement this that way.
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool signalmanager::raiseSignal(int32_t signum) {
	int32_t	result;
	do {
		result=raise(signum);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

uint32_t signalmanager::alarm(uint32_t seconds) {
	#ifdef RUDIMENTS_HAVE_ALARM
		return ::alarm(seconds);
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

bool signalmanager::ignoreSignals(const signalset *sset) {
	#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
		bool	result=true;
		for (linkedlistnode< int32_t > *node=
				sset->pvt->_siglist.getFirst();
				node; node=node->getNext()) {

			if (node->getValue()==SIGINT || 
				node->getValue()==SIGTERM) {

				result=(result &&
					SetConsoleCtrlHandler(
						(PHANDLER_ROUTINE)NULL,
						TRUE)==TRUE);
			}
		}
		return result;
	#elif defined(RUDIMENTS_HAVE_SIGACTION)
		int32_t	result;
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
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool signalmanager::waitForSignals(const signalset *sset) {
	#ifdef RUDIMENTS_HAVE_KILL
		return (sigsuspend(&sset->pvt->_sigset)==-1);
	#else
		// see sendSignal above...
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool signalmanager::examineBlockedSignals(signalset *sset) {
	#ifdef RUDIMENTS_HAVE_KILL
		int32_t	result;
		do {
			result=sigpending(&sset->pvt->_sigset);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		// see sendSignal above...
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}


class signalhandlerprivate {
	friend class signalhandler;
	private:
		const signalset	*_sset;
		int32_t		_flags;
		void		(*_handler)(int32_t);
		#if defined(RUDIMENTS_HAVE_SIGACTION)
			struct sigaction	_handlerstruct;
		#endif
		#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
			static signalhandlerprivate	*_sigsegvinstance;
			static LONG	_sigsegvHandler(
						struct _EXCEPTION_POINTERS *ei);
		#endif
};

// signalhandler methods
signalhandler::signalhandler() {
	pvt=new signalhandlerprivate;
	pvt->_sset=NULL;
	pvt->_flags=0;
	pvt->_handler=NULL;
}

signalhandler::~signalhandler() {
	delete pvt;
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
signalhandlerprivate	*signalhandlerprivate::_sigsegvinstance=NULL;

LONG signalhandlerprivate::_sigsegvHandler(struct _EXCEPTION_POINTERS *ei) {
	_sigsegvinstance->_handler(SIGSEGV);
	return 1;
}
#endif

bool signalhandler::handleSignal(int32_t signum, signalhandler *oldhandler) {
	#if defined(RUDIMENTS_HAVE_SETCONSOLECTRLHANDLER)
		if (oldhandler) {
			// FIXME: set this somehow
			oldhandler->pvt->_handler=NULL;
		}
		switch (signum) {
			case SIGINT:
			case SIGTERM:
				return SetConsoleCtrlHandler(
					(PHANDLER_ROUTINE)pvt->_handler,
					TRUE)==TRUE;
			case SIGABRT:
			case SIGFPE:
			case SIGILL:
			case SIGSEGV:
				{
					pvt->_sigsegvinstance=this->pvt;
					SetUnhandledExceptionFilter(
							pvt->_sigsegvHandler);
				}
				return true;
		}
		error::setErrorNumber(ENOSYS);
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
		error::setErrorNumber(ENOSYS);
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
