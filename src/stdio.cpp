// Copyright (c) 2013 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

filedescriptor	stdinput(0);
filedescriptor	stdoutput(1);
filedescriptor	stderror(2);

#ifdef RUDIMENTS_NAMESPACE
}
#endif
