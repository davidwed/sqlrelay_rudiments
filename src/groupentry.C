// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/groupentry.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/groupentryinlines.h>
#endif

#include <stdio.h>
#include <errno.h>

#define MAXBUFFER (32*1024)

#if defined(__GNUC__) && \
	(!defined(HAVE_GETGRNAM_R) || !defined(HAVE_GETGRUID_R))
pthread_mutex_t	*groupentry::gemutex;
#endif

bool groupentry::initialize(const char *groupname, gid_t groupid) {

	if (grp) {
		grp=NULL;
		delete[] buffer;
		buffer=NULL;
	}
	#if defined(HAVE_GETGRNAM_R) && defined(HAVE_GETGRGID_R)
		// getgrnam_r and getgrgid_t are goofy.
		// They will retrieve an arbitrarily large amount of data, but
		// require that you pass them a pre-allocated buffer.  If the
		// buffer is too small, they return an ENOMEM and you have to
		// just make the buffer bigger and try again.
		for (int size=1024; size<MAXBUFFER; size=size+1024) {
			buffer=new char[size];
			if (!((groupname)
				?(getgrnam_r(groupname,&grpbuffer,
							buffer,size,&grp))
				:(getgrgid_r(groupid,&grpbuffer,
							buffer,size,&grp)))) {
				return (grp!=NULL);
			}
			delete[] buffer;
			buffer=NULL;
			grp=NULL;
			if (errno!=ENOMEM) {
				return false;
			}
		}
		return false;
	#else
		return (((gemutex)?!pthread_mutex_lock(gemutex):true) &&
			((grp=((groupname)
				?getgrnam(groupname)
				:getgrgid(groupid)))!=NULL) &&
			((gemutex)?!pthread_mutex_unlock(gemutex):true));
	#endif
}

void groupentry::print() const {

	if (!grp) {
		return;
	}

	printf("Name: %s\n",getName());
	printf("Password: %s\n",getPassword());
	printf("Group Id: %d\n",getGroupId());
	printf("Members:\n");
	for (int i=0; getMembers()[i]; i++) {
		printf("	%s\n",getMembers()[i]);
	}
}
