// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/randomnumber.h>
#include <rudiments/device.h>
#include <rudiments/datetime.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#if defined(RUDIMENTS_HAVE_SRAND) && defined(RUDIMENTS_HAVE_RAND)
	#define SEEDRANDOM srand
	#define GETRANDOM rand
#elif defined(RUDIMENTS_HAVE_SRAND48) && defined(RUDIMENTS_HAVE_LRAND48)
	#define SEEDRANDOM srand48
	#define GETRANDOM lrand48
#else
	#error "Couldn't find a suitable replacement for rand/srand"
#endif

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_RAND_R)
static threadmutex	*_rnmutex;
#endif

int32_t randomnumber::getSeed() {

	// first try /dev/urandom
	device	d;
	if (d.open("/dev/urandom",O_RDONLY)) {
		int32_t	retval=0;
		if (d.read((void *)&retval,sizeof(retval))==sizeof(retval)) {
			return retval;
		}
	}

	// if that fails, use epoch
	datetime	dt;
	dt.getSystemDateAndTime();
	return (int32_t)dt.getEpoch();
}

int32_t randomnumber::generateNumber(int32_t seed) {

	// FIXME: use random(_r)()/srandom(_r)()/
	//		initstate(_r)()/setstate(_r)() instead?

	#ifdef RUDIMENTS_HAVE_RAND_R
		uint32_t	useed=seed;
		return rand_r(&useed);
	#else
		if (_rnmutex && !_rnmutex->lock()) {
			return -1;
		}
		SEEDRANDOM(seed);
		int32_t	retval=GETRANDOM();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return retval;
	#endif
}

int32_t randomnumber::generateScaledNumber(int32_t seed,
					int32_t lower, int32_t upper) {
	return lower+
		(int32_t)(((float)generateNumber(seed)*
				(float)(upper-lower))/float(RAND_MAX));
}

int32_t randomnumber::scaleNumber(int32_t number,
					int32_t lower, int32_t upper) {
	return lower+(int32_t)(((float)number*(float)(upper-lower))/
							float(RAND_MAX));
}

int32_t randomnumber::getRandMax() {
	return RAND_MAX;
}

bool randomnumber::needsMutex() {
	#if !defined(RUDIMENTS_HAVE_RAND_R)
		return true;
	#else
		return false;
	#endif
}

void randomnumber::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_RAND_R)
		_rnmutex=mtx;
	#endif
}
