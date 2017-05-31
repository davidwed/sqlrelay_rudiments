// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>

// for struct timespec (it includes inttypes.h as well)
#include <rudiments/private/snoozeincludes.h>

#include <sys/types.h>

// some systems need this for key_t
#ifdef RUDIMENTS_HAVE_SYS_IPC_H
	#include <sys/ipc.h>
#endif

struct sembuf;
union semun;

#ifndef IPC_PRIVATE
	#define IPC_PRIVATE	((key_t)0)
#endif
