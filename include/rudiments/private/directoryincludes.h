// Copyright (c) 2004 David Muse
// See the COPYING file for more information

#include <rudiments/private/config.h>
#include <sys/types.h>
#ifdef HAVE_DIRENT_H
	#include <dirent.h>
#else
	#include <direct.h>
#endif
#ifdef RUDIMENTS_HAS_THREADS
	#include <rudiments/mutex.h>
#endif
