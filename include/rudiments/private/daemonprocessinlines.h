// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#define __USE_UNIX98
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <stdio.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef HAVE_SYS_WAIT_H
	#include <sys/wait.h>
#endif

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

inline void daemonprocess::waitOnChildren() {
	while(waitpid(-1,NULL,WNOHANG)>0);
}


inline void daemonprocess::shutDown() {
	(*shutdownfunc)(0);
}

inline void daemonprocess::crash() {
	(*crashfunc)(0);
}

inline int daemonprocess::runAsUser(const char *username) const {
	uid_t	userid=-1;
	return passwdentry::getUserId(username,&userid) &&
		runAsUserId(userid);
}

inline int daemonprocess::runAsGroup(const char *groupname) const {
	gid_t	groupid=-1;
	return groupentry::getGroupId(groupname,&groupid) &&
		runAsGroupId(groupid);
}

inline int daemonprocess::runAsUserId(uid_t uid) const {
	#ifdef __CYGWIN__
		return 1;
	#else
		return !setreuid(uid,uid);
	#endif
}

inline int daemonprocess::runAsGroupId(gid_t gid) const {
	#ifdef __CYGWIN__
		return 1;
	#else
		return !setregid(gid,gid);
	#endif
}
