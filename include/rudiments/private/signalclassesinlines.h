// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#include <rudiments/private/rudimentsinlines.h>

// signalset methods
RUDIMENTS_INLINE int signalset::addSignal(int signum) {
	return sigaddset(&sigset,signum);
}

RUDIMENTS_INLINE int signalset::addAllSignals() {
	return sigfillset(&sigset);
}

RUDIMENTS_INLINE int signalset::removeSignal(int signum) {
	return sigdelset(&sigset,signum);
}

RUDIMENTS_INLINE int signalset::removeAllSignals() {
	return sigemptyset(&sigset);
}

RUDIMENTS_INLINE int signalset::signalIsInSet(int signum) const {
	return sigismember(&sigset,signum);
}

RUDIMENTS_INLINE sigset_t *signalset::getSignalSet() {
	return &sigset;
}




// signalmanager methods
RUDIMENTS_INLINE int signalmanager::sendSignal(pid_t processid, int signum) {
	return kill(processid,signum);
}

RUDIMENTS_INLINE int signalmanager::raiseSignal(int signum) {
	return raise(signum);
}

RUDIMENTS_INLINE int signalmanager::ignoreSignals(const sigset_t *sigset) {
	return sigprocmask(SIG_SETMASK,sigset,NULL);
}

RUDIMENTS_INLINE int signalmanager::waitForSignals(const sigset_t *sigset) {
	return sigsuspend(sigset);
}

RUDIMENTS_INLINE int signalmanager::examineBlockedSignals(sigset_t *sigset) {
	return sigpending(sigset);
}




// signalhandler methods

RUDIMENTS_INLINE void signalhandler::setHandler(void *handler) {
	#ifdef SIGNAL_HANDLER_INT
		handlerstruct.sa_handler=(void(*)(int))handler;
	#else
		handlerstruct.sa_handler=(void(*)(void))handler;
	#endif
}

RUDIMENTS_INLINE void signalhandler::removeAllFlags() {
	handlerstruct.sa_flags=0;
}

RUDIMENTS_INLINE void signalhandler::addFlag(int flag) {
	handlerstruct.sa_flags|=flag;
}

RUDIMENTS_INLINE int signalhandler::addAllSignalsToMask() {
	return sigfillset(&handlerstruct.sa_mask);
}

RUDIMENTS_INLINE int signalhandler::addSignalToMask(int signum) {
	return sigaddset(&handlerstruct.sa_mask,signum);
}

RUDIMENTS_INLINE int signalhandler::removeSignalFromMask(int signum) {
	return sigdelset(&handlerstruct.sa_mask,signum);
}

RUDIMENTS_INLINE int signalhandler::removeAllSignalsFromMask() {
	return sigemptyset(&handlerstruct.sa_mask);
}

RUDIMENTS_INLINE int signalhandler::signalIsInMask(int signum) const {
	return sigismember(&handlerstruct.sa_mask,signum);
}

RUDIMENTS_INLINE sigset_t signalhandler::getMask() const {
	return handlerstruct.sa_mask;
}

RUDIMENTS_INLINE int signalhandler::getFlags() const {
	return handlerstruct.sa_flags;
}

RUDIMENTS_INLINE int signalhandler::handleSignal(int signum) {
	return sigaction(signum,&handlerstruct,(struct sigaction *)NULL);
}
