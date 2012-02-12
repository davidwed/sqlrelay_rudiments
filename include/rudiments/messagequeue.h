// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MESSAGEQUEUE_H
#define RUDIMENTS_MESSAGEQUEUE_H

#include <rudiments/private/messagequeue.h>

// wrap:
//	sys/msg.h - msgctl(),msgget(),msgrcv(),msgsnd()

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class messagequeueprivate;

class DLLSPEC messagequeue {
	public:

	#include <rudiments/private/messagequeue.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
