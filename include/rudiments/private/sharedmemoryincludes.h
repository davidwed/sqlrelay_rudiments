// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#include <sys/types.h>
#ifndef RUDIMENTS_HAVE_SHMGET
	#include <sys/ipc.h>
	#include <sys/shm.h>
#endif
