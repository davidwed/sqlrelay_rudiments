// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_CREATESEMAPHORE
	#include <rudiments/charstring.h>
#endif

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
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
		int32_t			val;
		struct	semid_ds	*buf;
		unsigned short		*array;
	};
#endif

#ifndef RUDIMENTS_HAVE_SEMGET
	#define	IPC_CREAT	1
	#define	IPC_EXCL 	2
#endif

class semaphoresetprivate {
	public:
		int32_t	_semid;
		bool	_created;
		int32_t	_semcount;
		bool	_retryinterruptedoperations;
		#if defined(RUDIMENTS_HAVE_SEMGET)
			struct	sembuf	**_waitop;
			struct	sembuf	**_waitwithundoop;
			struct	sembuf	**_signalop;
			struct	sembuf	**_signalwithundoop;
		#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
			HANDLE			*_sems;
			SECURITY_ATTRIBUTES	**_securityattrs;
			char			**_semnames;
		#endif
};

semaphoreset::semaphoreset() {
	pvt=new semaphoresetprivate;
	pvt->_semid=-1;
	pvt->_created=false;
	pvt->_semcount=0;
	pvt->_retryinterruptedoperations=true;
	#if defined(RUDIMENTS_HAVE_SEMGET)
		pvt->_waitop=NULL;
		pvt->_waitwithundoop=NULL;
		pvt->_signalop=NULL;
		pvt->_signalwithundoop=NULL;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		pvt->_sems=NULL;
		pvt->_securityattrs=NULL;
		pvt->_semnames=NULL;
	#endif
}

