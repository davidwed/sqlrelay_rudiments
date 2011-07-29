// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#include <rudiments/resultcodes.h>

#include <sys/types.h>

#ifdef RUDIMENTS_HAS_SSL
	#include <openssl/ssl.h>
#endif

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

// so STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO will be defined if they need
// to be used by the calling program...
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <stdio.h>
