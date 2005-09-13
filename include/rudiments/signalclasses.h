// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SIGNALCLASSES_H
#define RUDIMENTS_SIGNALCLASSES_H

#include <rudiments/private/signalclassesincludes.h>

// Signals are a means for processes to interrupt each other.
//
// Many signals will cause a process to exit unless they are explicitly
// handled by a callback function.
//
// Different architectures have different signals, but most have the following
// signals:


#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif


// A signalset it just a collection of signals.  Many signal-related functions
// take signalsets for arguments.  The signalset class makes signalsets
// easy to manage.
class signalsetprivate;
class signalset {
	public:
			signalset();
			~signalset();
		bool	addSignal(int signum);
			// Add the signal "signum" to the set.
			// Returns true on success and false on failure.
		bool	addAllSignals();
			// Add all signals to the set.
			// Returns true on success and false on failure.
		bool	removeSignal(int signum);
			// Remove the signal "signum" from the set.
			// Returns true on success and false on failure.
		bool	removeAllSignals();
			// Remove all signals from the set.
			// Returns true on success and false on failure.
		int	signalIsInSet(int signum) const;
			// Returns 1 if the signal "signum" is in the 
			// set, 0 if it is not and -1 on error.

		sigset_t	*getSignalSet();
				// returns the set of signals

	#include <rudiments/private/signalset.h>
};



// The signalmanager class provides methods for sending, blocking and waiting 
// for signals.
class signalmanager {
	public:
		static	bool	sendSignal(pid_t processid, int signum);
				// Send singal "signum" to process "processid".
				// Returns true on success and false on failure.
		static	bool	raiseSignal(int signum);
				// Send singal "signum" to self.
				// Returns true on success and false on failure.
		static	bool	ignoreSignals(const sigset_t *sigset);
				// Ignore signal "signum".
				// Returns true on success and false on failure.
		static	bool	waitForSignals(const sigset_t *mask);
				// Wait until a signal NOT in the signal set 
				// "mask" is received.
				// Returns true on success and false on failure.
		static	bool	examineBlockedSignals(sigset_t *sigset);
				// Sets "sigset" to the set of signals that
				// were raised, but blocked during a call to 
				// waitForSignals().
				// Returns true on success and false on failure.
};


// The signalhandler class provides methods for catching and handling signals.
class signalhandlerprivate;
class signalhandler {
	public:
			signalhandler();
			signalhandler(int signum, void *handler);
			// Calls the setHandler() and handleSignal() methods
			// below during instantiation.
			~signalhandler();

		void	setHandler(void *handler);
			// Sets the function to call when the process
			// receives the signal.
		void	*getHandler();
			// Returns a pointer to the function that will be
			// called when the process receives the signal.

		bool	handleSignal(int signum);
			// Instructs the program to handle "signum" by calling
			// the handler set previously in setHandler().  May
			// be called multiple times to associate the same
			// handler with several signals.
			//
			// Returns true on success and false on failure.
		bool	handleSignal(int signum, signalhandler *oldhandler);
			// Instructs the program to handle "signum" by calling
			// the handler set previously in setHandler().  May
			// be called multiple times to associate the same
			// handler with several signals.  Populates oldhandler
			// with the values that were used to handle this signal
			// prior to this call.
			//
			// Returns true on success and false on failure.
		
		// Signal flags modify the behavior of the signal handling
		// process.
		void	removeAllFlags();
			// Remove all flags.
		void	addFlag(int flag);
			// Add "flag" to the set of flags modifying the
	 		// behavior of this signal handler.
		int	getFlags() const;
			// Return the set of flags modifying the behavior of 
			// this signal handler.


		// A signal mask is the set of signals that are blocked while
		// the signal handler function is being called.  Masking
		// signals can ensure that the function executes without
		// interruption.
		bool		addSignalToMask(int signum);
				// Add the signal "signum" to the mask.
				// Returns true on success and false on failure.
		bool		addAllSignalsToMask();
				// Mask all signals.
				// Returns true on success and false on failure.
		bool		removeSignalFromMask(int signum);
				// Remove the signal "signum" from the mask.
				// Returns true on success and false on failure.
		bool		removeAllSignalsFromMask();
				// Mask no signals.
				// Returns true on success and false on failure.
		int		signalIsInMask(int signum) const;
				// Returns 1 if the signal "signum" is in the 
				// set, 0 if it is not and -1 on error.
		void		setMask(sigset_t sigset);
				// Explicitly sets the mask to "sigset".
		sigset_t	getMask() const;
				// Returns the set of signals currently masked.

	#include <rudiments/private/signalhandler.h>

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
