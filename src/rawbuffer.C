// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/rawbuffer.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void *rawbuffer::copy(void *dest, const void *src, size_t size) {
	return memcpy(dest,src,size);
}

void *rawbuffer::copyWithOverlap(void *dest, const void *src, size_t size) {
	return memmove(dest,src,size);
}

void *rawbuffer::copyUntil(void *dest, const void *src,
				unsigned char character, size_t size) {
	return memccpy(dest,src,(int)character,size);
}

void *rawbuffer::copySwapBytes(void *dest, const void *src, size_t size) {
	#ifdef HAVE_SWAB_CHAR
	swab((const char *)src,(char *)dest,size);
	#else
	swab(src,dest,size);
	#endif
	return dest;
}

void *rawbuffer::set(void *dest, unsigned char character, size_t size) {
	return memset(dest,(int)character,size);
}

int rawbuffer::compare(const void *s1, const void *s2, size_t size) {
	return memcmp(s1,s2,size);
}

void *rawbuffer::findFirst(const void *haystack, unsigned char needle, size_t size) {
	return memchr(haystack,needle,size);
}

void *rawbuffer::findLast(const void *haystack, unsigned char needle, size_t size) {
	#ifdef HAVE_MEMRCHR
		return memrchr(haystack,needle.size);
	#else
		unsigned char	realneedle=needle;
		for (unsigned char *ptr=((unsigned char *)haystack)+
							size;
						ptr>=haystack; ptr--) {
			
			if (*ptr==realneedle) {
				return (void *)ptr;
			}
		}
		return NULL;
	#endif
}

void *rawbuffer::findFirst(const void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {
	#ifdef HAVE_MEMMEM
		return memmem(haystack,haystacksize,needle,needlesize);
	#else
		unsigned char	*endptr=(unsigned char *)haystack+
						haystacksize-needlesize;
		for (unsigned char *ptr=(unsigned char *)haystack;
						ptr<endptr; ptr++) {
			if (!memcmp((void *)ptr,needle,needlesize)) {
				return (void *)ptr;
			}
		}
		return NULL;
	#endif
}

void *rawbuffer::findLast(const void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {

	for (unsigned char *ptr=((unsigned char *)haystack)+
					haystacksize-needlesize;
						ptr>=haystack; ptr--) {
		if (!memcmp((void *)ptr,needle,needlesize)) {
			return (void *)ptr;
		}
	}
	return NULL;
}
