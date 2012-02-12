// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <sys/types.h>
#ifdef RUDIMENTS_HAVE_SHMGET
	#include <sys/ipc.h>
	#include <sys/shm.h>
#else
	typedef void shmid_ds;
#endif
