// Copyright (c) 2013 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STDIO_H
#define RUDIMENTS_STDIO_H

#include <rudiments/filedescriptor.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

extern	filedescriptor	stdinput;
extern	filedescriptor	stdoutput;
extern	filedescriptor	stderror;

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
