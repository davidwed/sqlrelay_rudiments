// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>


// signalset methods
inline int signalset::addSignal(int signum) {
	return sigaddset(&sigset,signum);
}

inline int signalset::addAllSignals() {
	return sigfillset(&sigset);
}

inline int signalset::removeSignal(int signum) {
	return sigdelset(&sigset,signum);
}

inline int signalset::removeAllSignals() {
	return sigemptyset(&sigset);
}

inline int signalset::signalIsInSet(int signum) const {
	return sigismember(&sigset,signum);
}

inline sigset_t *signalset::getSignalSet() {
	return &sigset;
}




// signalmanager methods
inline int signalmanager::sendSignal(pid_t processid, int signum) {
	return kill(processid,signum);
}

inline int signalmanager::raiseSignal(int signum) {
	return raise(signum);
}

inline int signalmanager::ignoreSignals(const sigset_t *sigset) {
	return sigprocmask(SIG_SETMASK,sigset,NULL);
}

inline int signalmanager::waitForSignals(const sigset_t *sigset) {
	return sigsuspend(sigset);
}

inline int signalmanager::examineBlockedSignals(sigset_t *sigset) {
	return sigpending(sigset);
}




// signalhandler methods
inline signalhandler::~signalhandler() {
	delete handlerstruct;
}

inline void signalhandler::setHandler(void *handler) {
	#ifdef SIGNAL_HANDLER_INT
		handlerstruct->sa_handler=(void(*)(int))handler;
	#else
		handlerstruct->sa_handler=(void(*)(void))handler;
	#endif
}

inline void signalhandler::removeAllFlags() {
	handlerstruct->sa_flags=0;
}

inline void signalhandler::addFlag(int flag) {
	handlerstruct->sa_flags|=flag;
}

inline int signalhandler::addAllSignalsToMask() {
	return sigfillset(&handlerstruct->sa_mask);
}

inline int signalhandler::addSignalToMask(int signum) {
	return sigaddset(&handlerstruct->sa_mask,signum);
}

inline int signalhandler::removeSignalFromMask(int signum) {
	return sigdelset(&handlerstruct->sa_mask,signum);
}

inline int signalhandler::removeAllSignalsFromMask() {
	return sigemptyset(&handlerstruct->sa_mask);
}

inline int signalhandler::signalIsInMask(int signum) const {
	return sigismember(&handlerstruct->sa_mask,signum);
}

inline sigset_t *signalhandler::getMask() const {
	return &handlerstruct->sa_mask;
}

inline int signalhandler::getFlags() const {
	return handlerstruct->sa_flags;
}

inline int signalhandler::handleSignal(int signum) {
	return sigaction(signum,handlerstruct,(struct sigaction *)NULL);
}
