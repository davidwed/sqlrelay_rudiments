// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/config.h>

#ifdef RUDIMENTS_HAS_THREADS
	#include <rudiments/mutex.h>
#endif

#include <netdb.h>
#ifdef HAVE_RPCENT_H
	#include <rpc/rpcent.h>
#endif
#ifdef HAVE_RPC_H
	#include <rpc/rpc.h>
#endif
