// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <signal.h>
#ifdef RUDIMENTS_HAVE_SYS_SIGNAL_H
	#include <sys/signal.h>
#endif
#include <sys/types.h>

// make sure SIGKILL is defined
#ifndef SIGKILL
	#define SIGKILL 9
#endif
