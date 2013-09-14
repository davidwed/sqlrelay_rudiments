// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

class sharedmemoryprivate {
	friend class sharedmemory;
	private:
		int32_t	_shmid;
		bool	_created;
		void	*_shmptr;
};

sharedmemory::sharedmemory() {
	pvt=new sharedmemoryprivate;
	pvt->_shmid=-1;
	pvt->_created=false;
	pvt->_shmptr=NULL;
}

sharedmemory::~sharedmemory() {
	if (pvt->_created) {
		forceRemove();
	}
	delete pvt;
}

bool sharedmemory::forceRemove() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		return shmControl(IPC_RMID,NULL);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

void sharedmemory::dontRemove() {
	pvt->_created=false;
}

int32_t sharedmemory::getId() const {
	return pvt->_shmid;
}

void *sharedmemory::getPointer() const {
	return pvt->_shmptr;
}

bool sharedmemory::setUserId(uid_t uid) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	setds;
		setds.shm_perm.uid=uid;
		return shmControl(IPC_SET,&setds);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sharedmemory::setGroupId(gid_t gid) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	setds;
		setds.shm_perm.gid=gid;
		return shmControl(IPC_SET,&setds);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sharedmemory::setPermissions(mode_t permissions) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	setds;
		setds.shm_perm.mode=permissions;
		return shmControl(IPC_SET,&setds);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

uid_t sharedmemory::getUserId() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		if (shmControl(IPC_STAT,&getds)) {
			return getds.shm_perm.uid;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return 0;
}

gid_t sharedmemory::getGroupId() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		if (shmControl(IPC_STAT,&getds)) {
			return getds.shm_perm.gid;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return 0;
}

mode_t sharedmemory::getPermissions() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		if (shmControl(IPC_STAT,&getds)) {
			return getds.shm_perm.mode;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return 0;
}

bool sharedmemory::create(key_t key, size_t size, mode_t permissions) {

	#ifdef RUDIMENTS_HAVE_SHMGET
		// create the shared memory segment
		if ((pvt->_shmid=shmget(key,size,
				IPC_CREAT|IPC_EXCL|permissions))!=-1) {

			// mark for removal
			pvt->_created=true;

			// attach to the segment, remove the
			// segment and return 0 on failure
			pvt->_shmptr=shmAttach();
			if (reinterpret_cast<int64_t>(pvt->_shmptr)==-1) {
				forceRemove();
				return false;
			}

			// init the segment to zero's
			rawbuffer::zero(pvt->_shmptr,size);
			return true;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return false;
}

bool sharedmemory::attach(key_t key) {

	#ifdef RUDIMENTS_HAVE_SHMGET
		// shmat's documentation says something like...
		// RETURN VALUE
		//	On failure shmat returns -1 with errno
		//		indicating the error.
		//	On success shmat returns the address of the
		//		attached shared memory segment.
		// So you might think, ok shmat()>-1 is success.  Ahh, but wait,
		// shmat()returns a (void *) which doesn't really have an well
		// defined size.  So, it's possible for a memory address to be
		// much bigger than the upper bound of whatever really big,
		// signed integer your system might have and thus appear to be
		// a negative number when cast to that type.  This appears to
		// happen on Solaris x86 all the time.  Also, compilers don't
		// like it when you compare (void *)'s to numbers, so you have
		// to cast it.  And you can't just cast it to an unsigned
		// integer because it needs to be compared to -1.  So, we cast
		// it to a signed integer to see if it's not -1, but allow that
		// it could very well be less than -1 and still be valid.
		return ((pvt->_shmid=shmGet(key,0,0))!=-1 &&
				reinterpret_cast<int64_t>(
					pvt->_shmptr=shmAttach())!=-1);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sharedmemory::createOrAttach(key_t key, size_t size, mode_t permissions) {

	#ifdef RUDIMENTS_HAVE_SHMGET
		// create the shared memory segment
		if ((pvt->_shmid=shmGet(key,size,
				IPC_CREAT|IPC_EXCL|permissions))!=-1) {

			// mark for removal
			pvt->_created=true;

			// attach to the segment, remove the
			// segment and return 0 on failure
			pvt->_shmptr=shmAttach();
			if (reinterpret_cast<int64_t>(pvt->_shmptr)==-1) {
				forceRemove();
				return false;
			}

			// init the segment to zero's
			rawbuffer::zero(pvt->_shmptr,size);
			return true;
		
		} else if (error::getErrorNumber()==EEXIST &&
				(pvt->_shmid=shmGet(key,0,permissions))!=-1) {

			// attach to the segment,
			// return 1 on success and 0 on failure
			pvt->_shmptr=shmAttach();
			return (reinterpret_cast<int64_t>(pvt->_shmptr)!=-1);

		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return false;
}

const char *sharedmemory::getUserName() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		char		*name;
		if (shmControl(IPC_STAT,&getds) &&
			passwdentry::getName(getds.shm_perm.uid,&name)) {
			return name;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return NULL;
}

const char *sharedmemory::getGroupName() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		char		*name;
		if (shmControl(IPC_STAT,&getds) &&
			groupentry::getName(getds.shm_perm.gid,&name)) {
			return name;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return NULL;
}

bool sharedmemory::setUserName(const char *username) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		uid_t	userid;
		return (passwdentry::getUserId(username,&userid) &&
							setUserId(userid));
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sharedmemory::setGroupName(const char *groupname) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		gid_t	groupid;
		return (groupentry::getGroupId(groupname,&groupid) &&
							setGroupId(groupid));
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

int32_t sharedmemory::shmGet(key_t key, size_t size, int32_t shmflag) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		int32_t	result;
		do {
			result=shmget(key,size,shmflag);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		error::setErrorNumber(ENOSYS);
		return -1;
	#endif
}

void *sharedmemory::shmAttach() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		void	*result;
		do {
			result=shmat(pvt->_shmid,0,0);
		} while (reinterpret_cast<int64_t>(result)==-1 &&
				error::getErrorNumber()==EINTR);
		return result;
	#else
		error::setErrorNumber(ENOSYS);
		return NULL;
	#endif
}

bool sharedmemory::shmControl(int32_t cmd, shmid_ds *buf) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		int32_t	result;
		do {
			result=shmctl(pvt->_shmid,cmd,buf);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}
