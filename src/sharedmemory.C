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
#ifdef HAVE_GETPWNAM_R
	passwd	*passwdent=new passwd;
	char	buffer[1024];
	if (getpwnam_r(username,passwdent,buffer,1024,&passwdent)) {
		return 0;
	}
#else
	passwd	*passwdent=NULL;
	if (!(passwdent=getpwnam(username))) {
		return 0;
	}
#endif
	int	retval=setUserId(passwdent->pw_uid);
#ifdef HAVE_GETPWNAM_R
	delete passwdent;
#endif
	return retval;
}

int sharedmemory::setGroupName(const char *groupname) {
#ifdef HAVE_GETGRNAM_R
	group	*groupent=new group;
	char	buffer[1024];
	if (getgrnam_r(groupname,groupent,buffer,1024,&groupent)) {
		return 0;
	}
#else
	group	*groupent=NULL;
	if (!(groupent=getgrnam(groupname))) {
		return 0;
	}
#endif
	int	retval=setGroupId(groupent->gr_gid);
#ifdef HAVE_GETGRNAM_R
	delete groupent;
#endif
	return retval;
}

char *sharedmemory::getUserName() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
#ifdef HAVE_GETPWUID_R
		passwd	*passwdent=new passwd;
		char	buffer[1024];
		if (getpwuid_r(getds.shm_perm.uid,passwdent,
					buffer,1024,&passwdent)) {
			return NULL;
		}
#else
		passwd	*passwdent=NULL;
		if (!(passwdent=getpwuid(getds.shm_perm.uid))) {
			return NULL;
		}
#endif
		char	*retval=strdup(passwdent->pw_name);
#ifdef HAVE_GETPWUID_R
		delete passwdent;
#endif
		return retval;
	}
	return NULL;
}

char *sharedmemory::getGroupName() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
#ifdef GETGRGID_R
		group	*groupent=new group;
		char	buffer[1024];
		if (getgrgid_r(getds.shm_perm.gid,groupent,
					buffer,1024,&groupent)) {
			return NULL;
		}
#else
		group	*groupent=NULL;
		if (!(groupent=getgrgid(getds.shm_perm.gid))) {
			return NULL;
		}
#endif
		char	*retval=strdup(groupent->gr_name);
#ifdef GETGRGID_R
		delete groupent;
#endif
		return retval;
	}
	return NULL;
}

#endif
