// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/bytestring.h>
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_MUST_DEFINE_SWAB
extern "C" void swab(const void *from, void *to, ssize_t n);
#endif

void *bytestring::duplicate(const void *src, size_t size) {
	if (!src || !size) {
		return NULL;
	}
	unsigned char	*buffer=new unsigned char[size];
	return copy(static_cast<void *>(buffer),src,size);
}

void *bytestring::copy(void *dest, const void *src, size_t size) {
	return (dest && src)?memcpy(dest,src,size):NULL;
}

void *bytestring::copyWithOverlap(void *dest, const void *src, size_t size) {
	return (dest && src)?memmove(dest,src,size):NULL;
}

void *bytestring::copyUntil(void *dest, const void *src,
				unsigned char character, size_t size) {
	#if defined(RUDIMENTS_HAVE__MEMCCPY)
		return (dest && src)?_memccpy(dest,src,
					static_cast<int>(character),size):NULL;
	#elif defined(RUDIMENTS_HAVE_MEMCCPY_CHAR)
		return (dest && src)?static_cast<void *>(
					memccpy(static_cast<char *>(dest),
					static_cast<const char *>(src),
					static_cast<int>(character),size)):NULL;
	#else
		return (dest && src)?memccpy(dest,src,
					static_cast<int>(character),size):NULL;
	#endif
}

void *bytestring::copySwapBytes(void *dest, const void *src, size_t size) {
	if (dest && src) {
		#if defined(RUDIMENTS_HAVE__SWAB)
			_swab((char *)src,(char *)dest,size);
		#elif defined(RUDIMENTS_HAVE_SWAB_CONST_CHAR)
			swab((const char *)src,(char *)dest,size);
		#elif defined(RUDIMENTS_HAVE_SWAB_CHAR)
			swab((char *)src,(char *)dest,size);
		#elif defined(RUDIMENTS_HAVE_SWAB_CONST_VOID)
			swab((const void *)src,(void *)dest,size);
		#else
			swab(src,dest,size);
		#endif
	}
	return dest;
}

void *bytestring::set(void *dest, unsigned char character, size_t size) {
	return (dest)?memset(dest,static_cast<int>(character),size):NULL;
}

void *bytestring::zero(void *dest, size_t size) {
	return set(dest,0,size);
}

int32_t bytestring::compare(const void *s1, const void *s2, size_t size) {
	if (!s1 && !s2) {
		return 0;
	}
	if (s1 && s2) {
		return memcmp(s1,s2,size);
	}
	return 1;
}

const void *bytestring::findFirst(const void *haystack,
				unsigned char needle, size_t size) {
	return (haystack)?memchr(haystack,needle,size):NULL;
}

const void *bytestring::findLast(const void *haystack,
				unsigned char needle, size_t size) {
	#ifdef RUDIMENTS_HAVE_MEMRCHR
		return (haystack)?
			memrchr(haystack,needle,size):NULL;
	#else
		if (haystack) {
			unsigned char	realneedle=needle;
			for (const unsigned char *ptr=
				(static_cast<const unsigned char *>(haystack))+
				size;
				ptr>=
				static_cast<const unsigned char *>(haystack);
				ptr--) {
			
				if (*ptr==realneedle) {
					return static_cast<const void *>(ptr);
				}
			}
		}
		return NULL;
	#endif
}

const void *bytestring::findFirst(const void *haystack, size_t haystacksize,
					const void *needle, size_t needlesize) {
	#ifdef RUDIMENTS_HAVE_MEMMEM
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

				if (!memcmp(ptr,needle,needlesize)) {
					return static_cast<const void *>(ptr);
				}
			}
		}
		return NULL;
	#endif
}

const void *bytestring::findLast(const void *haystack, size_t haystacksize,
					const void *needle, size_t needlesize) {

	if (haystack && needle) {
		for (const unsigned char *ptr=
			static_cast<const unsigned char *>(haystack)+
						haystacksize-needlesize;
			ptr>=static_cast<const unsigned char *>(haystack);
			ptr--) {

			if (!memcmp(ptr,needle,needlesize)) {
				return const_cast<void *>(
					static_cast<const void *>(ptr));
			}
		}
	}
	return NULL;
}

void *bytestring::findFirst(void *haystack,
				unsigned char needle, size_t size) {
	return const_cast<void *>(findFirst(
					const_cast<const void *>(haystack),
					needle,size));
}

void *bytestring::findLast(void *haystack,
				unsigned char needle, size_t size) {
	return const_cast<void *>(findLast(
					const_cast<const void *>(haystack),
					needle,size));
}

void *bytestring::findFirst(void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {
	return const_cast<void *>(findFirst(
					const_cast<const void *>(haystack),
					haystacksize,
					needle,needlesize));
}

void *bytestring::findLast(void *haystack, size_t haystacksize,
				const void *needle, size_t needlesize) {
	return const_cast<void *>(findLast(
					const_cast<const void *>(haystack),
					haystacksize,
					needle,needlesize));
}
