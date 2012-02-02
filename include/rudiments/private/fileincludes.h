// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/filedescriptor.h>

#include <sys/types.h>

// for open flags
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif

#ifndef RUDIMENTS_HAVE_BLKSIZE_T
	typedef long blksize_t;
#endif
#ifndef RUDIMENTS_HAVE_BLKCNT_T
	typedef long blkcnt_t;
#endif
