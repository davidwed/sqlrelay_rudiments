// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/error.h>

#include <stdio.h>
#include <stdlib.h>
#ifdef RUDIMENTS_HAVE_SYS_IPC_H
	#include <sys/ipc.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class semaphoresetprivate {
	public:
			int	_semid;
			bool	_created;
			int	_semcount;
		struct	sembuf	**_waitop;
		struct	sembuf	**_waitwithundoop;
		struct	sembuf	**_signalop;
		struct	sembuf	**_signalwithundoop;
};

#ifndef RUDIMENTS_HAVE_SEMUN
union semun {
	int	val;
	struct	semid_ds	*buf;
	ushort	*array;
};
#endif

// lame that this isn't part of the class, but I can't think of another way to
// keep #ifndef RUDIMENTS_HAVE_SEMUN out of the header file
static int semControl(semaphoresetprivate *pvt, int semnum,
						int cmd, semun semctlun) {
	int	result;
	do {
		result=semctl(pvt->_semid,semnum,cmd,semctlun);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

semaphoreset::semaphoreset() {
	pvt=new semaphoresetprivate;
	pvt->_semid=-1;
	pvt->_created=false;
	pvt->_waitop=NULL;
	pvt->_waitwithundoop=NULL;
	pvt->_signalop=NULL;
	pvt->_signalwithundoop=NULL;
}

semaphoreset::~semaphoreset() {

	if (pvt->_waitop) {
		for (int i=0; i<pvt->_semcount; i++) {
			delete[] pvt->_waitop[i];
			delete[] pvt->_waitwithundoop[i];
			delete[] pvt->_signalop[i];
			delete[] pvt->_signalwithundoop[i];
		}
		delete[] pvt->_waitop;
		delete[] pvt->_waitwithundoop;
		delete[] pvt->_signalop;
		delete[] pvt->_signalwithundoop;
	}

	if (pvt->_created) {
		forceRemove();
	}

	delete pvt;
}

bool semaphoreset::forceRemove() {
	semun	semctlun;
	return !semControl(pvt,0,IPC_RMID,semctlun);
}

void semaphoreset::dontRemove() {
	pvt->_created=false;
}

int semaphoreset::getId() const {
	return pvt->_semid;
}

bool semaphoreset::wait(int index) {
	return semOp(pvt->_waitop[index]);
}

bool semaphoreset::wait(int index, long seconds, long nanoseconds) {
	timespec	ts;
	ts.tv_sec=seconds;
	ts.tv_nsec=nanoseconds;
	return semTimedOp(pvt->_waitop[index],&ts);
}

bool semaphoreset::waitWithUndo(int index) {
	return semOp(pvt->_waitwithundoop[index]);
}

bool semaphoreset::waitWithUndo(int index, long seconds, long nanoseconds) {
	timespec	ts;
	ts.tv_sec=seconds;
	ts.tv_nsec=nanoseconds;
	return semTimedOp(pvt->_waitwithundoop[index],&ts);
}

bool semaphoreset::signal(int index) {
	return semOp(pvt->_signalop[index]);
}

bool semaphoreset::signalWithUndo(int index) {
	return semOp(pvt->_signalwithundoop[index]);
}

int semaphoreset::getValue(int index) {
	semun	semctlun;
	return semControl(pvt,index,GETVAL,semctlun);
}

bool semaphoreset::setValue(int index, int value) {
	semun	semctlun;
	semctlun.val=value;
	return !semControl(pvt,index,SETVAL,semctlun);
}

int semaphoreset::getWaitingForZero(int index) {
	semun	semctlun;
	return semControl(pvt,index,GETZCNT,semctlun);
}

int semaphoreset::getWaitingForIncrement(int index) {
	semun	semctlun;
	return semControl(pvt,index,GETNCNT,semctlun);
}

bool semaphoreset::create(key_t key, mode_t permissions, 
					int semcount, const int *values) {

	pvt->_semcount=semcount;

	// create the semaphore
	if ((pvt->_semid=semGet(key,semcount,
				IPC_CREAT|IPC_EXCL|permissions))!=-1) {

		// if creation succeeded, initialize the semaphore
		if (values) {
			for (int i=0; i<semcount; i++) {
				setValue(i,values[i]);
			}
		}

		// mark for removal
		pvt->_created=true;

		createOperations();
		return true;
	}

	return false;
}

bool semaphoreset::attach(key_t key, int semcount) {

	pvt->_semcount=semcount;

	if ((pvt->_semid=semGet(key,semcount,0))!=-1) {
		createOperations();
		return true;
	}

	return false;
}

bool semaphoreset::createOrAttach(key_t key, mode_t permissions, 
					int semcount, const int *values) {

	pvt->_semcount=semcount;

	// create the semaphore
	if ((pvt->_semid=semGet(key,semcount,
				IPC_CREAT|IPC_EXCL|permissions))!=-1) {

		// if creation succeeded, initialize the semaphore
		if (values) {
			for (int i=0; i<semcount; i++) {
				setValue(i,values[i]);
			}
		}

		// mark for removal
		pvt->_created=true;
		
	} else if (!(error::getErrorNumber()==EEXIST && 
			(pvt->_semid=semGet(key,semcount,permissions))!=-1)) {

		return false;
	}
	createOperations();
	return true;
}

void semaphoreset::createOperations() {

	pvt->_waitop=new sembuf *[pvt->_semcount];
	pvt->_waitwithundoop=new sembuf *[pvt->_semcount];
	pvt->_signalop=new sembuf *[pvt->_semcount];
	pvt->_signalwithundoop=new sembuf *[pvt->_semcount];

	for (int i=0; i<pvt->_semcount; i++) {

		// wait without undo
		pvt->_waitop[i]=new sembuf[1];
		pvt->_waitop[i][0].sem_num=(short)i;
		pvt->_waitop[i][0].sem_op=-1;
		pvt->_waitop[i][0].sem_flg=0;

		// wait with undo
		pvt->_waitwithundoop[i]=new sembuf[1];
		pvt->_waitwithundoop[i][0].sem_num=(short)i;
		pvt->_waitwithundoop[i][0].sem_op=-1;
		pvt->_waitwithundoop[i][0].sem_flg=SEM_UNDO;

		// signal without undo
		pvt->_signalop[i]=new sembuf[1];
		pvt->_signalop[i][0].sem_num=(short)i;
		pvt->_signalop[i][0].sem_op=1;
		pvt->_signalop[i][0].sem_flg=0;

		// signal with undo
		pvt->_signalwithundoop[i]=new sembuf[1];
		pvt->_signalwithundoop[i][0].sem_num=(short)i;
		pvt->_signalwithundoop[i][0].sem_op=1;
		pvt->_signalwithundoop[i][0].sem_flg=SEM_UNDO;
	}
}

bool semaphoreset::setUserId(uid_t uid) {
	semid_ds	setds;
	setds.sem_perm.uid=uid;
	semun	semctlun;
	semctlun.buf=&setds;
	return !semControl(pvt,0,IPC_SET,semctlun);
}

bool semaphoreset::setGroupId(gid_t gid) {
	semid_ds	setds;
	setds.sem_perm.gid=gid;
	semun	semctlun;
	semctlun.buf=&setds;
	return !semControl(pvt,0,IPC_SET,semctlun);
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
	return !semControl(pvt,0,IPC_SET,semctlun);
}

const char *semaphoreset::getUserName() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	char		*name;
	if (!semControl(pvt,0,IPC_STAT,semctlun) &&
			passwdentry::getName(getds.sem_perm.uid,&name)) {
		return name;
	}
	return NULL;
}

uid_t semaphoreset::getUserId() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	if (!semControl(pvt,0,IPC_STAT,semctlun)) {
		return (short)getds.sem_perm.uid;
	}
	return 0;
}

