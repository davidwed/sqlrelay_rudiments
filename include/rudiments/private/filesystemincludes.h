// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#ifdef HAVE_LINUX_STATFS
	#include <sys/types.h>
	#include <sys/vfs.h>
#endif
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	#include <sys/param.h>
	#include <sys/mount.h>
#endif
#ifdef HAVE_STATVFS
	#include <sys/types.h>
	#include <sys/statvfs.h>
#endif
