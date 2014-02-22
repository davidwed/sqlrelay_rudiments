// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/filedescriptor.h>

#include <sys/types.h>

// some systems need this for key_t
#ifdef RUDIMENTS_HAVE_SYS_IPC_H
	#include <sys/ipc.h>
#endif

// for open flags
#ifdef RUDIMENTS_HAVE_FCNTL_H
	// for open flags with msvc/mingw32...
	// make sure to undefine _POSIX_ if it wasn't already defined though,
	// as it will prevent various process-related functions from being
	// found later if it's still defined
	#ifdef _WIN32
		#ifndef _POSIX_
			#define _POSIX_
			#define RUDIMENTS_UNDEFPOSIX
		#endif
	#endif
	#include <fcntl.h>
	#ifdef _WIN32
		#ifdef RUDIMENTS_UNDEFPOSIX
			#undef _POSIX_
		#endif
	#endif
#endif

#ifndef RUDIMENTS_HAVE_BLKSIZE_T
	typedef long blksize_t;
#endif
#ifndef RUDIMENTS_HAVE_BLKCNT_T
	typedef long blkcnt_t;
#endif

// windows doesn't define these but we need them to be able to lock files
#ifndef F_RDLCK
	#define F_RDLCK	0
#endif
#ifndef F_WRLCK
	#define F_WRLCK	1
#endif
#ifndef F_UNLCK
	#define F_UNLCK	2
#endif
