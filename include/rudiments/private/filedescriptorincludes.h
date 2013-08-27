// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <rudiments/resultcodes.h>

// This extern "C" is necessary because sys/types.h on some versions of minix
// (and maybe others) include stdio.h and the definition of vdprintf doesn't
// have extern "C" surrounding the definition.
extern "C" {
	#include <sys/types.h>
}

#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	// IRIX appears to need this but OpenBSD can't
	// find select() if it's still defined later
	#ifndef _XOPEN_SOURCE
		#define _XOPEN_SOURCE
		#define UN_XOPEN_SOURCE
	#endif
	#include <sys/socket.h>
	#ifdef UN_XOPEN_SOURCE
		#undef _XOPEN_SOURCE
	#endif
#endif

#ifdef RUDIMENTS_HAVE_STDARG_H
	#include <stdarg.h>
#endif
