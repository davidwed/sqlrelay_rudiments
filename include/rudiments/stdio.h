// Copyright (c) 2013 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STDIO_H
#define RUDIMENTS_STDIO_H

#include <rudiments/filedescriptor.h>

class stdiofiledescriptor : public filedescriptor {
	public:
		/** Flushes buffered prints to the console.  Returns
		 *  true on success or false if an error occurred. */
		bool	flush();

	#include <rudiments/private/stdio.h>
};

extern	RUDIMENTS_DLLSPEC stdiofiledescriptor	stdinput;
extern	RUDIMENTS_DLLSPEC stdiofiledescriptor	stdoutput;
extern	RUDIMENTS_DLLSPEC stdiofiledescriptor	stderror;

#endif
