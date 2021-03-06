// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <sys/types.h>
#ifdef RUDIMENTS_HAVE_SYS_PARAM_H
	#include <sys/param.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_UN_H
	#include <sys/un.h>
#endif

#ifndef RUDIMENTS_HAVE_SOCKADDR_UN
	struct sockaddr_un {
		int16_t	sun_family;
		char	sun_path[512];
	};
#endif
