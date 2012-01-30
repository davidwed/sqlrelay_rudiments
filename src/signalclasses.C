// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#include <rudiments/error.h>
#include <rudiments/rawbuffer.h>
#if !defined(RUDIMENTS_HAVE_SIGACTION)
	#include <rudiments/linkedlist.h>
#endif

#include <stddef.h>
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <stdlib.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

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
		if (!pvt->_siglist.getNodeByData(signum)) {
			pvt->_siglist.append(signum);
		}
		return true;
	#endif
}

bool signalset::addAllSignals() {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigfillset(&pvt->_sigset);
	#else
		#ifdef HAVE_SIGHUP
			addSignal(SIGHUP);
		#endif
		#ifdef HAVE_SIGINT
			addSignal(SIGINT);
		#endif
		#ifdef HAVE_SIGQUIT
			addSignal(SIGQUIT);
		#endif
		#ifdef HAVE_SIGILL
			addSignal(SIGILL);
		#endif
		#ifdef HAVE_SIGTRAP
			addSignal(SIGTRAP);
		#endif
		#ifdef HAVE_SIGABRT
			addSignal(SIGABRT);
		#endif
		#ifdef HAVE_SIGIOT
			addSignal(SIGIOT);
		#endif
		#ifdef HAVE_SIGBUS
			addSignal(SIGBUS);
		#endif
		#ifdef HAVE_SIGFPE
			addSignal(SIGFPE);
		#endif
		#ifdef HAVE_SIGUSR1
			addSignal(SIGUSR1);
		#endif
		#ifdef HAVE_SIGSEGV
			addSignal(SIGSEGV);
		#endif
		#ifdef HAVE_SIGUSR2
			addSignal(SIGUSR2);
		#endif
		#ifdef HAVE_SIGPIPE
			addSignal(SIGPIPE);
		#endif
		#ifdef HAVE_SIGALRM
			addSignal(SIGALRM);
		#endif
		#ifdef HAVE_SIGTERM
			addSignal(SIGTERM);
		#endif
		#ifdef HAVE_SIGSTKFLT
			addSignal(SIGSTKFLT);
		#endif
		#ifdef HAVE_SIGCHLD
			addSignal(SIGCHLD);
		#endif
		#ifdef HAVE_SIGCONT
			addSignal(SIGCONT);
		#endif
		#ifdef HAVE_SIGSTOP
			addSignal(SIGSTOP);
		#endif
		#ifdef HAVE_SIGTSTP
			addSignal(SIGTSTP);
		#endif
		#ifdef HAVE_SIGTTIN
			addSignal(SIGTTIN);
		#endif
		#ifdef HAVE_SIGTTOU
			addSignal(SIGTTOU);
		#endif
		#ifdef HAVE_SIGURG
			addSignal(SIGURG);
		#endif
		#ifdef HAVE_SIGXCPU
			addSignal(SIGXCPU);
		#endif
		#ifdef HAVE_SIGXFSZ
			addSignal(SIGXFSZ);
		#endif
		#ifdef HAVE_SIGVTALRM
			addSignal(SIGVTALRM);
		#endif
		#ifdef HAVE_SIGPROF
			addSignal(SIGPROF);
		#endif
		#ifdef HAVE_SIGWINCH
			addSignal(SIGWINCH);
		#endif
		#ifdef HAVE_SIGIO
			addSignal(SIGIO);
		#endif
		#ifdef HAVE_SIGPOLL
			addSignal(SIGPOLL);
		#endif
		#ifdef HAVE_SIGPWR
			addSignal(SIGPWR);
		#endif
		#ifdef HAVE_SIGUNUSED
			addSignal(SIGUNUSED);
		#endif
		#ifdef HAVE_SIGEMT
			addSignal(SIGEMT);
		#endif
		#ifdef HAVE_SIGSYS
			addSignal(SIGSYS);
		#endif
		#ifdef HAVE_SIGWAITING
			addSignal(SIGWAITING);
		#endif
		#ifdef HAVE_SIGLWP
			addSignal(SIGLWP);
		#endif
		#ifdef HAVE_SIGFREEZE
			addSignal(SIGFREEZE);
		#endif
		#ifdef HAVE_SIGTHAW
			addSignal(SIGTHAW);
		#endif
		#ifdef HAVE_SIGCANCEL
			addSignal(SIGCANCEL);
		#endif
		#ifdef HAVE_SIGLOST
			addSignal(SIGLOST);
		#endif
		#ifdef HAVE__SIGRTMIN
			addSignal(_SIGRTMIN);
		#endif
		#ifdef HAVE__SIGRTMAX
			addSignal(_SIGRTMAX);
		#endif
		#ifdef HAVE_SIGRTMIN
			addSignal(SIGRTMIN);
		#endif
		#ifdef HAVE_SIGRTMAX
			addSignal(SIGRTMAX);
		#endif
		return false;
	#endif
}

bool signalset::removeSignal(int32_t signum) {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		return !sigdelset(&pvt->_sigset,signum);
	#else
		pvt->_siglist.removeByData(signum);
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
		return (pvt->_siglist.getNodeByData(signum)!=NULL);
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
	#if defined(RUDIMENTS_HAVE_SIGACTION)
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
	#else
		bool	result=true;
		for (linkedlistnode< int32_t > *node=
				sset->pvt->_siglist.getFirstNode();
					node; node->getNext()) {
			result=(result &&
				(signal(node->getData(),SIG_IGN)!=SIG_ERR));
		}
		return result;
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
		#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
			void		(*_handler)(int);
		#else
			void		(*_handler)(void);
		#endif
		#if defined(RUDIMENTS_HAVE_SIGACTION)
			struct sigaction	_handlerstruct;
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

void signalhandler::setHandler(void (*handler)(int)) {
	#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
		pvt->_handler=handler;
	#else
		pvt->_handler=(void(*)(void))handler;
	#endif
}

void (*signalhandler::getHandler())(int) {
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

bool signalhandler::handleSignal(int32_t signum, signalhandler *oldhandler) {
	#if defined(RUDIMENTS_HAVE_SIGACTION)
		struct sigaction	oldaction;
		int32_t			result;
		if (pvt->_sset) {
			rawbuffer::copy(&pvt->_handlerstruct.sa_mask,
						&pvt->_sset->pvt->_sigset,
						sizeof(sigset_t));
		} else {
			sigemptyset(&pvt->_handlerstruct.sa_mask);
		}
		pvt->_handlerstruct.sa_flags=pvt->_flags;
		pvt->_handlerstruct.sa_handler=pvt->_handler;
		do {
			result=sigaction(signum,&pvt->_handlerstruct,
								&oldaction);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		if (oldhandler) {
			oldhandler->pvt->_handlerstruct=oldaction;
		}
		return !result;
	#else
		void (*prev)(int)=signal(signum,pvt->_handler);
		if (oldhandler) {
			oldhandler->pvt->_handler=prev;
		}
		return (prev!=SIG_ERR);
	#endif
}

bool signalhandler::isSignalHandlerIntUsed() {
	#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
		return true;
	#else
		return false;
	#endif
}


#ifdef RUDIMENTS_NAMESPACE
}
#endif
