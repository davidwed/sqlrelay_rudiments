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

#ifdef RUDIMENTS_HAVE_SYS_SEM_H
	// for struct sembuf
	#include <sys/sem.h>
#else
	struct sembuf {
		uint16_t	sem_num;
		int16_t		sem_op;
		int16_t		sem_flg;
	};
#endif

#ifndef RUDIMENTS_HAVE_SEMUN
	union semun {
		int			val;
		struct	semid_ds	*buf;
		unsigned short		*array;
	};
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class semaphoresetprivate {
	public:
		int	_semid;
		bool	_created;
		int	_semcount;
		#if defined(RUDIMENTS_HAVE_SEMGET)
			struct	sembuf	**_waitop;
			struct	sembuf	**_waitwithundoop;
			struct	sembuf	**_signalop;
			struct	sembuf	**_signalwithundoop;
		#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
			HANDLE	*_semaphores;
		#endif
};

semaphoreset::semaphoreset() {
	pvt=new semaphoresetprivate;
	pvt->_semid=-1;
	pvt->_created=false;
	pvt->_semcount=0;
	#if defined(RUDIMENTS_HAVE_SEMGET)
		pvt->_waitop=NULL;
		pvt->_waitwithundoop=NULL;
		pvt->_signalop=NULL;
		pvt->_signalwithundoop=NULL;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		pvt->_semaphores=NULL;
	#endif
}

semaphoreset::~semaphoreset() {

	#if defined(RUDIMENTS_HAVE_SEMGET)
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
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		if (pvt->_semaphores) {
			for (int i=0; i<pvt->_semcount; i++) {
				CloseHandle(pvt->_semaphores[i]);
			}
			delete[] pvt->_semaphores;
		}
	#endif

	if (pvt->_created) {
		forceRemove();
	}

	delete pvt;
}

bool semaphoreset::forceRemove() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return !semControl(pvt,0,IPC_RMID,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

void semaphoreset::dontRemove() {
	pvt->_created=false;
}

int semaphoreset::getId() const {
	return pvt->_semid;
}

bool semaphoreset::wait(int index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_waitop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::wait(int index, long seconds, long nanoseconds) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		timespec	ts;
		ts.tv_sec=seconds;
		ts.tv_nsec=nanoseconds;
		return semTimedOp(pvt->_waitop[index],&ts);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::waitWithUndo(int index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_waitwithundoop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::waitWithUndo(int index, long seconds, long nanoseconds) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		timespec	ts;
		ts.tv_sec=seconds;
		ts.tv_nsec=nanoseconds;
		return semTimedOp(pvt->_waitwithundoop[index],&ts);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::signal(int index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_signalop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::signalWithUndo(int index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_signalwithundoop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int semaphoreset::getValue(int index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETVAL,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::setValue(int index, int value) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		semctlun.val=value;
		return !semControl(pvt,index,SETVAL,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int semaphoreset::getWaitingForZero(int index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETZCNT,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int semaphoreset::getWaitingForIncrement(int index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETNCNT,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::create(key_t key, mode_t permissions, 
					int semcount, const int *values) {

	#if !defined(RUDIMENTS_HAVE_SEMGET) && \
		!defined(RUDIMENTS_HAVE_CREATESEMAPHORE)

		error::setErrorNumber(ENOSYS);
		return false;

	#else

		pvt->_semcount=semcount;

		// create the semaphore
		#if defined(RUDIMENTS_HAVE_SEMGET)
			if ((pvt->_semid=semGet(key,semcount,
					IPC_CREAT|IPC_EXCL|permissions))!=-1) {
		#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
			#error not sure what to do here...
			if (true) {
		#endif
	
			// if creation succeeded, initialize the semaphore
			if (values) {
				for (int i=0; i<semcount; i++) {
					setValue(i,values[i]);
				}
			}
	
			// mark for removal
			pvt->_created=true;
	
			#ifdef RUDIMENTS_HAVE_SEMGET
				// create the signal/wait operations
				createOperations();
			#endif
	
			return true;
		}

		return false;
	#endif
}

bool semaphoreset::attach(key_t key, int semcount) {

	pvt->_semcount=semcount;

	// attach to the semaphore
	#if defined(RUDIMENTS_HAVE_SEMGET)
	if ((pvt->_semid=semGet(key,semcount,0))!=-1) {
		createOperations();
		return true;
	}
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
	#endif

	return false;
}

bool semaphoreset::createOrAttach(key_t key, mode_t permissions, 
					int semcount, const int *values) {
	if (create(key,permissions,semcount,values)) {
		return true;
	}
	if (error::getErrorNumber()==EEXIST) {
		return attach(key,semcount);
	}
	return false;
}

void semaphoreset::createOperations() {

	#if defined(RUDIMENTS_HAVE_SEMGET)
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
	#endif
}

bool semaphoreset::setUserId(uid_t uid) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	setds;
		setds.sem_perm.uid=uid;
		semun	semctlun;
		semctlun.buf=&setds;
		return !semControl(pvt,0,IPC_SET,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::setGroupId(gid_t gid) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	setds;
		setds.sem_perm.gid=gid;
		semun	semctlun;
		semctlun.buf=&setds;
		return !semControl(pvt,0,IPC_SET,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
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
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	setds;
		setds.sem_perm.mode=permissions;
		semun	semctlun;
		semctlun.buf=&setds;
		return !semControl(pvt,0,IPC_SET,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

const char *semaphoreset::getUserName() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	getds;
		semun		semctlun;
		semctlun.buf=&getds;
		char		*name;
		if (!semControl(pvt,0,IPC_STAT,&semctlun) &&
			passwdentry::getName(getds.sem_perm.uid,&name)) {
			return name;
		}
		return NULL;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

uid_t semaphoreset::getUserId() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	getds;
		semun		semctlun;
		semctlun.buf=&getds;
		if (!semControl(pvt,0,IPC_STAT,&semctlun)) {
			return (short)getds.sem_perm.uid;
		}
		return 0;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

const char *semaphoreset::getGroupName() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	getds;
		semun		semctlun;
		semctlun.buf=&getds;
		char		*name;
		if (!semControl(pvt,0,IPC_STAT,&semctlun) &&
				groupentry::getName(getds.sem_perm.gid,&name)) {
			return name;
		}
		return NULL;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

gid_t semaphoreset::getGroupId() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	getds;
		semun		semctlun;
		semctlun.buf=&getds;
		if (!semControl(pvt,0,IPC_STAT,&semctlun)) {
			return (short)getds.sem_perm.gid;
		}
		return 0;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

mode_t semaphoreset::getPermissions() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	getds;
		semun		semctlun;
		semctlun.buf=&getds;
		if (!semControl(pvt,0,IPC_STAT,&semctlun)) {
			return getds.sem_perm.mode;
		}
		return 0;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int semaphoreset::semGet(key_t key, int nsems, int semflg) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		int	result;
		do {
			result=semget(key,nsems,semflg);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int semaphoreset::semControl(semaphoresetprivate *pvt, int semnum,
						int cmd, semun *semctlun) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		int	result;
		do {
			result=semctl(pvt->_semid,semnum,cmd,*semctlun);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool semaphoreset::semOp(struct sembuf *sops) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		int	result;
		do {
			result=semop(pvt->_semid,sops,1);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::semTimedOp(struct sembuf *sops, timespec *ts) {
	#if defined(RUDIMENTS_HAVE_SEMTIMEDOP)
		int	result;
		do {
			result=semtimedop(pvt->_semid,sops,1,ts);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		#error not sure what to do here...
	#else
		error::setErrorNumber(ENOSYS);
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
