// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#ifndef HAVE_SEMUN
union semun {
	int	val;
	struct	semid_ds	*buf;
	ushort	*array;
};
#endif

inline semaphoreset::semaphoreset() {
	waitop=NULL;
	created=0;
	semid=-1;
}

inline int semaphoreset::forceRemove() {
	semun	semctlun;
	return !semctl(semid,0,IPC_RMID,semctlun);
}

inline void semaphoreset::dontRemove() {
	created=0;
}

inline int semaphoreset::getId() const {
	return semid;
}

inline int semaphoreset::wait(int index) {
	return !semop(semid,waitop[index],1);
}

inline int semaphoreset::waitWithUndo(int index) {
	return !semop(semid,waitwithundoop[index],1);
}

inline int semaphoreset::signal(int index) {
	return !semop(semid,signalop[index],1);
}

inline int semaphoreset::signalWithUndo(int index) {
	return !semop(semid,signalwithundoop[index],1);
}

inline int semaphoreset::getValue(int index) {
	semun	semctlun;
	return !semctl(semid,index,GETVAL,semctlun);
}

inline int semaphoreset::setValue(int index, int value) {
	semun	semctlun;
	semctlun.val=value;
	return !semctl(semid,index,SETVAL,semctlun);
}

inline int semaphoreset::getWaitingForZero(int index) {
	semun	semctlun;
	return !semctl(semid,index,GETZCNT,semctlun);
}

inline int semaphoreset::getWaitingForIncrement(int index) {
	semun	semctlun;
	return !semctl(semid,index,GETNCNT,semctlun);
}
