// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#ifndef __CYGWIN__

#include <rudiments/semaphoreset.h>
#ifndef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/semaphoresetinlines.h>
#endif

#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <errno.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#include <sys/stat.h>

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

int semaphoreset::create(key_t key, mode_t permissions, 
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
		created=1;

		createOperations();
		return 1;
	}

	return 0;
}

int semaphoreset::attach(key_t key, int semcount) {

	this->semcount=semcount;

	if ((semid=semget(key,semcount,0))>-1) {
		createOperations();
		return 1;
	}

	return 0;
}

int semaphoreset::createOrAttach(key_t key, mode_t permissions, 
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
		created=1;
		
	} else if (!(errno==EEXIST && 
				(semid=semget(key,semcount,permissions))>-1)) {

		return 0;
	}
	createOperations();
	return 1;
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

int semaphoreset::setUserId(uid_t uid) {
	semid_ds	setds;
	setds.sem_perm.uid=uid;
	semun	semctlun;
	semctlun.buf=&setds;
	return !semctl(semid,0,IPC_SET,semctlun);
}

int semaphoreset::setGroupId(gid_t gid) {
	semid_ds	setds;
	setds.sem_perm.gid=gid;
	semun	semctlun;
	semctlun.buf=&setds;
	return !semctl(semid,0,IPC_SET,semctlun);
}

int semaphoreset::setPermissions(mode_t permissions) {
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

#endif
