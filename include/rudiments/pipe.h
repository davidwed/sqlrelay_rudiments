// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PIPE_H
#define RUDIMENTS_PIPE_H

#include <rudiments/private/pipe.h>

// create() calls ::pipe()
// read() methods read from filedes[0]
// write() methods write to filedes[1]

class pipe : public filedescriptor {
	public:

	#include <rudiments/private/pipe.h>
};

#endif
