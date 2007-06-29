// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <stddef.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <stdlib.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class signalsetprivate {
	friend class signalset;
	private:
		sigset_t	_sigset;
};

// signalset methods
signalset::signalset() {
	pvt=new signalsetprivate;
	rawbuffer::zero(&pvt->_sigset,sizeof(pvt->_sigset));
}

signalset::~signalset() {
	delete pvt;
}

bool signalset::addSignal(int signum) {
	return !sigaddset(&pvt->_sigset,signum);
}

bool signalset::addAllSignals() {
	return !sigfillset(&pvt->_sigset);
}

bool signalset::removeSignal(int signum) {
	return !sigdelset(&pvt->_sigset,signum);
}

bool signalset::removeAllSignals() {
	return !sigemptyset(&pvt->_sigset);
}

int signalset::signalIsInSet(int signum) const {
	return sigismember(&pvt->_sigset,signum);
}

sigset_t *signalset::getSignalSet() {
	return &pvt->_sigset;
}



// signalmanager methods
bool signalmanager::sendSignal(pid_t processid, int signum) {
	int	result;
	do {
		result=kill(processid,signum);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool signalmanager::raiseSignal(int signum) {
	int	result;
	do {
		result=raise(signum);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

unsigned int signalmanager::alarm(unsigned int seconds) {
	return ::alarm(seconds);
}

bool signalmanager::ignoreSignals(const sigset_t *sigset) {
	int	result;
	do {
		result=sigprocmask(SIG_SETMASK,sigset,NULL);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool signalmanager::waitForSignals(const sigset_t *sigset) {
	return (sigsuspend(sigset)==-1);
}

bool signalmanager::examineBlockedSignals(sigset_t *sigset) {
	int	result;
	do {
		result=sigpending(sigset);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}


class signalhandlerprivate {
	friend class signalhandler;
	private:
		struct	sigaction	_handlerstruct;
};

// signalhandler methods
signalhandler::signalhandler() {
	pvt=new signalhandlerprivate;
	removeAllSignalsFromMask();
	removeAllFlags();
}

signalhandler::signalhandler(int signum, void *handler) {
	pvt=new signalhandlerprivate;
	removeAllSignalsFromMask();
	removeAllFlags();
	setHandler(handler);
	handleSignal(signum);
}

signalhandler::~signalhandler() {
	delete pvt;
}

void signalhandler::setHandler(void *handler) {
	#ifdef RUDIMENTS_SIGNAL_HANDLER_INT
		pvt->_handlerstruct.sa_handler=(void(*)(int))handler;
	#else
		pvt->_handlerstruct.sa_handler=(void(*)(void))handler;
	#endif
}

void *signalhandler::getHandler() {
	return (void *)pvt->_handlerstruct.sa_handler;
}

void signalhandler::removeAllFlags() {
	pvt->_handlerstruct.sa_flags=0;
}

void signalhandler::addFlag(int flag) {
	pvt->_handlerstruct.sa_flags|=flag;
}

bool signalhandler::addAllSignalsToMask() {
	return !sigfillset(&pvt->_handlerstruct.sa_mask);
}

bool signalhandler::addSignalToMask(int signum) {
	return !sigaddset(&pvt->_handlerstruct.sa_mask,signum);
}

bool signalhandler::removeSignalFromMask(int signum) {
	return !sigdelset(&pvt->_handlerstruct.sa_mask,signum);
}

bool signalhandler::removeAllSignalsFromMask() {
	return !sigemptyset(&pvt->_handlerstruct.sa_mask);
}

int signalhandler::signalIsInMask(int signum) const {
	return sigismember(&pvt->_handlerstruct.sa_mask,signum);
}

sigset_t signalhandler::getMask() const {
	return pvt->_handlerstruct.sa_mask;
}

int signalhandler::getFlags() const {
	return pvt->_handlerstruct.sa_flags;
}

bool signalhandler::handleSignal(int signum) {
	int	result;
	do {
		result=sigaction(signum,&pvt->_handlerstruct,
				static_cast<struct sigaction *>(NULL));
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool signalhandler::handleSignal(int signum, signalhandler *oldhandler) {
	struct sigaction	oldaction;
	int			result;
	do {
		result=sigaction(signum,&pvt->_handlerstruct,&oldaction);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	rawbuffer::copy(&oldhandler->pvt->_handlerstruct,&oldaction,
					sizeof(struct sigaction));
	return !result;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