const char *semaphoreset::getGroupName() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	char		*name;
	if (!semControl(pvt,0,IPC_STAT,semctlun) &&
			groupentry::getName(getds.sem_perm.gid,&name)) {
		return name;
	}
	return NULL;
}

gid_t semaphoreset::getGroupId() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	if (!semControl(pvt,0,IPC_STAT,semctlun)) {
		return (short)getds.sem_perm.gid;
	}
	return 0;
}

mode_t semaphoreset::getPermissions() {
	semid_ds	getds;
	semun		semctlun;
	semctlun.buf=&getds;
	if (!semControl(pvt,0,IPC_STAT,semctlun)) {
		return getds.sem_perm.mode;
	}
	return 0;
}

int semaphoreset::semGet(key_t key, int nsems, int semflg) {
	int	result;
	do {
		result=semget(key,nsems,semflg);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool semaphoreset::semOp(struct sembuf *sops) {
	int	result;
	do {
		result=semop(pvt->_semid,sops,1);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool semaphoreset::semTimedOp(struct sembuf *sops, timespec *ts) {
#ifdef RUDIMENTS_HAVE_SEMTIMEDOP
	int	result;
	do {
		result=semtimedop(pvt->_semid,sops,1,ts);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
#else
	return false;
#endif
}

bool semaphoreset::supportsTimedSemaphoreOperations() {
#ifdef RUDIMENTS_HAVE_SEMTIMEDOP
	return true;
#else
	return false;
#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
