// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE sharedmemory::sharedmemory() {
	created=false;
	shmptr=NULL;
	shmid=-1;
}

RUDIMENTS_INLINE sharedmemory::~sharedmemory() {
	if (created) {
		forceRemove();
	}
}

RUDIMENTS_INLINE bool sharedmemory::forceRemove() {
	return !shmctl(shmid,IPC_RMID,NULL);
}

RUDIMENTS_INLINE void sharedmemory::dontRemove() {
	created=false;
}

RUDIMENTS_INLINE int sharedmemory::getId() const {
	return shmid;
}

RUDIMENTS_INLINE void *sharedmemory::getPointer() const {
	return shmptr;
}

RUDIMENTS_INLINE bool sharedmemory::setUserName(const char *username) {
	uid_t	userid;
	return passwdentry::getUserId(username,&userid) &&
			setUserId(userid);
}

RUDIMENTS_INLINE bool sharedmemory::setGroupName(const char *groupname) {
	gid_t	groupid;
	return groupentry::getGroupId(groupname,&groupid) &&
			setGroupId(groupid);
}

RUDIMENTS_INLINE bool sharedmemory::setUserId(uid_t uid) {
	shmid_ds	setds;
	setds.shm_perm.uid=uid;
	return !shmctl(shmid,IPC_SET,&setds);
}

RUDIMENTS_INLINE bool sharedmemory::setGroupId(gid_t gid) {
	shmid_ds	setds;
	setds.shm_perm.gid=gid;
	return !shmctl(shmid,IPC_SET,&setds);
}

RUDIMENTS_INLINE bool sharedmemory::setPermissions(mode_t permissions) {
	shmid_ds	setds;
	setds.shm_perm.mode=permissions;
	return !shmctl(shmid,IPC_SET,&setds);
}

RUDIMENTS_INLINE uid_t sharedmemory::getUserId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.uid;
	}
	return 0;
}

RUDIMENTS_INLINE gid_t sharedmemory::getGroupId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.gid;
	}
	return 0;
}

RUDIMENTS_INLINE mode_t sharedmemory::getPermissions() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.mode;
	}
	return 0;
}