semaphoreset::~semaphoreset() {

	#if defined(RUDIMENTS_HAVE_SEMGET)
		if (pvt->_waitop) {
			for (int32_t i=0; i<pvt->_semcount; i++) {
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
		if (pvt->_sems) {
			for (int32_t i=0; i<pvt->_semcount; i++) {
				CloseHandle(pvt->_sems[i]);
				delete[] pvt->_securityattrs[i];
				delete[] pvt->_semnames[i];
			}
			delete[] pvt->_sems;
			delete[] pvt->_securityattrs;
			delete[] pvt->_semnames;
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
		// this isn't possible on windows, the semaphore will be
		// destroyed when the last process that held it open exits
		return true;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

void semaphoreset::dontRemove() {
	pvt->_created=false;
}

int32_t semaphoreset::getId() const {
	return pvt->_semid;
}

bool semaphoreset::wait(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_waitop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return WaitForSingleObject(pvt->_sems[index],INFINITE);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::wait(int32_t index, int32_t seconds, int32_t nanoseconds) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semTimedOp(pvt->_waitop[index],seconds,nanoseconds);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return WaitForSingleObject(pvt->_sems[index],
					seconds*1000+nanoseconds/1000000);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::waitWithUndo(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_waitwithundoop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// no such thing as undo on windows
		return WaitForSingleObject(pvt->_sems[index],INFINITE);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::waitWithUndo(int32_t index,
				int32_t seconds, int32_t nanoseconds) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semTimedOp(pvt->_waitwithundoop[index],
						seconds,nanoseconds);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// no such thing as undo on windows
		return WaitForSingleObject(pvt->_sems[index],
					seconds*1000+nanoseconds/1000000);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::signal(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_signalop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return ReleaseSemaphore(pvt->_sems[index],1,NULL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::signalWithUndo(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_signalwithundoop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// no such thing as undo on windows
		return ReleaseSemaphore(pvt->_sems[index],1,NULL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int32_t semaphoreset::getValue(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETVAL,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::setValue(int32_t index, int32_t value) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		semctlun.val=value;
		return !semControl(pvt,index,SETVAL,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int32_t semaphoreset::getWaitingForZero(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETZCNT,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int32_t semaphoreset::getWaitingForIncrement(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETNCNT,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::create(key_t key, mode_t permissions, 
				int32_t semcount, const int32_t *values) {

	pvt->_semcount=semcount;

	#if defined(RUDIMENTS_HAVE_SEMGET) || \
		defined(RUDIMENTS_HAVE_CREATESEMAPHORE)

		// create the semaphore
		if ((pvt->_semid=semGet(key,semcount,
					IPC_CREAT|IPC_EXCL|permissions))!=-1) {
	
			// if creation succeeded, initialize the semaphore
			if (values) {
				for (int32_t i=0; i<semcount; i++) {
					setValue(i,values[i]);
				}
			}
	
			// mark for removal
			pvt->_created=true;
	
			// create the signal/wait operations
			createOperations();
	
			return true;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif

	return false;
}

bool semaphoreset::attach(key_t key, int32_t semcount) {

	pvt->_semcount=semcount;

	#if defined(RUDIMENTS_HAVE_SEMGET) || \
		defined(RUDIMENTS_HAVE_CREATESEMAPHORE)

		// attach to the semaphore
		if ((pvt->_semid=semGet(key,semcount,0))!=-1) {

			// create the signal/wait operations
			createOperations();

			return true;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif

	return false;
}

bool semaphoreset::createOrAttach(key_t key, mode_t permissions, 
				int32_t semcount, const int32_t *values) {
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

		for (int32_t i=0; i<pvt->_semcount; i++) {

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
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return false;
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
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::setUserName(const char *username) {
	uid_t	userid=passwdentry::getUserId(username);
	return (userid!=(uid_t)-1 && setUserId(userid));
}

bool semaphoreset::setGroupName(const char *groupname) {
	gid_t	groupid=groupentry::getGroupId(groupname);
	return (groupid!=(gid_t)-1 && setGroupId(groupid));
}

bool semaphoreset::setPermissions(mode_t permissions) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	setds;
		setds.sem_perm.mode=permissions;
		semun	semctlun;
		semctlun.buf=&setds;
		return !semControl(pvt,0,IPC_SET,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

const char *semaphoreset::getUserName() {
	// FIXME: memory leak here
	return passwdentry::getName(getUserId());
}

uid_t semaphoreset::getUserId() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	getds;
		semun		semctlun;
		semctlun.buf=&getds;
		if (!semControl(pvt,0,IPC_STAT,&semctlun)) {
			return (short)getds.sem_perm.uid;
		}
		return (uid_t)-1;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return (uid_t)-1;
	#else
		error::setErrorNumber(ENOSYS);
		return (uid_t)-1;
	#endif
}

const char *semaphoreset::getGroupName() {
	// FIXME: memory leak here
	return groupentry::getName(getGroupId());
}

gid_t semaphoreset::getGroupId() {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semid_ds	getds;
		semun		semctlun;
		semctlun.buf=&getds;
		if (!semControl(pvt,0,IPC_STAT,&semctlun)) {
			return (short)getds.sem_perm.gid;
		}
		return (uid_t)-1;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return (gid_t)-1;
	#else
		error::setErrorNumber(ENOSYS);
		return (gid_t)-1;
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
		// FIXME: there's got to be a way to do this...
		error::setErrorNumber(ENOSYS);
		return 0;
	#else
		error::setErrorNumber(ENOSYS);
		return 0;
	#endif
}

int32_t semaphoreset::semGet(key_t key, int32_t nsems, int32_t semflg) {
	#if defined(RUDIMENTS_HAVE_SEMGET)

		int32_t	result;
		do {
			result=semget(key,nsems,semflg);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);
		return result;

	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)

		// FIXME: delete first?
		pvt->_sems=new HANDLE[nsems];
		pvt->_securityattrs=new SECURITY_ATTRIBUTES *[nsems];
		pvt->_semnames=new char *[nsems];

		for (int32_t i=0; i<nsems; i++) {

			// set the semaphore name
			int32_t	semnamelen=
					11+charstring::integerLength(key)+1+
					charstring::integerLength(nsems)+1;
			pvt->_semnames[i]=new char[semnamelen];
			charstring::copy(pvt->_semnames[i],"rudiments::");
			charstring::append(pvt->_semnames[i],key);
			charstring::append(pvt->_semnames[i],"-");
			charstring::append(pvt->_semnames[i],nsems);

			// set up the security attributes
			pvt->_securityattrs[i]=new SECURITY_ATTRIBUTES;
			pvt->_securityattrs[i]->nLength=
						sizeof(SECURITY_ATTRIBUTES);
			// FIXME: set up security descriptor
			pvt->_securityattrs[i]->lpSecurityDescriptor=NULL;
			pvt->_securityattrs[i]->bInheritHandle=TRUE;
			

			if (semflg&(IPC_CREAT|IPC_EXCL)) {

				// create a new semaphore
				HANDLE	sem=CreateSemaphore(
						pvt->_securityattrs[i],
						0,(2^31)-1,
						pvt->_semnames[i]);

				// failure...
				if (!sem) {
					// see if this semaphore already exists
					if (GetLastError()==
						ERROR_ALREADY_EXISTS) {
						error::setErrorNumber(EEXIST);
					}
					// FIXME: clean up previously created
					// semaphores...
					return false;
				}

				// success...
				pvt->_sems[i]=sem;
				return true;

			} else {

				// attach to existing semaphore
				HANDLE	sem=OpenSemaphore(
						// FIXME: set this for real...
						0,
						TRUE,
						pvt->_semnames[i]);

				// failure...
				if (!sem) {
					// FIXME: clean up previously created
					// semaphores...
					return false;
				}

				// success...
				pvt->_sems[i]=sem;
				return true;
			}
		}

		return false;

	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int32_t semaphoreset::semControl(semaphoresetprivate *pvt, int32_t semnum,
						int32_t cmd, semun *semctlun) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		int32_t	result;
		do {
			result=semctl(pvt->_semid,semnum,cmd,*semctlun);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);
		return result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return 1;
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

bool semaphoreset::semOp(struct sembuf *sops) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		int32_t	result;
		do {
			result=semop(pvt->_semid,sops,1);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return 1;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool semaphoreset::semTimedOp(struct sembuf *sops,
				int32_t seconds, int32_t nanoseconds) {
	#if defined(RUDIMENTS_HAVE_SEMTIMEDOP)
		int32_t	result;
		timespec	ts;
		ts.tv_sec=seconds;
		ts.tv_nsec=nanoseconds;
		do {
			result=semtimedop(pvt->_semid,sops,1,&ts);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return 1;
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

void semaphoreset::retryInterruptedOperations() {
	pvt->_retryinterruptedoperations=true;
}

void semaphoreset::dontRetryInterruptedOperations() {
	pvt->_retryinterruptedoperations=false;
}
