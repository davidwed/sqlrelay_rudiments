// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>
#include <rudiments/private/rudimentsinlines.h>

#include <rudiments/regularexpression.h>

#include <sys/types.h>

#ifdef RUDIMENTS_HAVE_STDARG_H
	#include <stdarg.h>
#endif

#ifndef RUDIMENTS_HAVE_VA_COPY
	#define va_copy(a,b) ((a)=(b))
#endif
