// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/rawbuffer.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef MUST_DEFINE_SWAB
extern "C" void swab(const void *from, void *to, ssize_t n);
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

void *rawbuffer::copy(void *dest, const void *src, size_t size) {
	return (dest && src)?memcpy(dest,src,size):NULL;
}

void *rawbuffer::copyWithOverlap(void *dest, const void *src, size_t size) {
	return (dest && src)?memmove(dest,src,size):NULL;
}

void *rawbuffer::copyUntil(void *dest, const void *src,
				unsigned char character, size_t size) {
	return (dest && src)?memccpy(dest,src,(int)character,size):NULL;
}

void *rawbuffer::copySwapBytes(void *dest, const void *src, size_t size) {
	if (dest && src) {
		#ifdef HAVE_SWAB_CHAR
		swab(static_cast<const char *>(src),
			static_cast<char *>(dest),size);
		#else
		swab(src,dest,size);
		#endif
	}
	return dest;
}

void *rawbuffer::set(void *dest, unsigned char character, size_t size) {
	return (dest)?memset(dest,static_cast<int>(character),size):NULL;
}

void *rawbuffer::zero(void *dest, size_t size) {
	return set(dest,0,size);
}

int rawbuffer::compare(const void *s1, const void *s2, size_t size) {
	return (s1 && s1)?memcmp(s1,s2,size):NULL;
}

void *rawbuffer::findFirst(const void *haystack,
				unsigned char needle, size_t size) {
	return (haystack && needle)?memchr(haystack,needle,size):NULL;
}

void *rawbuffer::findLast(const void *haystack,
				unsigned char needle, size_t size) {
	#ifdef HAVE_MEMRCHR
		return (haystack && needle)?
			memrchr(haystack,needle.size):NULL;
	#else
		if (haystack && needle) {
			unsigned char	realneedle=needle;
			for (const unsigned char *ptr=
				(static_cast<const unsigned char *>(haystack))+
				size;
				ptr>=haystack; ptr--) {
			
				if (*ptr==realneedle) {
					return const_cast<void *>(
						static_cast<const void *>(ptr));
				}
			}
		}
		return NULL;
	#endif
}

void *rawbuffer::findFirst(const void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {
	#ifdef HAVE_MEMMEM
		return (haystack && needle)?
			memmem(haystack,haystacksize,needle,needlesize):NULL;
	#else
		if (haystack && needle) {

			const unsigned char	*endptr=
				static_cast<const unsigned char *>(haystack)+
					haystacksize-needlesize;

			for (const unsigned char *ptr=
				static_cast<const unsigned char *>(haystack);
				ptr<endptr; ptr++) {

				if (!memcmp(static_cast<const void *>(ptr),
							needle,needlesize)) {
					return const_cast<void *>(
						static_cast<const void *>(ptr));
				}
			}
		}
		return NULL;
	#endif
}

void *rawbuffer::findLast(const void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {

	if (haystack && needle) {
		for (const unsigned char *ptr=
			static_cast<const unsigned char *>(haystack)+
						haystacksize-needlesize;
			ptr>=haystack; ptr--) {

			if (!memcmp(static_cast<const void *>(ptr),
						needle,needlesize)) {
				return const_cast<void *>(
					static_cast<const void *>(ptr));
			}
		}
	}
	return NULL;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
