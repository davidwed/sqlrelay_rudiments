// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/randomnumber.h>

#include <stdlib.h>

#if defined(HAVE_SRAND) && defined(HAVE_RAND)
	#define SEEDRANDOM srand
	#define GETRANDOM rand
#elif defined(HAVE_SRAND48) && defined(HAVE_LRAND48)
	#define SEEDRANDOM srand48
	#define GETRANDOM lrand48
#else
	#error "Couldn't find a suitable replacement for rand/srand"
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

#if defined(RUDIMENTS_HAS_THREADS) && !defined(HAVE_RAND_R)
mutex	*randomnumber::rnmutex;
#endif

int randomnumber::generateNumber(int seed) {

	// FIXME: use random(_r)()/srandom(_r)()/
	//		initstate(_r)()/setstate(_r)() instead?

	#ifdef HAVE_RAND_R
		unsigned int	useed=seed;
		return rand_r(&useed);
	#else
		#ifdef RUDIMENTS_HAS_THREADS
		if (rnmutex && !rnmutex->lock()) {
			return -1;
		}
		#endif
		SEEDRANDOM(seed);
		int	retval=GETRANDOM();
		#ifdef RUDIMENTS_HAS_THREADS
		if (rnmutex) {
			rnmutex->unlock();
		}
		#endif
		return retval;
	#endif
}

int randomnumber::generateScaledNumber(int seed, int lower, int upper) {
	return lower+(int)(((float)generateNumber(seed)*(float)(upper-lower))/
							float(RAND_MAX));
}

int randomnumber::scaleNumber(int number, int lower, int upper) {
	return lower+(int)(((float)number*(float)(upper-lower))/
							float(RAND_MAX));
}

int randomnumber::getRandMax() {
	return RAND_MAX;
}

#ifdef RUDIMENTS_HAS_THREADS
bool randomnumber::needsMutex() {
	#if !defined(HAVE_RAND_R)
		return true;
	#else
		return false;
	#endif
}

void randomnumber::setMutex(mutex *mtx) {
	#if !defined(HAVE_RAND_R)
		rnmutex=mtx;
	#endif
}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
