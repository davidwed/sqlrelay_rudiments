// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#ifndef __CYGWIN__

#include <rudiments/sharedmemory.h>
#ifndef ENABLE_INLINES
	#define inline
	#include <rudiments/private/sharedmemoryinlines.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <sys/stat.h>

int sharedmemory::create(key_t key, int size, mode_t permissions) {

	// create the shared memory segment
	if ((shmid=shmget(key,size,IPC_CREAT|IPC_EXCL|permissions))>-1) {

		// mark for removal
		created=1;

		// attach to the segment
		shmptr=shmat(shmid,0,0);

		// init the segment to zero's
		memset((void *)shmptr,0,size);
		return 1;
	}

	return 0;
}

int sharedmemory::attach(key_t key) {
	return ((shmid=shmget(key,0,0))>-1 &&
			(int)(shmptr=shmat(shmid,0,0))>-1);
}

int sharedmemory::createOrAttach(key_t key, int size, mode_t permissions) {

	// create the shared memory segment
	if ((shmid=shmget(key,size,IPC_CREAT|IPC_EXCL|permissions))>-1) {

		// mark for removal
		created=1;

		// attach to the segment
		shmptr=shmat(shmid,0,0);

		// init the segment to zero's
		memset((void *)shmptr,0,size);
		return 1;
		
	} else if (errno==EEXIST && (shmid=shmget(key,0,permissions))>-1) {

		// attach to the segment
		shmptr=shmat(shmid,0,0);
		return 1;

	}

	return 0;
}

int sharedmemory::setUserName(const char *username) {
	passwd	*passwdent=getpwnam(username);
	if (!passwdent) {
		return 0;
	}
	int	retval=setUserId(passwdent->pw_uid);
	delete passwdent;
	return retval;
}

int sharedmemory::setGroupName(const char *groupname) {
	group	*groupent=getgrnam(groupname);
	if (!groupent) {
		return 0;
	}
	int	retval=setGroupId(groupent->gr_gid);
	delete groupent;
	return retval;
}

char *sharedmemory::getUserName() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		passwd	*passwdent=getpwuid(getds.shm_perm.uid);
		char	*retval=strdup(passwdent->pw_name);
		delete passwdent;
		return retval;
	}
	return NULL;
}

char *sharedmemory::getGroupName() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		group	*groupent=getgrgid(getds.shm_perm.gid);
		char	*retval=strdup(groupent->gr_name);
		delete groupent;
		return retval;
	}
	return NULL;
}

#endif
