// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>


#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS)
	#include <sys/types.h>
	#include <sys/vfs.h>
#endif
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	#include <sys/param.h>
	#include <sys/mount.h>
#endif
#if defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	#include <sys/types.h>
	#include <sys/statvfs.h>
#endif
