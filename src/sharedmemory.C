// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/rawbuffer.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

sharedmemory::sharedmemory() {
	created=false;
	shmptr=NULL;
	shmid=-1;
}

sharedmemory::~sharedmemory() {
	if (created) {
		forceRemove();
	}
}

bool sharedmemory::forceRemove() {
	return !shmctl(shmid,IPC_RMID,NULL);
}

void sharedmemory::dontRemove() {
	created=false;
}

int sharedmemory::getId() const {
	return shmid;
}

void *sharedmemory::getPointer() const {
	return shmptr;
}

bool sharedmemory::setUserId(uid_t uid) {
	shmid_ds	setds;
	setds.shm_perm.uid=uid;
	return !shmctl(shmid,IPC_SET,&setds);
}

bool sharedmemory::setGroupId(gid_t gid) {
	shmid_ds	setds;
	setds.shm_perm.gid=gid;
	return !shmctl(shmid,IPC_SET,&setds);
}

bool sharedmemory::setPermissions(mode_t permissions) {
	shmid_ds	setds;
	setds.shm_perm.mode=permissions;
	return !shmctl(shmid,IPC_SET,&setds);
}

uid_t sharedmemory::getUserId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.uid;
	}
	return 0;
}

gid_t sharedmemory::getGroupId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.gid;
	}
	return 0;
}

mode_t sharedmemory::getPermissions() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.mode;
	}
	return 0;
}

bool sharedmemory::create(key_t key, size_t size, mode_t permissions) {

	// create the shared memory segment
	if ((shmid=shmget(key,size,IPC_CREAT|IPC_EXCL|permissions))!=-1) {

		// mark for removal
		created=true;

		// attach to the segment, remove the
		// segment and return 0 on failure
		shmptr=shmat(shmid,0,0);
		if (reinterpret_cast<long>(shmptr)==-1) {
			forceRemove();
			return false;
		}

		// init the segment to zero's
		rawbuffer::zero(static_cast<void *>(shmptr),size);
		return true;
	}

	return false;
}

bool sharedmemory::attach(key_t key) {

	// shmat's documentation says something like...
	// RETURN VALUE
	//	On failure shmat returns -1 with errno indicating the error.
	//	On success shmat returns the address of the attached shared
	//	memory segment.
	// So you might think, ok shmat()>-1 is success.  Ahh, but wait, shmat()
	// returns a (void *) which doesn't really have an well defined size.
	// So, it's possible for a memory address to be much bigger than the
	// upper bound of whatever really big, signed integer your system might
	// have and thus appear to be a negative number when cast to that type.
	// This appears to happen on Solaris x86 all the time.  Also, compilers
	// don't like it when you compare (void *)'s to numbers, so you have to
	// cast it.  And you can't just cast it to an unsigned integer because
	// it needs to be compared to -1.  So, we cast it to a signed integer
	// to see if it's not -1, but allow that it could very well be less
	// than -1 and still be valid.
	return ((shmid=shmget(key,0,0))!=-1 &&
			reinterpret_cast<long>(shmptr=shmat(shmid,0,0))!=-1);
}

bool sharedmemory::createOrAttach(key_t key, size_t size, mode_t permissions) {

	// create the shared memory segment
	if ((shmid=shmget(key,size,IPC_CREAT|IPC_EXCL|permissions))!=-1) {

		// mark for removal
		created=true;

		// attach to the segment, remove the
		// segment and return 0 on failure
		shmptr=shmat(shmid,0,0);
		if (reinterpret_cast<long>(shmptr)==-1) {
			forceRemove();
			return false;
		}

		// init the segment to zero's
		rawbuffer::zero(static_cast<void *>(shmptr),size);
		return true;
		
	} else if (errno==EEXIST && (shmid=shmget(key,0,permissions))!=-1) {

		// attach to the segment, return 1 on success and 0 on failure
		shmptr=shmat(shmid,0,0);
		return (reinterpret_cast<long>(shmptr)!=-1);

	}

	return false;
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

bool sharedmemory::setUserName(const char *username) {
	uid_t	userid;
	return (passwdentry::getUserId(username,&userid) &&
						setUserId(userid));
}

bool sharedmemory::setGroupName(const char *groupname) {
	gid_t	groupid;
	return (groupentry::getGroupId(groupname,&groupid) &&
						setGroupId(groupid));
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
