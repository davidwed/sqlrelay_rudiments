// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>


// signalset methods
INLINE int signalset::addSignal(int signum) {
	return sigaddset(&sigset,signum);
}

INLINE int signalset::addAllSignals() {
	return sigfillset(&sigset);
}

INLINE int signalset::removeSignal(int signum) {
	return sigdelset(&sigset,signum);
}

INLINE int signalset::removeAllSignals() {
	return sigemptyset(&sigset);
}

INLINE int signalset::signalIsInSet(int signum) const {
	return sigismember(&sigset,signum);
}

INLINE sigset_t *signalset::getSignalSet() {
	return &sigset;
}




// signalmanager methods
INLINE int signalmanager::sendSignal(pid_t processid, int signum) {
	return kill(processid,signum);
}

INLINE int signalmanager::raiseSignal(int signum) {
	return raise(signum);
}

INLINE int signalmanager::ignoreSignals(const sigset_t *sigset) {
	return sigprocmask(SIG_SETMASK,sigset,NULL);
}

INLINE int signalmanager::waitForSignals(const sigset_t *sigset) {
	return sigsuspend(sigset);
}

INLINE int signalmanager::examineBlockedSignals(sigset_t *sigset) {
	return sigpending(sigset);
}




// signalhandler methods

INLINE void signalhandler::setHandler(void *handler) {
	#ifdef SIGNAL_HANDLER_INT
		handlerstruct.sa_handler=(void(*)(int))handler;
	#else
		handlerstruct.sa_handler=(void(*)(void))handler;
	#endif
}

INLINE void signalhandler::removeAllFlags() {
	handlerstruct.sa_flags=0;
}

INLINE void signalhandler::addFlag(int flag) {
	handlerstruct.sa_flags|=flag;
}

INLINE int signalhandler::addAllSignalsToMask() {
	return sigfillset(&handlerstruct.sa_mask);
}

INLINE int signalhandler::addSignalToMask(int signum) {
	return sigaddset(&handlerstruct.sa_mask,signum);
}

INLINE int signalhandler::removeSignalFromMask(int signum) {
	return sigdelset(&handlerstruct.sa_mask,signum);
}

INLINE int signalhandler::removeAllSignalsFromMask() {
	return sigemptyset(&handlerstruct.sa_mask);
}

INLINE int signalhandler::signalIsInMask(int signum) const {
	return sigismember(&handlerstruct.sa_mask,signum);
}

INLINE sigset_t signalhandler::getMask() const {
	return handlerstruct.sa_mask;
}

INLINE int signalhandler::getFlags() const {
	return handlerstruct.sa_flags;
}

INLINE int signalhandler::handleSignal(int signum) {
	return sigaction(signum,&handlerstruct,(struct sigaction *)NULL);
}
