// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <rudiments/private/rudimentsinlines.h>

#ifndef HAVE_SEMUN
union semun {
	int	val;
	struct	semid_ds	*buf;
	ushort	*array;
};
#endif

RUDIMENTS_INLINE semaphoreset::semaphoreset() {
	waitop=NULL;
	created=0;
	semid=-1;
}

RUDIMENTS_INLINE int semaphoreset::forceRemove() {
	semun	semctlun;
	return !semctl(semid,0,IPC_RMID,semctlun);
}

RUDIMENTS_INLINE void semaphoreset::dontRemove() {
	created=0;
}

RUDIMENTS_INLINE int semaphoreset::getId() const {
	return semid;
}

RUDIMENTS_INLINE int semaphoreset::wait(int index) {
	return !semop(semid,waitop[index],1);
}

RUDIMENTS_INLINE int semaphoreset::waitWithUndo(int index) {
	return !semop(semid,waitwithundoop[index],1);
}

RUDIMENTS_INLINE int semaphoreset::signal(int index) {
	return !semop(semid,signalop[index],1);
}

RUDIMENTS_INLINE int semaphoreset::signalWithUndo(int index) {
	return !semop(semid,signalwithundoop[index],1);
}

RUDIMENTS_INLINE int semaphoreset::getValue(int index) {
	semun	semctlun;
	return semctl(semid,index,GETVAL,semctlun);
}

RUDIMENTS_INLINE int semaphoreset::setValue(int index, int value) {
	semun	semctlun;
	semctlun.val=value;
	return !semctl(semid,index,SETVAL,semctlun);
}

RUDIMENTS_INLINE int semaphoreset::getWaitingForZero(int index) {
	semun	semctlun;
	return !semctl(semid,index,GETZCNT,semctlun);
}

RUDIMENTS_INLINE int semaphoreset::getWaitingForIncrement(int index) {
	semun	semctlun;
	return !semctl(semid,index,GETNCNT,semctlun);
}

RUDIMENTS_INLINE int semaphoreset::setUserName(const char *username) {
	uid_t	userid;
	return passwdentry::getUserId(username,&userid) &&
			setUserId(userid);
}

RUDIMENTS_INLINE int semaphoreset::setGroupName(const char *groupname) {
	gid_t	groupid;
	return groupentry::getGroupId(groupname,&groupid) &&
			setGroupId(groupid);
}
