// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	#include <sys/types.h>
	#include <sys/vfs.h>
#endif
#ifdef HAVE_CYGWIN_STATFS
	typedef	long	fsid_t;
#endif
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	#include <sys/param.h>
	#include <sys/mount.h>
#endif
#ifdef HAVE_STATVFS
	#include <sys/types.h>
	#include <sys/statvfs.h>
#endif
