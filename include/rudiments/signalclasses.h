// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef SIGNALCLASSES_H
#define SIGNALCLASSES_H

#include <rudiments/private/config.h>

#include <signal.h>
#include <sys/types.h>

// Signals are a means for processes to interrupt each other.
//
// Many signals will cause a process to exit unless they are explicitly
// handled by a callback function.
//
// Different architectures have different signals, but most have the following
// signals:



// A signalset it just a collection of signals.  Many signal-related functions
// take signalsets for arguments.  The signalset class makes signalsets
// easy to manage.
class signalset {
	public:
		int	addSignal(int signum);
			// add the signal "signum" to the set
		int	addAllSignals();
			// add all signals to the set
		int	removeSignal(int signum);
			// remove the signal "signum" from the set
		int	removeAllSignals();
			// remove all signals from the set
		int	signalIsInSet(int signum) const;
			// returns 1 if the signal "signum" is in the 
			// set and 0 if it is not

		sigset_t	*getSignalSet();
				// returns the set of signals

	#include <rudiments/private/signalset.h>
};



// The signalmanager class provides methods for sending, blocking and waiting 
// for signals.
class signalmanager {
	public:
		static	int	sendSignal(pid_t processid, int signum);
				// send singal "signum" to process "processid"
		static	int	raiseSignal(int signum);
				// send singal "signum" to self
		static	int	ignoreSignals(const sigset_t *sigset);
				// ignore signal "signum"
		static	int	waitForSignals(const sigset_t *mask);
				// wait until a signal NOT in the signal set 
				// "mask" is received
		static	int	examineBlockedSignals(sigset_t *sigset);
				// sets "sigset" to the set of signals that
				// were raised, but blocked during a call to 
				// waitForSignals()
};


// The signalhandler class provides methods for catching and handling signals.
class signalhandler {
	public:
			signalhandler();
			signalhandler(int signum, void *handler);
			// calls the setHandler() and handleSignal() methods
			// below during instantiation
		virtual	~signalhandler();

		void	setHandler(void *inthandler);
			// sets the function to call when the process
			// receives the signal

		int	handleSignal(int signum);
			// Instructs the program to handle "signum" by calling
			// the handler set previously in setHandler().  May
			// be called multiple times to associate the same
			// handler with several signals.
		
		// Signal flags modify the behavior of the signal handling
		// process.
		void	removeAllFlags();
			// remove all flags
		void	addFlag(int flag);
			// add "flag" to the set of flags modifying the
	 		// behavior of this signal handler
		int	getFlags() const;
			// return the set of flags modifying the behavior of 
			// this signal handler


		// A signal mask is the set of signals that are blocked while
		// the signal handler function is being called.  Masking
		// signals can ensure that the function executes without
		// interruption.
		int		addSignalToMask(int signum);
				// add the signal "signum" to the mask
		int		addAllSignalsToMask();
				// mask all signals
		int		removeSignalFromMask(int signum);
				// remove the signal "signum" from the mask
		int		removeAllSignalsFromMask();
				// mask no signals
		int		signalIsInMask(int signum) const;
				// returns 1 if the signal "signum" is masked 
				// and 0 if it is not
		void		setMask(sigset_t sigset);
				// explicitly sets the mask to "sigset"
		sigset_t	*getMask() const;
				// returns the set of signals currently masked

	#include <rudiments/private/signalhandler.h>

};

#ifdef ENABLE_INLINES
	#include <rudiments/private/signalclassesinlines.h>
#endif

#endif
