// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RANDOMNUMBER_H
#define RUDIMENTS_RANDOMNUMBER_H

#include <rudiments/private/randomnumberincludes.h>

// The randomnumber class provides methods for generating and scaling 
// random numbers.
//
// A common practice for generating random number sequences is to seed the
// first call to generateNumber() with the number of seconds since 1970 (see
// the datetime class) and seed each successive call with the previously
// generated number.
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

class randomnumber {
	public:
		static	int	generateNumber(int seed);
			// Generates a random number between 0 and 2^32, based
			// on seed, and returns it.  
			//
			// It is ok to use the result of this method as the 
			// seed for the next number.
		static	int	generateScaledNumber(int seed, 
							int lower, int upper);
			// Generates a random number between 0 and 2^32, based
			// on seed, scales that value to be between
			// lower and upper and returns it.
			//
			// It is NOT ok to use the result of this method as 
			// the seed for the next number.
		static	int	scaleNumber(int number, int lower, int upper);
			// Scales number to be between lower and upper and 
			// returns it.
			//
			// It is NOT ok to use the result of this method as 
			// the seed for the next number.

		static	int	getRandMax();
			// Operating systems can generate random numbers
			// between 0 and RAND_MAX.
			// This method returns RAND_MAX.
			// 
			// RAND_MAX is usually 2^31-1 on 32 bit machines
			// but there are exceptions where it is 2^15-1 or
			// 2^32-1.
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/randomnumberinlines.h>
#endif

#endif
