// Copyright (c) 2013 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STDIO_H
#define RUDIMENTS_STDIO_H

#include <rudiments/filedescriptor.h>

/** The filedescriptor class provides methods for generic input and output to
 *  file descriptors.  Rudiments pre-defines three instances: stdinput,
 *  stdoutput and stderror, which provide access to standard input, standard
 *  output and standard error.
 *
 *  As these classes are ultimately instances of the filedescriptor class, many
 *  I/O methods are available, but the most commonly used are read(), write()
 *  and printf().  The printf() method features all of the formatting
 *  capabilities of the standard C printf() function. */
class RUDIMENTS_DLLSPEC stdiofiledescriptor : public filedescriptor {
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
