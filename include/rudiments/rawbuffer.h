// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RAWBUFFER_H
#define RUDIMENTS_RAWBUFFER_H

#include <rudiments/private/rawbufferincludes.h>

// The rawbuffer class provides methods for operating on raw memory buffers.

class rawbuffer {
	public:

		static void	*copy(void *dest, const void *src,
							size_t size);
				// Copies "size" bytes from "src" into "dest".
				// "src" and "dest" may NOT overlap.
				// Returns a pointer to "dest".
		static void	*copyWithOverlap(void *dest,
							const void *src,
							size_t size);
				// Copies "size" bytes from "src" to "dest".
				// "src" and "dest" may overlap.
				// Returns a pointer to "dest".
		static void	*copyUntil(void *dest, const void *src,
							unsigned char character,
							size_t size);
				// Copies bytes from "src" into "dest" until
				// "character" is encountered in "src" or "size"
				// bytes have been copied.
				// "src" and "dest" may NOT overlap.
				// Returns a pointer to "dest".
		static void	*copySwapBytes(void *dest,
							const void *src,
							size_t size);
				// Copies "size" bytes from "src" into "dest"
				// while swapping byte order.  Useful for
				// exchanging data between big and little
				// endian machines.
				// "src" and "dest" may NOT overlap.
				// Returns a pointer to "dest".

		static void	*set(void *dest, unsigned char character,
							size_t size);
				// Sets "size" bytes of "dest" to "character".
				// Returns a pointer to "dest".

		static int	compare(const void *s1, const void *s2,
							size_t size);
				// Returns an integer less than, equal to or
				// greater than 0 if "size" bytes of "s1" are
				// less than, equal to or greater than "size"
				// bytes of "s2".

		static void	*findFirst(const void *haystack,
							unsigned char needle,
							size_t size);
				// Returns a pointer to the first occurrance
				// of "needle" in the first "size" bytes of
				// "haystack".
				// Returns NULL if no match is found.
		static void	*findLast(const void *haystack,
							unsigned char needle,
							size_t size);
				// Returns a pointer to the last occurrance
				// of "needle" in the first "size" bytes of
				// "haystack".
				// Returns NULL if no match is found.

		static void	*findFirst(const void *haystack,
							size_t haystacksize,
							const void *needle, 
							size_t needleize);
				// Returns a pointer to the first occurrance
				// of "needle" in the first "size" bytes of
				// "haystack".
				// Returns NULL if no match is found.
		static void	*findLast(const void *haystack,
							size_t haystacksize,
							const void *needle, 
							size_t needleize);
				// Returns a pointer to the last occurrance
				// of "needle" in the first "size" bytes of
				// "haystack".
				// Returns NULL if no match is found.
};

#endif
