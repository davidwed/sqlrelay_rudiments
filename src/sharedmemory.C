// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#ifndef __CYGWIN__

#include <rudiments/sharedmemory.h>
#ifndef ENABLE_INLINES
	#define INLINE
	#include <rudiments/private/sharedmemoryinlines.h>
#endif

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

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

char *sharedmemory::getUserName() {
	shmid_ds	getds;
	char		*name;
	if (!shmctl(shmid,IPC_STAT,&getds) &&
			passwdentry::getName(getds.shm_perm.uid,&name)) {
		return name;
	}
	return NULL;
}

char *sharedmemory::getGroupName() {
	shmid_ds	getds;
	char		*name;
	if (!shmctl(shmid,IPC_STAT,&getds) &&
			groupentry::getName(getds.shm_perm.gid,&name)) {
		return name;
	}
	return NULL;
}

#endif
