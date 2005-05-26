// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#include <rudiments/resultcodes.h>

#include <sys/types.h>

#ifdef RUDIMENTS_HAS_SSL
	#include <openssl/ssl.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
	// IRIX appears to need this
	#ifndef _XOPEN_SOURCE
		#define _XOPEN_SOURCE
	#endif
	#include <sys/socket.h>
#endif
