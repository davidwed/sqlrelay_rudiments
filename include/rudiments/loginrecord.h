// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LOGINRECORD_H
#define RUDIMENTS_LOGINRECORD_H

#include <rudiments/private/loginrecord.h>

// @cond

class loginrecordprivate;

// wrap:
//	unistd.h - ttyslot(),getlogin(_r)(),setlogin()
//			getusershell(),endusershell(),setusershell()
//	stdio.h - cuserid()
//	utmpx.h - setutxent(),endutxent(),
//			getutxent(),getutxid(),getutxline()
//			pututxline()
//			utmpxname(),updwtmpx()
//			getutmp(),getutmpx()
//	not on solaris -
//		utmp.h - login(),logout(),logwtmp(),updwtmp(),uptmpname(),
//			getutent(_r)(),setutent(),endutent(),
//			getutid(_r)(),getutline(_r)(),pututline()
class RUDIMENTS_DLLSPEC loginrecord {
	public:

	#include <rudiments/private/loginrecord.h>
};

// @endcond

#endif
