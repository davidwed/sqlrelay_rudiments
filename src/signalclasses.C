// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/signalclasses.h>
#include <rudiments/rawbuffer.h>

#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>



// signalset methods
bool signalset::addSignal(int signum) {
	return !sigaddset(&sigset,signum);
}

bool signalset::addAllSignals() {
	return !sigfillset(&sigset);
}

bool signalset::removeSignal(int signum) {
	return !sigdelset(&sigset,signum);
}

bool signalset::removeAllSignals() {
	return !sigemptyset(&sigset);
}

int signalset::signalIsInSet(int signum) const {
	return sigismember(&sigset,signum);
}

sigset_t *signalset::getSignalSet() {
	return &sigset;
}



// signalmanager methods
bool signalmanager::sendSignal(pid_t processid, int signum) {
	return !kill(processid,signum);
}

bool signalmanager::raiseSignal(int signum) {
	return !raise(signum);
}

bool signalmanager::ignoreSignals(const sigset_t *sigset) {
	return !sigprocmask(SIG_SETMASK,sigset,NULL);
}

bool signalmanager::waitForSignals(const sigset_t *sigset) {
	return (sigsuspend(sigset)==-1);
}

bool signalmanager::examineBlockedSignals(sigset_t *sigset) {
	return !sigpending(sigset);
}



// signalhandler methods
signalhandler::signalhandler() {
	removeAllSignalsFromMask();
	removeAllFlags();
}

signalhandler::signalhandler(int signum, void *handler) {
	removeAllSignalsFromMask();
	removeAllFlags();
	setHandler(handler);
	handleSignal(signum);
}

void signalhandler::setHandler(void *handler) {
	#ifdef SIGNAL_HANDLER_INT
		handlerstruct.sa_handler=(void(*)(int))handler;
	#else
		handlerstruct.sa_handler=(void(*)(void))handler;
	#endif
}

void *signalhandler::getHandler() {
	return (void *)handlerstruct.sa_handler;
}

void signalhandler::removeAllFlags() {
	handlerstruct.sa_flags=0;
}

void signalhandler::addFlag(int flag) {
	handlerstruct.sa_flags|=flag;
}

bool signalhandler::addAllSignalsToMask() {
	return !sigfillset(&handlerstruct.sa_mask);
}

bool signalhandler::addSignalToMask(int signum) {
	return !sigaddset(&handlerstruct.sa_mask,signum);
}

bool signalhandler::removeSignalFromMask(int signum) {
	return !sigdelset(&handlerstruct.sa_mask,signum);
}

bool signalhandler::removeAllSignalsFromMask() {
	return !sigemptyset(&handlerstruct.sa_mask);
}

int signalhandler::signalIsInMask(int signum) const {
	return sigismember(&handlerstruct.sa_mask,signum);
}

sigset_t signalhandler::getMask() const {
	return handlerstruct.sa_mask;
}

int signalhandler::getFlags() const {
	return handlerstruct.sa_flags;
}

bool signalhandler::handleSignal(int signum) {
	return !sigaction(signum,&handlerstruct,(struct sigaction *)NULL);
}

bool signalhandler::handleSignal(int signum, signalhandler *oldhandler) {
	struct sigaction	oldaction;
	bool	retval=!sigaction(signum,&handlerstruct,&oldaction);
	rawbuffer::copy((void *)&oldhandler->handlerstruct,
			(void *)&oldaction,sizeof(struct sigaction));
	return retval;
}
