// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

INLINE sharedmemory::sharedmemory() {
	created=0;
	shmptr=NULL;
	shmid=-1;
}

INLINE sharedmemory::~sharedmemory() {
	if (created) {
		forceRemove();
	}
}

INLINE int sharedmemory::forceRemove() {
	return !shmctl(shmid,IPC_RMID,NULL);
}

INLINE void sharedmemory::dontRemove() {
	created=0;
}

INLINE int sharedmemory::getId() const {
	return shmid;
}

INLINE void *sharedmemory::getPointer() const {
	return shmptr;
}

INLINE int sharedmemory::setUserName(const char *username) {
	uid_t	userid;
	return passwdentry::getUserId(username,&userid) &&
			setUserId(userid);
}

INLINE int sharedmemory::setGroupName(const char *groupname) {
	gid_t	groupid;
	return groupentry::getGroupId(groupname,&groupid) &&
			setGroupId(groupid);
}

INLINE int sharedmemory::setUserId(uid_t uid) {
	shmid_ds	setds;
	setds.shm_perm.uid=uid;
	return !shmctl(shmid,IPC_SET,&setds);
}

INLINE int sharedmemory::setGroupId(gid_t gid) {
	shmid_ds	setds;
	setds.shm_perm.gid=gid;
	return !shmctl(shmid,IPC_SET,&setds);
}

INLINE int sharedmemory::setPermissions(mode_t permissions) {
	shmid_ds	setds;
	setds.shm_perm.mode=permissions;
	return !shmctl(shmid,IPC_SET,&setds);
}

INLINE uid_t sharedmemory::getUserId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.uid;
	}
	return 0;
}

INLINE gid_t sharedmemory::getGroupId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.gid;
	}
	return 0;
}

INLINE mode_t sharedmemory::getPermissions() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.mode;
	}
	return 0;
}
