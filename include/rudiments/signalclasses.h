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
	friend class signalmanager;
	friend class signalhandler;
	public:
			signalset();
			~signalset();
		bool	addSignal(int32_t signum);
			// Add the signal "signum" to the set.
			// Returns true on success and false on failure.
		bool	addAllSignals();
			// Add all signals to the set.
			// Returns true on success and false on failure.
		bool	removeSignal(int32_t signum);
			// Remove the signal "signum" from the set.
			// Returns true on success and false on failure.
		bool	removeAllSignals();
			// Remove all signals from the set.
			// Returns true on success and false on failure.
		int32_t	signalIsInSet(int32_t signum) const;
			// Returns 1 if the signal "signum" is in the 
			// set, 0 if it is not and -1 on error.

	#include <rudiments/private/signalset.h>
};



// The signalmanager class provides methods for sending, blocking and waiting 
// for signals.
class signalmanager {
	public:
		static	bool	sendSignal(pid_t processid, int32_t signum);
				// Send signal "signum" to process "processid".
				// Returns true on success and false on failure.
		static	bool	raiseSignal(int32_t signum);
				// Send signal "signum" to self.
				// Returns true on success and false on failure.
		static	uint32_t	alarm(uint32_t seconds);
				// Sends signal SIGALRM to self after "seconds"
				// have elapsed.  If "seconds" is 0, the alarm
				// is disabled.  Calling this method cancels
				// any previously set alarm.
				//
				// Returns the number of seconds that were
				// remaining until any previously scheduled
				// alarm was to be delivered or 0 if there
				// was no previously scheduled alarm.
		static	bool	ignoreSignals(const signalset *sset);
				// Ignore signal "signum".
				// Returns true on success and false on failure.
		static	bool	waitForSignals(const signalset *mask);
				// Wait until a signal NOT in the signal set 
				// "mask" is received.
				// Returns true on success and false on failure.
		static	bool	examineBlockedSignals(signalset *sset);
				// Sets "sset" to the set of signals that
				// were raised, but blocked during a call to 
				// waitForSignals().
				// Returns true on success and false on failure.
};


// The signalhandler class provides methods for catching and handling signals.
class signalhandlerprivate;
class signalhandler {
	public:
			signalhandler();
			~signalhandler();

		void	setHandler(void (*handler)(int32_t));
			// Sets the function to call when the process
			// receives the signal.
		void    (*getHandler())(int32_t);
			// Returns a pointer to the function that will be
			// called when the process receives the signal.

		bool	handleSignal(int32_t signum);
			// Instructs the program to handle "signum" by calling
			// the handler set previously in setHandler().  May
			// be called multiple times to associate the same
			// handler with several signals.
			//
			// Returns true on success and false on failure.
		bool	handleSignal(int32_t signum, signalhandler *oldhandler);
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
		void	addFlag(int32_t flag);
			// Add "flag" to the set of flags modifying the
	 		// behavior of this signal handler.
		void	removeFlag(int32_t flag);
			// Remove "flag" from the set of flags modifying the
	 		// behavior of this signal handler.
		int32_t	getFlags() const;
			// Return the set of flags modifying the behavior of 
			// this signal handler.


		// A signal mask is the set of signals that are blocked while
		// the signal handler function is being called.  Masking
		// signals can ensure that the function executes without
		// interruption.
		void		setMask(const signalset *sset);
				// Explicitly sets the mask to "sset".
		const signalset	*getMask() const;
				// Returns the current signal mask.

		static	bool	isSignalHandlerIntUsed();
				// The function that you pass into setHandler()
				// must have a  void (*)(int32_t) signature.
				//
				// This method will return true if the
				// platform's signal handling framework allows
				// this class to pass the signal number
				// into the int parameter of that function
				// and false otherwise.
				//
				// In the event that the platform's signal
				// handling framework does not allow this
				// class to pass the signal number into the
				// int parameter of that function, a 0 will
				// be passed in.

	#include <rudiments/private/signalhandler.h>

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
