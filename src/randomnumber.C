// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/randomnumber.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/randomnumberinlines.h>
#endif

int randomnumber::generateNumber(int seed) {
	SEEDRANDOM(seed);
	return GETRANDOM();
}
