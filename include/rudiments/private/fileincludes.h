// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/filedescriptor.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#ifndef HAVE_BLKCNT_T
	typedef long blkcnt_t;
#endif
#ifndef HAVE_BLKSIZE_T
	typedef long blksize_t;
#endif
