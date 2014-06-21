// Copyright (c) 2000-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/environment.h>
#include <rudiments/bytestring.h>

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#define _WINSOCKAPI_
	#include <windows.h>
#endif
#include <time.h>
#include <sys/types.h>
#include <rudiments/threadmutex.h>
