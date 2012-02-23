// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_THREAD_H
#define RUDIMENTS_THREAD_H

#include <rudiments/private/thread.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// @cond

class threadprivate;

// pthread wrapper
class RUDIMENTS_DLLSPEC thread {
	public:

	#include <rudiments/private/thread.h>
};

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
