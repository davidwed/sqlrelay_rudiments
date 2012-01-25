// Copyright (c) 2005 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INTTYPES_H
#define RUDIMENTS_INTTYPES_H

#include <rudiments/private/config.h>

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
#ifndef RUDIMENTS_HAVE_INT32_T
	typedef	signed int		int32_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT32_T
	typedef	unsigned int		uint32_t;
#endif
#ifndef RUDIMENTS_HAVE_INT64_T
	typedef	signed long long	int64_t;
#endif
#ifndef RUDIMENTS_HAVE_UINT64_T
	typedef	unsigned long long	uint64_t;
#endif

#endif
