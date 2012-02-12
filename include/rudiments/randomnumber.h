// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RANDOMNUMBER_H
#define RUDIMENTS_RANDOMNUMBER_H

#include <rudiments/private/randomnumberincludes.h>

// The randomnumber class provides methods for generating and scaling 
// random numbers.
//
// Superfluous background:
//
// Random numbers are actually just a highly divergent series.  There is
// no such thing as true randomness.  Using the above described methodology, 
// if there is only one running process generating random numbers, successive 
// calls to generateNumber() will never return the same value until all 
// numbers between 0 and 2^32 have been returned.  At that point, the entire 
// sequence will repeat.  Calls to generateScaledNumber() may return the
// same value before all numbers in the range have been returned because
// it scales the result of generateNumber() which operates on a larger range.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class DLLSPEC randomnumber {
	public:
		static	int32_t	getSeed();
			// Generates a random number seed by first attempting
			// to get one from /dev/urandom and if that fails,
			// getting the number of seconds since 1970.
		static	int32_t	generateNumber(int32_t seed);
			// Generates a random number between 0 and 2^32, based
			// on seed, and returns it.  
			//
			// It is ok to use the result of this method as the 
			// seed for the next number.
		static	int32_t	generateScaledNumber(int32_t seed, 
							int32_t lower,
							int32_t upper);
			// Generates a random number between 0 and 2^32, based
			// on seed, scales that value to be between
			// lower and upper and returns it.
			//
			// It is NOT ok to use the result of this method as 
			// the seed for the next number.
		static	int32_t	scaleNumber(int32_t number,
						int32_t lower,
						int32_t upper);
			// Scales number to be between lower and upper and 
			// returns it.
			//
			// It is NOT ok to use the result of this method as 
			// the seed for the next number.

		static	int32_t	getRandMax();
			// Operating systems can generate random numbers
			// between 0 and RAND_MAX.
			// This method returns RAND_MAX.
			// 
			// RAND_MAX is usually 2^31-1 on 32 bit machines
			// but there are exceptions where it is 2^15-1 or
			// 2^32-1.

		static	bool	needsMutex();
			// If your system doesn't support rand_r() then this
			// class needs a mutex to assure thread safety.
			//
			// This method returns true if this class needs a mutex
			// to operate safely in a threaded environment and false
			// otherwise.
		static	void	setMutex(mutex *mtx);
			// Allows you to supply a mutex is the class needs it.
			// If your application is not multithreaded, then
			// there is no need to supply a mutex.
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
