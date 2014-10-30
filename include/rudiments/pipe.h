// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PIPE_H
#define RUDIMENTS_PIPE_H

#include <rudiments/private/pipe.h>

// @cond

class pipeprivate;

// create() calls ::pipe()
// read() methods read from filedes[0]
// write() methods write to filedes[1]
// wrap:
//	unistd.h - pipe(), pathconf()/fpathconf(): __POSIX_PIPE_BUF()
class RUDIMENTS_DLLSPEC pipe : public filedescriptor {
	public:

	#include <rudiments/private/pipe.h>
};

// @endcond

#endif