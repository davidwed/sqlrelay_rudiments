// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PSEUDOTTY_H
#define RUDIMENTS_PSEUDOTTY_H

#include <rudiments/private/pseudotty.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

// @cond

class pseudottyprivate;

// wrap:
//	not in solaris - 
//		pty.h - openpty(),forkpty()
//		utmp.h - login_tty()
//	not on solaris or linux -
//		ttyent.h - getttyent()/getttynam()/setttyent()/endttyent()
//	not in bsd -
//		stdlib.h - posix_optnpt(),grantpt(),unlockpt(),ptsname(_r)()
//	gnu extensions -
//		stdlib.h - getpt()
class RUDIMENTS_DLLSPEC pseudotty : public filedescriptor {
	public:

	#include <rudiments/private/pseudotty.h>
};

// @endcond

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
