// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RAWBUFFER_H
#define RUDIMENTS_RAWBUFFER_H

#include <rudiments/private/rawbuffer.h>

// wrap:
//	string.h - memcpy(),memmove(),memccpy,memset(),memcmp(),memchr()
//	unistd.h - swab()
//	gnu extensions - 
//		string.h - rawmemchr(),memrchr(),memmem(),mempcpy()
//	linux only - 
//		string.h - memfrob()

class rawbuffer {
	public:

	#include <rudiments/private/rawbuffer.h>
};

#endif
