// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RAWBUFFER_H
#define RUDIMENTS_RAWBUFFER_H

#include <rudiments/private/rawbufferincludes.h>

// wrap:
//	string.h - memcpy(),memmove(),memccpy,memset(),memcmp(),memchr()
//	unistd.h - swab()
//	gnu extensions - 
//		string.h - rawmemchr(),memrchr(),memmem(),mempcpy()
//	linux only - 
//		string.h - memfrob()

class rawbuffer {
	public:

		static void	*copy(void *dest, const void *src, size_t size);
		static void	*copyUntil(void *dest, const void *src,
						int character, size_t size);

		static void	*move(void *dest, const void *src, size_t size);
		static void	*moveSwapBytes(void *dest, const void *src,
								size_t size);

		static void	*set(void *dest, int c, size_t size);

		static int	compare(const void *s1, const void *s2,
								size_t size);

		static void	*findFirst(const void *haystack, int needle,
								size_t size);
		static void	*findLast(const void *haystack, int needle,
								size_t size);

		static void	*findFirst(const void *haystack,
							size_t haystacksize,
							const void *needle, 
							size_t needleize);
		static void	*findLast(const void *haystack,
							size_t haystacksize,
							const void *needle, 
							size_t needleize);
};

#endif
