// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#if defined(RUDIMENTS_HAS_PCRE)
	#include <pcre.h>
#elif defined(HAVE_REGEX_H)
	#include <sys/types.h>
	#include <regex.h>
#endif
