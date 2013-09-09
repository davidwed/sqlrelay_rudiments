// Copyright (c) 2013 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STDIO_H
#define RUDIMENTS_STDIO_H

#include <rudiments/filedescriptor.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

extern	RUDIMENTS_DLLSPEC	filedescriptor	stdinput;
extern	RUDIMENTS_DLLSPEC	filedescriptor	stdoutput;
extern	RUDIMENTS_DLLSPEC	filedescriptor	stderror;

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
