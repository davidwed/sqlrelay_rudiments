// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

//#include <rudiments/private/config.h>

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
#include <pwd.h>
#include <grp.h>
#ifdef HAVE_SYS_WAIT_H
	#include <sys/wait.h>
#endif

inline int	daemonprocess::createPidFile(const char *filename,
						mode_t permissions) {
	int	fd=open(filename,O_CREAT|O_TRUNC,permissions);
	if (fd>-1) {
		char	pidbuffer[21];
		sprintf(pidbuffer,"%d",getpid());
		write(fd,pidbuffer,strlen(pidbuffer));
		close(fd);
		return 1;
	}
	return 0;
}

inline void	daemonprocess::waitOnChildren() {
	while(waitpid(-1,NULL,WNOHANG)>0);
}


inline void	daemonprocess::shutDown() {
	(*shutdownfunc)(0);
}

inline int	daemonprocess::runAsUser(const char *username) const {
	struct	passwd	*ent=getpwnam((username)?username:"");
	return (ent)?runAsUserId(ent->pw_uid):0;
}

inline int	daemonprocess::runAsUserId(uid_t uid) const {
	#ifdef __CYGWIN__
		return 1;
	#else
		return setreuid(uid,uid);
	#endif
}

inline int	daemonprocess::runAsGroup(const char *groupname) const {
	struct	group	*ent=getgrnam((groupname)?groupname:"");
	return (ent)?runAsGroupId(ent->gr_gid):0;
}

inline int	daemonprocess::runAsGroupId(gid_t gid) const {
	#ifdef __CYGWIN__
		return 1;
	#else
		return setregid(gid,gid);
	#endif
}
