// Copyright (c) 2015 David Muse
// See the COPYING file for more information

#ifndef RUDIMENTS_NEW_H
#define RUDIMENTS_NEW_H

#include <rudiments/private/config.h>

#if defined(RUDIMENTS_HAVE_NEW)
	#include <new>
#elif defined(RUDIMENTS_HAVE_NEW_H)
	#include <new.h>
#else
	inline void *operator new(size_t size, void *place) {
		return place;
	}
#endif

#endif
