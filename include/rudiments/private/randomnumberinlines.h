// Copyright (c) 2002 David Muse
// See the COPYING file for more information

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

inline int randomnumber::generateNumber(int seed) {
	SEEDRANDOM(seed);
	return GETRANDOM();
}

inline int randomnumber::generateScaledNumber(int seed, int lower,
								int upper) {
	return lower+(int)(((float)generateNumber(seed)*(float)(upper-lower))/
							float(RAND_MAX));
}

inline int randomnumber::scaleNumber(int number, int lower, int upper) {
	return lower+(int)(((float)number*(float)(upper-lower))/
							float(RAND_MAX));
}

inline int randomnumber::getRandMax() {
	return RAND_MAX;
}
