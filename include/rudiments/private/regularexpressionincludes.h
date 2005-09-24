// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#ifdef RUDIMENTS_HAS_PCRE
	#include <pcre.h>
#else
	#include <sys/types.h>
	#include <regex.h>
#endif
