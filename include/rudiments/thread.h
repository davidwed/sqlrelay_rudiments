// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_THREAD_H
#define RUDIMENTS_THREAD_H

#include <rudiments/private/thread.h>

// pthread wrapper

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class threadprivate;

class thread {
	public:

	#include <rudiments/private/thread.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
