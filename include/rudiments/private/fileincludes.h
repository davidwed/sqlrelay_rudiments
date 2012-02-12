// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/filedescriptor.h>

#include <sys/types.h>

// for open flags
#ifdef RUDIMENTS_HAVE_FCNTL_H
	// for open flags with msvc
	#ifdef _WIN32
		#define _POSIX_
	#endif
	#include <fcntl.h>
#endif

#ifndef RUDIMENTS_HAVE_BLKSIZE_T
	typedef long blksize_t;
#endif
#ifndef RUDIMENTS_HAVE_BLKCNT_T
	typedef long blkcnt_t;
#endif
