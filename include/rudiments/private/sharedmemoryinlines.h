// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information.

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

inline sharedmemory::sharedmemory() {
	created=0;
	shmptr=NULL;
	shmid=-1;
}

inline sharedmemory::~sharedmemory() {
	if (created) {
		forceRemove();
	}
}

inline int	sharedmemory::forceRemove() {
	return !shmctl(shmid,IPC_RMID,NULL);
}

inline void	sharedmemory::dontRemove() {
	created=0;
}

inline int	sharedmemory::getId() const {
	return shmid;
}

inline void	*sharedmemory::getPointer() const {
	return shmptr;
}

inline int	sharedmemory::setUserId(ushort uid) {
	shmid_ds	setds;
	setds.shm_perm.uid=uid;
	return !shmctl(shmid,IPC_SET,&setds);
}

inline int	sharedmemory::setGroupId(ushort gid) {
	shmid_ds	setds;
	setds.shm_perm.gid=gid;
	return !shmctl(shmid,IPC_SET,&setds);
}

inline int	sharedmemory::setPermissions(mode_t permissions) {
	shmid_ds	setds;
	setds.shm_perm.mode=permissions;
	return !shmctl(shmid,IPC_SET,&setds);
}

inline unsigned short	sharedmemory::getUserId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.uid;
	}
	return 0;
}

inline unsigned short	sharedmemory::getGroupId() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.gid;
	}
	return 0;
}

inline mode_t	sharedmemory::getPermissions() {
	shmid_ds	getds;
	if (!shmctl(shmid,IPC_STAT,&getds)) {
		return getds.shm_perm.mode;
	}
	return 0;
}
