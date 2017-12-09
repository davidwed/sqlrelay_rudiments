// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MESSAGEQUEUE_H
#define RUDIMENTS_MESSAGEQUEUE_H

#include <rudiments/private/messagequeue.h>

// @cond

class messagequeueprivate;

// wrap:
//	sys/msg.h - msgctl(),msgget(),msgrcv(),msgsnd()
class RUDIMENTS_DLLSPEC messagequeue {
	public:

	#include <rudiments/private/messagequeue.h>
};

// @endcond

#endif