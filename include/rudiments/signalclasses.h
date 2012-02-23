// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SIGNALCLASSES_H
#define RUDIMENTS_SIGNALCLASSES_H

#include <rudiments/private/signalclassesincludes.h>

/** Signals are a means for processes to interrupt each other.
 * 
 *  Many signals will cause a process to exit unless they are explicitly
 *  handled by a callback function. */


#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif


/** A signalset it just a collection of signals.  Many signal-related functions
 *  take signalsets for arguments.  The signalset class makes signalsets
 *  easy to manage. */
class signalsetprivate;
class RUDIMENTS_DLLSPEC signalset {
	friend class signalmanager;
	friend class signalhandler;
	public:

		/** Creates an instance of the signalset class. */
		signalset();

		/** Deletes this instance of the signalset class. */
		~signalset();

		/** Add the signal "signum" to the set.
		 *  Returns true on success and false on failure. */
		bool	addSignal(int32_t signum);

		/** Add all signals to the set.
		 *  Returns true on success and false on failure. */
		bool	addAllSignals();

		/** Remove the signal "signum" from the set.
		 *  Returns true on success and false on failure. */
		bool	removeSignal(int32_t signum);

		/** Remove all signals from the set.
		 *  Returns true on success and false on failure. */
		bool	removeAllSignals();

		/** Returns 1 if the signal "signum" is in the 
		 *  set, 0 if it is not and -1 on error. */
		int32_t	signalIsInSet(int32_t signum) const;

	#include <rudiments/private/signalset.h>
};



/** The signalmanager class provides methods for sending, blocking and waiting 
 *  for signals. */
class RUDIMENTS_DLLSPEC signalmanager {
	public:
		/** Send signal "signum" to process "processid".
		 *  Returns true on success and false on failure. */
		static	bool	sendSignal(pid_t processid, int32_t signum);

		/** Send signal "signum" to self.
		 *  Returns true on success and false on failure. */
		static	bool	raiseSignal(int32_t signum);

		/** Sends signal SIGALRM to self after "seconds"
		 *  have elapsed.  If "seconds" is 0, the alarm
		 *  is disabled.  Calling this method cancels
		 *  any previously set alarm.
		 * 
		 *  Returns the number of seconds that were
		 *  remaining until any previously scheduled
		 *  alarm was to be delivered or 0 if there
		 *  was no previously scheduled alarm. */
		static	uint32_t	alarm(uint32_t seconds);

		/** Ignore signal "signum".
		 *  Returns true on success and false on failure. */
		static	bool	ignoreSignals(const signalset *sset);

		/** Wait until a signal NOT in the signal set 
		 *  "mask" is received.
		 *  Returns true on success and false on failure. */
		static	bool	waitForSignals(const signalset *mask);

		/** Sets "sset" to the set of signals that
		 *  were raised, but blocked during a call to 
		 *  waitForSignals().
		 *  Returns true on success and false on failure. */
		static	bool	examineBlockedSignals(signalset *sset);
};


/** The signalhandler class provides methods for catching and handling
 *  signals. */
class signalhandlerprivate;
class RUDIMENTS_DLLSPEC signalhandler {
	public:

		/** Creates an instance of the signalhandler class. */
		signalhandler();

		/** Deletes this instance of the signalhandler class. */
		~signalhandler();

		/** Sets the function to call when the process
		 *  receives the signal. */
		void	setHandler(void (*handler)(int32_t));

		/** Returns a pointer to the function that will be
		 *  called when the process receives the signal. */
		void    (*getHandler())(int32_t);

		/** Instructs the program to handle "signum" by calling
		 *  the handler set previously in setHandler().  May
		 *  be called multiple times to associate the same
		 *  handler with several signals.
		 * 
		 *  Returns true on success and false on failure. */
		bool	handleSignal(int32_t signum);

		/** Instructs the program to handle "signum" by calling
		 *  the handler set previously in setHandler().  May
		 *  be called multiple times to associate the same
		 *  handler with several signals.  Populates oldhandler
		 *  with the values that were used to handle this signal
		 *  prior to this call.
		 * 
		 *  Returns true on success and false on failure. */
		bool	handleSignal(int32_t signum, signalhandler *oldhandler);
		
		/** Remove all signal flags. */
		void	removeAllFlags();

		/** Add "flag" to the set of flags modifying the
	 	 *  behavior of this signal handler. */
		void	addFlag(int32_t flag);

		/** Remove "flag" from the set of flags modifying the
	 	 *  behavior of this signal handler. */
		void	removeFlag(int32_t flag);

		/** Return the set of flags modifying the behavior of 
		 *  this signal handler. */
		int32_t	getFlags() const;


		/**  Explicitly sets the mask to "sset". */
		void		setMask(const signalset *sset);

		/** Returns the current signal mask. */
		const signalset	*getMask() const;

		/** The function that you pass into setHandler() must have a
		 *  void (*)(int32_t) signature.  However, the native signal
		 *  handling frameworks on some systems do not allow the
		 *  signal number to be passed to the handler in the int32_t
		 *  parameter.  As a result, on those platforms, inside the
		 *  signal handler, the paramer must be ignored.
		 * 
		 *  This method will return true if the platform's native
		 *  signal handling framework passes anything into the int32_t
		 *  parameter and false otherwise. */
		static	bool	isSignalHandlerIntUsed();

	#include <rudiments/private/signalhandler.h>

};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
