// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#ifndef HAVE_SEMUN
union semun {
	int	val;
	struct	semid_ds	*buf;
	ushort	*array;
};
#endif

semaphoreset::semaphoreset() {
	waitop=NULL;
	created=false;
	semid=-1;
}

semaphoreset::~semaphoreset() {

	if (waitop) {
		for (int i=0; i<semcount; i++) {
			delete[] waitop[i];
			delete[] waitwithundoop[i];
			delete[] signalop[i];
			delete[] signalwithundoop[i];
		}
		delete[] waitop;
		delete[] waitwithundoop;
		delete[] signalop;
		delete[] signalwithundoop;
	}

	if (created) {
		forceRemove();
	}
}

bool semaphoreset::forceRemove() {
	semun	semctlun;
	return !semctl(semid,0,IPC_RMID,semctlun);
}

void semaphoreset::dontRemove() {
	created=false;
}

int semaphoreset::getId() const {
	return semid;
}

bool semaphoreset::wait(int index) {
	return !semop(semid,waitop[index],1);
}

#ifdef HAVE_SEMTIMEDOP
bool semaphoreset::wait(int index, long seconds, long nanoseconds) {
	timespec	ts;
	ts.tv_sec=seconds;
	ts.tv_nsec=nanoseconds;
	return !semtimedop(semid,waitop[index],1,&ts);
}
#endif

bool semaphoreset::waitWithUndo(int index) {
	return !semop(semid,waitwithundoop[index],1);
}

#ifdef HAVE_SEMTIMEDOP
bool semaphoreset::waitWithUndo(int index, long seconds, long nanoseconds) {
	timespec	ts;
	ts.tv_sec=seconds;
	ts.tv_nsec=nanoseconds;
	return !semtimedop(semid,waitwithundoop[index],1,&ts);
}
#endif

bool semaphoreset::signal(int index) {
	return !semop(semid,signalop[index],1);
}

bool semaphoreset::signalWithUndo(int index) {
	return !semop(semid,signalwithundoop[index],1);
}

int semaphoreset::getValue(int index) {
	semun	semctlun;
	return semctl(semid,index,GETVAL,semctlun);
}

bool semaphoreset::setValue(int index, int value) {
	semun	semctlun;
	semctlun.val=value;
	return !semctl(semid,index,SETVAL,semctlun);
}

int semaphoreset::getWaitingForZero(int index) {
	semun	semctlun;
	return semctl(semid,index,GETZCNT,semctlun);
}

int semaphoreset::getWaitingForIncrement(int index) {
	semun	semctlun;
	return semctl(semid,index,GETNCNT,semctlun);
}

bool semaphoreset::create(key_t key, mode_t permissions, 
					int semcount, const int *values) {

	this->semcount=semcount;

	// create the semaphore
	if ((semid=semget(key,semcount,IPC_CREAT|IPC_EXCL|permissions))>-1) {

		// if creation succeeded, initialize the semaphore
		if (values) {
			for (int i=0; i<semcount; i++) {
				setValue(i,values[i]);
			}
		}

		// mark for removal
		created=true;

		createOperations();
		return true;
	}

	return false;
}

bool semaphoreset::attach(key_t key, int semcount) {

	this->semcount=semcount;

	if ((semid=semget(key,semcount,0))>-1) {
		createOperations();
		return true;
	}

	return false;
}

bool semaphoreset::createOrAttach(key_t key, mode_t permissions, 
					int semcount, const int *values) {

	this->semcount=semcount;

	// create the semaphore
	if ((semid=semget(key,semcount,IPC_CREAT|IPC_EXCL|permissions))>-1) {

		// if creation succeeded, initialize the semaphore
		if (values) {
			for (int i=0; i<semcount; i++) {
				setValue(i,values[i]);
			}
		}

		// mark for removal
		created=true;
		
	} else if (!(errno==EEXIST && 
				(semid=semget(key,semcount,permissions))>-1)) {

		return false;
	}
	createOperations();
	return true;
}

void semaphoreset::createOperations() {

	waitop=new sembuf *[semcount];
	waitwithundoop=new sembuf *[semcount];
	signalop=new sembuf *[semcount];
	signalwithundoop=new sembuf *[semcount];

	for (int i=0; i<semcount; i++) {

		// wait without undo
		waitop[i]=new sembuf[1];
		waitop[i][0].sem_num=(short)i;
		waitop[i][0].sem_op=-1;
		waitop[i][0].sem_flg=0;

		// wait with undo
		waitwithundoop[i]=new sembuf[1];
		waitwithundoop[i][0].sem_num=(short)i;
		waitwithundoop[i][0].sem_op=-1;
		waitwithundoop[i][0].sem_flg=SEM_UNDO;

		// signal without undo
		signalop[i]=new sembuf[1];
		signalop[i][0].sem_num=(short)i;
		signalop[i][0].sem_op=1;
		signalop[i][0].sem_flg=0;

		// signal with undo
		signalwithundoop[i]=new sembuf[1];
		signalwithundoop[i][0].sem_num=(short)i;
		signalwithundoop[i][0].sem_op=1;
		signalwithundoop[i][0].sem_flg=SEM_UNDO;
	}
}

bool semaphoreset::setUserId(uid_t uid) {
	semid_ds	setds;
	setds.sem_perm.uid=uid;
	semun	semctlun;
	semctlun.buf=&setds;
	return !semctl(semid,0,IPC_SET,semctlun);
}

bool semaphoreset::setGroupId(gid_t gid) {
	semid_ds	setds;
	setds.sem_perm.gid=gid;
	semun	semctlun;
	semctlun.buf=&setds;
	return !semctl(semid,0,IPC_SET,semctlun);
}

bool semaphoreset::setUserName(const char *username) {
	uid_t	userid;
	return passwdentry::getUserId(username,&userid) &&
			setUserId(userid);
}

bool semaphoreset::setGroupName(const char *groupname) {
	gid_t	groupid;
	return groupentry::getGroupId(groupname,&groupid) &&
			setGroupId(groupid);
}

bool semaphoreset::setPermissions(mode_t permissions) {
	semid_ds	setds;
	setds.sem_perm.mode=permissions;
	semun	semctlun;
	semctlun.buf=&setds;
	return !semctl(semid,0,IPC_SET,semctlun);
}

char *semaphoreset::getUserName() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	char		*name;
	if (!semctl(semid,0,IPC_STAT,semctlun) &&
			passwdentry::getName(getds.sem_perm.uid,&name)) {
		return name;
	}
	return NULL;
}

uid_t semaphoreset::getUserId() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	if (!semctl(semid,0,IPC_STAT,semctlun)) {
		return (short)getds.sem_perm.uid;
	}
	return 0;
}

char *semaphoreset::getGroupName() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	char		*name;
	if (!semctl(semid,0,IPC_STAT,semctlun) &&
			groupentry::getName(getds.sem_perm.gid,&name)) {
		return name;
	}
	return NULL;
}

gid_t semaphoreset::getGroupId() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	if (!semctl(semid,0,IPC_STAT,semctlun)) {
		return (short)getds.sem_perm.gid;
	}
	return 0;
}

mode_t semaphoreset::getPermissions() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	if (!semctl(semid,0,IPC_STAT,semctlun)) {
		return getds.sem_perm.mode;
	}
	return 0;
}
