// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PSEUDOTTY_H
#define RUDIMENTS_PSEUDOTTY_H

#include <rudiments/private/pseudotty.h>

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

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class pseudottyprivate;

class DLLSPEC pseudotty : public filedescriptor {
	public:

	#include <rudiments/private/pseudotty.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
