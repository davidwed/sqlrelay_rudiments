// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/filedescriptor.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

RUDIMENTS_DLLSPEC filedescriptor	stdinput(0);
RUDIMENTS_DLLSPEC filedescriptor	stdoutput(1);
RUDIMENTS_DLLSPEC filedescriptor	stderror(2);

#ifdef RUDIMENTS_NAMESPACE
}
#endif
