// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/userentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_CREATESEMAPHORE
	#include <rudiments/charstring.h>
	#include <rudiments/sys.h>
	#include <rudiments/bytestring.h>
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
		char	*_username;
		char	*_groupname;
		#if defined(RUDIMENTS_HAVE_SEMGET)
			struct	sembuf	**_waitop;
			struct	sembuf	**_waitwithundoop;
			struct	sembuf	**_signalop;
			struct	sembuf	**_signalwithundoop;
		#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
			HANDLE	*_sems;
			char	**_semnames;
			HMODULE	_lib;
		#endif
		bool	_supportstimedops;
};

semaphoreset::semaphoreset() {
	pvt=new semaphoresetprivate;
	pvt->_semid=-1;
	pvt->_created=false;
	pvt->_semcount=0;
	pvt->_retryinterruptedoperations=true;
	pvt->_username=NULL;
	pvt->_groupname=NULL;
	#if defined(RUDIMENTS_HAVE_SEMGET)
		pvt->_waitop=NULL;
		pvt->_waitwithundoop=NULL;
		pvt->_signalop=NULL;
		pvt->_signalwithundoop=NULL;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		pvt->_sems=NULL;
		pvt->_semnames=NULL;
		pvt->_lib=NULL;
	#endif
	#if defined(RUDIMENTS_HAVE_SEMTIMEDOP)
		int32_t	result;
		error::clearError();
		do {
			result=semtimedop(-1,NULL,0,NULL);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		pvt->_supportstimedops=(error::getErrorNumber()!=ENOSYS);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		pvt->_supportstimedops=true;
	#else
		pvt->_supportstimedops=false;
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
				delete[] pvt->_semnames[i];
			}
			delete[] pvt->_sems;
			delete[] pvt->_semnames;
		}
		if (pvt->_lib) {
			FreeLibrary(pvt->_lib);
		}
	#endif

	delete[] pvt->_username;
	delete[] pvt->_groupname;

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
		RUDIMENTS_SET_ENOSYS
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
		return (WaitForSingleObject(pvt->_sems[index],INFINITE)==
								WAIT_OBJECT_0);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::wait(int32_t index, int32_t seconds, int32_t nanoseconds) {
	if (seconds<0 || nanoseconds<0) {
		return wait(index);
	}
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semTimedOp(pvt->_waitop[index],seconds,nanoseconds);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		DWORD	result=WaitForSingleObject(pvt->_sems[index],
					seconds*1000+nanoseconds/1000000);
		if (result==WAIT_TIMEOUT) {
			error::setErrorNumber(EAGAIN);
		}
		return (result==WAIT_OBJECT_0);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::waitWithUndo(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_waitwithundoop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// no such thing as undo on windows
		return wait(index);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::waitWithUndo(int32_t index,
				int32_t seconds, int32_t nanoseconds) {
	if (seconds<0 || nanoseconds<0) {
		return waitWithUndo(index);
	}
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semTimedOp(pvt->_waitwithundoop[index],
						seconds,nanoseconds);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// no such thing as undo on windows
		return wait(index,seconds,nanoseconds);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::signal(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_signalop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return (ReleaseSemaphore(pvt->_sems[index],1,NULL)!=0);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::signalWithUndo(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		return semOp(pvt->_signalwithundoop[index]);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// no such thing as undo on windows
		return signal(index);
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

#if defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
struct SEMINFO {
	ULONG	count;
	ULONG	limit;
};
#endif

int32_t semaphoreset::getValue(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETVAL,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		uint32_t	retval=-1;
		if (!pvt->_lib) {
			pvt->_lib=LoadLibrary("NTDLL");
		}
		if (pvt->_lib) {
			LONG (NTAPI *proc)
				(HANDLE,ULONG,SEMINFO *,ULONG,ULONG *)=
			(LONG (NTAPI *)
				(HANDLE,ULONG,SEMINFO *,ULONG,ULONG *))
				GetProcAddress(pvt->_lib,"NtQuerySemaphore");
			if (proc) {
				SEMINFO	seminfo;
				LONG	status=
					proc(pvt->_sems[index],0,
						&seminfo,sizeof(seminfo),
						NULL);
				if (status==ERROR_SUCCESS) {
					retval=seminfo.count;
				}
			}
		}
		if (retval==-1) {
			RUDIMENTS_SET_ENOSYS
		}
		return retval;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

bool semaphoreset::setValue(int32_t index, int32_t value) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		semctlun.val=value;
		return !semControl(pvt,index,SETVAL,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		int32_t	current=getValue(index);
		if (current<value) {
			if (ReleaseSemaphore(pvt->_sems[index],
						value-current,NULL)==0) {
				return false;
			}
		} else if (current>value) {
			for (int32_t i=current; i<value; i++) {
				if (WaitForSingleObject(
						pvt->_sems[index],
						INFINITE)!=WAIT_OBJECT_0) {
					return false;
				}
			}
		}
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

int32_t semaphoreset::getWaitingForZero(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETZCNT,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		RUDIMENTS_SET_ENOSYS
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

int32_t semaphoreset::getWaitingForIncrement(int32_t index) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		semun	semctlun;
		return semControl(pvt,index,GETNCNT,&semctlun);
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		// FIXME: there's got to be a way to do this...
		RUDIMENTS_SET_ENOSYS
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::create(key_t key, mode_t permissions, 
				int32_t semcount, const int32_t *values) {

	pvt->_semcount=semcount;

	// create the semaphore
	if (semGet(key,semcount,IPC_CREAT|IPC_EXCL|permissions,values)!=-1) {

		// mark for removal
		pvt->_created=true;

		// create the signal/wait operations
		createOperations();

		return true;
	}
	return false;
}

bool semaphoreset::attach(key_t key, int32_t semcount) {

	pvt->_semcount=semcount;

	// attach to the semaphore
	if (semGet(key,semcount,0,NULL)!=-1) {

		// create the signal/wait operations
		createOperations();

		return true;
	}
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
		RUDIMENTS_SET_ENOSYS
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
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
		RUDIMENTS_SET_ENOSYS
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::setUserName(const char *username) {
	uid_t	userid=userentry::getUserId(username);
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
		RUDIMENTS_SET_ENOSYS
		return false;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

const char *semaphoreset::getUserName() {
	delete[] pvt->_username;
	pvt->_username=userentry::getName(getUserId());
	return pvt->_username;
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
		RUDIMENTS_SET_ENOSYS
		return (uid_t)-1;
	#else
		RUDIMENTS_SET_ENOSYS
		return (uid_t)-1;
	#endif
}

const char *semaphoreset::getGroupName() {
	delete[] pvt->_groupname;
	pvt->_groupname=groupentry::getName(getGroupId());
	return pvt->_groupname;
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
		RUDIMENTS_SET_ENOSYS
		return (gid_t)-1;
	#else
		RUDIMENTS_SET_ENOSYS
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
		RUDIMENTS_SET_ENOSYS
		return 0;
	#else
		RUDIMENTS_SET_ENOSYS
		return 0;
	#endif
}

int32_t semaphoreset::semGet(key_t key, int32_t nsems,
				int32_t semflg, const int32_t *values) {
	#if defined(RUDIMENTS_HAVE_SEMGET)

		int32_t	result;
		error::clearError();
		do {
			result=semget(key,nsems,semflg);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);

		pvt->_semid=result;
	
		// initialize the semaphores (if we're supposed to)
		if (values && result!=-1) {
			for (int32_t i=0; i<nsems; i++) {
				setValue(i,values[i]);
			}
		}
		return result;

	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)

		pvt->_sems=new HANDLE[nsems];
		pvt->_semnames=new char *[nsems];

		for (int32_t i=0; i<nsems; i++) {

			// set the semaphore name
			int32_t	semnamelen=11+charstring::integerLength(key)+1+
						charstring::integerLength(i)+1;
			pvt->_semnames[i]=new char[semnamelen];
			charstring::copy(pvt->_semnames[i],"rudiments::");
			charstring::append(pvt->_semnames[i],(int64_t)key);
			charstring::append(pvt->_semnames[i],"-");
			charstring::append(pvt->_semnames[i],(int64_t)i);

			if (semflg&(IPC_CREAT|IPC_EXCL)) {

				// create a new semaphore
				// FIXME: set the security attributes or the
				// semaphore can't be inherited by a child
				// process
				HANDLE	sem=CreateSemaphore(
						NULL,values[i],
						sys::getMaxSemaphoreValue(),
						pvt->_semnames[i]);

				// outright failure...
				if (!sem) {
					return false;
				}

				// see if this semaphore already exists
				if (GetLastError()==ERROR_ALREADY_EXISTS) {
					error::setErrorNumber(EEXIST);
					return false;
				}

				// success...
				pvt->_sems[i]=sem;

			} else {

				// attach to existing semaphore
				// SEMAPHORE_ALL_ACCESS is necessary rather than
				// just SYNCHRONIZE|SEMAPHORE_MODIFY_STATE if
				// we want to be able to use NTQuerySemaphore,
				// and for some reason they can't all just be
				// or'ed together
				HANDLE	sem=OpenSemaphore(
						SEMAPHORE_ALL_ACCESS,
						TRUE,pvt->_semnames[i]);

				// failure...
				if (!sem) {
					return false;
				}

				// success...
				pvt->_sems[i]=sem;
			}
		}

		return true;

	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

int32_t semaphoreset::semControl(semaphoresetprivate *pvt, int32_t semnum,
						int32_t cmd, semun *semctlun) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		int32_t	result;
		error::clearError();
		do {
			result=semctl(pvt->_semid,semnum,cmd,*semctlun);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);
		return result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return 1;
	#else
		RUDIMENTS_SET_ENOSYS
		return -1;
	#endif
}

bool semaphoreset::semOp(struct sembuf *sops) {
	#if defined(RUDIMENTS_HAVE_SEMGET)
		int32_t	result;
		error::clearError();
		do {
			result=semop(pvt->_semid,sops,1);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return 1;
	#else
		RUDIMENTS_SET_ENOSYS
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
		error::clearError();
		do {
			result=semtimedop(pvt->_semid,sops,1,&ts);
		} while (result==-1 &&
				error::getErrorNumber()==EINTR &&
				pvt->_retryinterruptedoperations);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATESEMAPHORE)
		return 1;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool semaphoreset::supportsTimedSemaphoreOperations() {
	return pvt->_supportstimedops;
}

bool semaphoreset::supportsUndoSemaphoreOperations() {
	#ifdef SEM_UNDO
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
