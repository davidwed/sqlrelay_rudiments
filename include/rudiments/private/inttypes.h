// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INTTYPES_H
#define RUDIMENTS_INTTYPES_H

#include <rudiments/private/config.h>

// define NULL...

// NULL is typically defined in stddef.h
#include <stddef.h>

// Certain versions of gcc define NULL as ((void *)0) and then complain when
// you set a const pointer to it.  Work around that.
#ifdef RUDIMENTS_REDEFINE_NULL
#undef NULL
#define NULL 0
#endif


// some platforms define types like char16_t in their new or new.h
// (some firstworks C code uses inttypes.h to make sure types are defined
// though, and we don't want that code to include new.h)
#ifdef __cplusplus
	#include <rudiments/private/new.h>
#endif


// define [u]int(8|16|32|64)_t...

#if defined(RUDIMENTS_HAVE_STDINT_H)
	#include <stdint.h>
#elif defined(RUDIMENTS_HAVE_SYS_BITYPES_H)
	// Tru64 needs __arch64__ for int64_t and uint64_t typedefs
	#ifndef __arch64__
		#define __arch64__
	#endif
	#include <sys/bitypes.h>
#elif defined(RUDIMENTS_HAVE_INTTYPES_H)
	#include <inttypes.h>
#endif

#if defined(RUDIMENTS_HAVE_UCHAR_H)
	#include <uchar.h>
#endif

#ifndef RUDIMENTS_HAVE_INT8_T
	typedef	signed char		int8_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT8_T
	typedef	unsigned char		uint8_t;
#endif
#ifndef RUDIMENTS_HAVE_INT16_T
	typedef	signed short		int16_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT16_T
	typedef	unsigned short		uint16_t;
#endif
#if !defined(RUDIMENTS_HAVE_CHAR16_T) && \
	defined(__cplusplus) && (__cplusplus<201103L)
	typedef	unsigned short		char16_t;
#endif
#ifndef RUDIMENTS_HAVE_INT32_T
	typedef	signed int		int32_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT32_T
	typedef	unsigned int		uint32_t;
	// older versions of solaris require this to prevent a pthreads conflict
	#define	_UINT32_T 1
#endif
#ifndef RUDIMENTS_HAVE_INT64_T
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		typedef	signed long long	int64_t;
	#else
		typedef	signed long		int64_t;
	#endif
#endif
#ifndef RUDIMENTS_HAVE_UINT64_T
	#ifdef RUDIMENTS_HAVE_LONG_LONG
		typedef	unsigned long long	uint64_t;
	#else
		typedef	unsigned long		uint64_t;
	#endif
#endif

#ifndef RUDIMENTS_HAVE_BOOL
	class bool {
		public:
			bool(const bool &b) {
				value=b.value;
			}
			bool(const long &b) {
				value=b;
			}
			bool(const int &b) {
				value=b;
			}
			bool(const short &b) {
				value=b;
			}
			bool(const char &b) {
				value=b;
			}
			bool(const unsigned long &b) {
				value=b;
			}
			bool(const unsigned int &b) {
				value=b;
			}
			bool(const unsigned short &b) {
				value=b;
			}
			bool(const unsigned char &b) {
				value=b;
			}
			bool &operator=(const bool &b) {
				value=b.value;
				return *this;
			}
			bool &operator=(const long &b) {
				value=b;
				return *this;
			}
			bool &operator=(const int &b) {
				value=b;
				return *this;
			}
			bool &operator=(const short &b) {
				value=b;
				return *this;
			}
			bool &operator=(const char &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned long &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned int &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned short &b) {
				value=b;
				return *this;
			}
			bool &operator=(const unsigned char &b) {
				value=b;
				return *this;
			}
			operator long() const {
				return value;
			}
			int operator!() {
				value=!value;
				return value;
			}
			int operator==(const bool &b) {
				return value==b.value;
			}
			int operator!=(const bool &b) {
				return value!=b.value;
			}
		private:
			long	value;
	};
#endif
#ifndef RUDIMENTS_HAVE_TRUE_FALSE
	#define true 1
	#define false 0
#endif

#endif
