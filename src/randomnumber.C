// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/randomnumber.h>

#include <stdlib.h>

#if defined(RUDIMENTS_HAVE_SRAND) && defined(RUDIMENTS_HAVE_RAND)
	#define SEEDRANDOM srand
	#define GETRANDOM rand
#elif defined(RUDIMENTS_HAVE_SRAND48) && defined(RUDIMENTS_HAVE_LRAND48)
	#define SEEDRANDOM srand48
	#define GETRANDOM lrand48
#else
	#error "Couldn't find a suitable replacement for rand/srand"
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_RAND_R)
static mutex	*_rnmutex;
#endif

int randomnumber::generateNumber(int seed) {

	// FIXME: use random(_r)()/srandom(_r)()/
	//		initstate(_r)()/setstate(_r)() instead?

	#ifdef RUDIMENTS_HAVE_RAND_R
		unsigned int	useed=seed;
		return rand_r(&useed);
	#else
		if (_rnmutex && !_rnmutex->lock()) {
			return -1;
		}
		SEEDRANDOM(seed);
		int	retval=GETRANDOM();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
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

bool randomnumber::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_RAND_R)
		return true;
	#else
		return false;
	#endif
}

void randomnumber::setMutex(mutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_RAND_R)
		_rnmutex=mtx;
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
