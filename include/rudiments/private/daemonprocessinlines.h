// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <sys/types.h>
#include <sys/stat.h>
/*#include <fcntl.h>
#include <stdio.h>*/
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef HAVE_SYS_WAIT_H
	#include <sys/wait.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE void daemonprocess::waitOnChildren() {
	while(waitpid(-1,NULL,WNOHANG)>0);
}


RUDIMENTS_INLINE void daemonprocess::shutDown() {
	(*shutdownfunc)(0);
}

RUDIMENTS_INLINE void daemonprocess::crash() {
	(*crashfunc)(0);
}

RUDIMENTS_INLINE int daemonprocess::runAsUserId(uid_t uid) const {
	#ifdef __CYGWIN__
		return 1;
	#else
		return !setuid(uid);
	#endif
}

RUDIMENTS_INLINE int daemonprocess::runAsGroupId(gid_t gid) const {
	#ifdef __CYGWIN__
		return 1;
	#else
		return !setgid(gid);
	#endif
}
