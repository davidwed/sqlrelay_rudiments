// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/config.h>
#include <sys/types.h>

// for socklen_t
#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	#include <sys/socket.h>
#endif
#ifdef RUDIMENTS_HAVE_WINSOCK2_H
	#include <winsock2.h>
#endif

#include <rudiments/client.h>
