// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RANDOMNUMBER_H
#define RUDIMENTS_RANDOMNUMBER_H

#include <rudiments/private/randomnumberincludes.h>

class randomnumberprivate;

/** The randomnumber class provides methods for generating and scaling 
 *  random numbers.
 * 
 *  Superfluous background:
 * 
 *  There is no such thing as true randomness.   Many random number generators
 *  just return values from a highly divergent series.  Others collect "entropy"
 *  from system hardware returning numbers that are actually associated with
 *  system activity but in an indiscernible way.  In most cases, entropy-based
 *  systems are just used to seed systems that return values from a highly
 *  divergnet series.
 *
 *  If there is only one running process generating random numbers using a
 *  highly divergent series, successive calls to generateNumber() will never
 *  return the same value until all numbers between 0 and 2^32 have been
 *  returned.  At that point, the entire sequence will repeat.
 *
 *  Calls to generateScaledNumber() may return the same value before all
 *  numbers in the range have been returned because it just scales the result
 *  of generateNumber() which operates on a larger range. */
class RUDIMENTS_DLLSPEC randomnumber {
	public:

		/** Creates a new instance of the randomnumber class. */
		randomnumber();

		/** Deletes this instance of the randomnumber class. */
		~randomnumber();

		/** Sets the initial seed for this instance. */
		bool	setSeed(int32_t seed);

		/** Generates a random number between 0 and 2^32 and sets
		 *  "result" to this number.
		 *
		 *  Returns true on success and false on failure. */
		bool	generateNumber(int32_t *result);

		/** Generates a random number between "lower" and "upper"
		 *  and sets "result" to this number.
		 *
		 *  Returns true on success and false on failure. */
		bool	generateScaledNumber(int32_t lower,
						int32_t upper,
						int32_t *result);

		/** Generates a random number seed by first attempting
		 *  to get one from /dev/urandom and if that fails,
		 *  getting the number of seconds since 1970. */
		static	int32_t	getSeed();

		/** Generates a random number between 0 and 2^32, based
		 *  on seed, and returns it.  
		 * 
		 *  It is ok to use the result of this method as the 
		 *  seed for the next number. */
		static	int32_t	generateNumber(int32_t seed);

		/** Generates a random number between 0 and 2^32, based
		 *  on seed, scales that value to be between
		 *  lower and upper and returns it.
		 * 
		 *  It is NOT ok to use the result of this method as 
		 *  the seed for the next number. */
		static	int32_t	generateScaledNumber(int32_t seed, 
							int32_t lower,
							int32_t upper);

		/** Scales number to be between lower and upper and 
		 *  returns it.
		 * 
		 *  It is NOT ok to use the result of this method as 
		 *  the seed for the next number. */
		static	int32_t	scaleNumber(int32_t number,
						int32_t lower,
						int32_t upper);

		/** Operating systems can generate random numbers
		 *  between 0 and RAND_MAX.
		 *  This method returns RAND_MAX.
		 *  
		 *  RAND_MAX is usually 2^31-1 on 32 bit machines
		 *  but there are exceptions where it is 2^15-1 or
		 *  2^32-1. */
		static	int32_t	getRandMax();

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise. */
		static	bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it
		 *  (see needsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex. */
		static	void	setMutex(threadmutex *mtx);

	#include <rudiments/private/randomnumber.h>
};

#endif
