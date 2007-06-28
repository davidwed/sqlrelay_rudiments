// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/private/inttypes.h>

#include <rudiments/mutex.h>

#include <netdb.h>
#ifdef HAVE_RPCENT_H
	#include <rpc/rpcent.h>
#endif
#ifdef HAVE_RPC_H
	#include <rpc/rpc.h>
#endif
