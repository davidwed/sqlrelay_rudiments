// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TTY_H
#define RUDIMENTS_TTY_H

#include <rudiments/private/tty.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// @cond

class ttyprivate;

// wrap:
//	unistd.h - tcgetpgrp(),tcsetpgrp()
//			ctermid()
//			ttyname(_r)()
//			isatty()
//	termios.h - tcgetsid()
//	unistd.h - pathconf()/fpathconf():
//				__PC_VDISABLE()
//				__POSIX_MAX_CANON()
//				__POSIX_MAX_INPUT()
//	linux only -
//		unistd.h - vhangup()
class RUDIMENTS_DLLSPEC tty : public filedescriptor {
	public:

	#include <rudiments/private/tty.h>
};

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
