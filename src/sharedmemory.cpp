// Copyright (c) 1999-2002 David Muse
// See the COPYING file for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>
#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
	#include <rudiments/charstring.h>
#endif

#ifdef RUDIMENTS_HAVE_SHMGET
	#include <sys/ipc.h>
	#include <sys/shm.h>
#endif

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif

#if defined(RUDIMENTS_HAVE_SHMGET)

static int32_t shmGet(key_t key, size_t size, int32_t shmflag) {
	int32_t	result;
	do {
		result=shmget(key,size,shmflag);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

static void *shmAttach(int32_t id) {
	void	*result;
	do {
		result=shmat(id,0,0);
	} while (reinterpret_cast<int64_t>(result)==-1 &&
				error::getErrorNumber()==EINTR);
	return result;
}

static bool shmControl(int32_t id, int32_t cmd, shmid_ds *buf) {
	int32_t	result;
	do {
		result=shmctl(id,cmd,buf);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)

static char	*shmName(key_t key) {
	uint32_t	shmnamelen=11+charstring::integerLength(key)+1;
	char		*shmname=new char[shmnamelen];
	charstring::copy(shmname,"rudiments::");
	charstring::append(shmname,(int64_t)key);
	return shmname;
}

#endif

class sharedmemoryprivate {
	friend class sharedmemory;
	private:
		int32_t	_shmid;
		bool	_created;
		void	*_shmptr;
		char	*_username;
		char	*_groupname;
		#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
			HANDLE	_map;
		#endif
};

sharedmemory::sharedmemory() {
	pvt=new sharedmemoryprivate;
	pvt->_shmid=-1;
	pvt->_created=false;
	pvt->_shmptr=NULL;
	pvt->_username=NULL;
	pvt->_groupname=NULL;
	#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
		pvt->_map=NULL;
	#endif
}

sharedmemory::~sharedmemory() {
	if (pvt->_created) {
		forceRemove();
	}
	delete[] pvt->_username;
	delete[] pvt->_groupname;
	delete pvt;
}

bool sharedmemory::forceRemove() {
	#if defined(RUDIMENTS_HAVE_SHMGET)
		return shmControl(pvt->_shmid,IPC_RMID,NULL);
	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
		return (UnmapViewOfFile(pvt->_shmptr)==TRUE &&
					CloseHandle(pvt->_map)==TRUE);
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
		return shmControl(pvt->_shmid,IPC_SET,&setds);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sharedmemory::setGroupId(gid_t gid) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	setds;
		setds.shm_perm.gid=gid;
		return shmControl(pvt->_shmid,IPC_SET,&setds);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sharedmemory::setPermissions(mode_t permissions) {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	setds;
		setds.shm_perm.mode=permissions;
		return shmControl(pvt->_shmid,IPC_SET,&setds);
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

uid_t sharedmemory::getUserId() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		if (shmControl(pvt->_shmid,IPC_STAT,&getds)) {
			return getds.shm_perm.uid;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return (uid_t)-1;
}

gid_t sharedmemory::getGroupId() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		if (shmControl(pvt->_shmid,IPC_STAT,&getds)) {
			return getds.shm_perm.gid;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return (gid_t)-1;
}

mode_t sharedmemory::getPermissions() {
	#ifdef RUDIMENTS_HAVE_SHMGET
		shmid_ds	getds;
		if (shmControl(pvt->_shmid,IPC_STAT,&getds)) {
			return getds.shm_perm.mode;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return 0;
}

bool sharedmemory::create(key_t key, size_t size, mode_t permissions) {

	#if defined(RUDIMENTS_HAVE_SHMGET)

		// create the shared memory segment
		pvt->_shmid=shmget(key,size,IPC_CREAT|IPC_EXCL|permissions);
		if (pvt->_shmid==-1) {
			return false;
		}

		// mark for removal
		pvt->_created=true;

		// attach to the segment
		pvt->_shmptr=shmAttach(pvt->_shmid);
		if (reinterpret_cast<int64_t>(pvt->_shmptr)==-1) {
			forceRemove();
			return false;
		}

	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)

		// On Windows, a shared memory segment is created by
		// memory-mapping the page file and naming the mapping.

		// get the map name
		char	*shmname=shmName(key);

		// calculate max mapping size
		DWORD	maxsizehigh=(((uint64_t)size)>>32);
		DWORD	maxsizelow=(((uint64_t)size)&0x0000FFFF);

		// create a named file mapping
		// (INVALID_HANDLE_VALUE means to map the page file)
		pvt->_map=CreateFileMapping(INVALID_HANDLE_VALUE,
						NULL,PAGE_READWRITE,
						maxsizehigh,maxsizelow,
						shmname);
		delete[] shmname;
		if (!pvt->_map) {
			return false;
		}

		// mark for removal
		pvt->_created=true;

		// create a view of the file mapping
		pvt->_shmptr=MapViewOfFile(pvt->_map,
						FILE_MAP_ALL_ACCESS,
						0,0,size);
		if (!pvt->_shmptr) {
			CloseHandle(pvt->_map);
			return false;
		}
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif

	// init the segment to zero's
	rawbuffer::zero(pvt->_shmptr,size);
	return true;
}

bool sharedmemory::attach(key_t key, size_t size) {

	#if defined(RUDIMENTS_HAVE_SHMGET)
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
				reinterpret_cast<int64_t>(pvt->_shmptr=
						shmAttach(pvt->_shmid))!=-1);

	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)

		// get the map name
		char	*shmname=shmName(key);

		// attach to the named file mapping
		pvt->_map=OpenFileMapping(FILE_MAP_ALL_ACCESS,FALSE,shmname);
		delete[] shmname;
		if (!pvt->_map) {
			return false;
		}

		// create a view of the file mapping
		pvt->_shmptr=MapViewOfFile(pvt->_map,
						FILE_MAP_ALL_ACCESS,
						0,0,size);
		if (!pvt->_shmptr) {
			CloseHandle(pvt->_map);
			return false;
		}
		return true;
	#else
		error::setErrorNumber(ENOSYS);
		return false;
	#endif
}

bool sharedmemory::createOrAttach(key_t key, size_t size, mode_t permissions) {

	#if defined(RUDIMENTS_HAVE_SHMGET)
		// create the shared memory segment
		if ((pvt->_shmid=shmGet(key,size,
				IPC_CREAT|IPC_EXCL|permissions))!=-1) {

			// mark for removal
			pvt->_created=true;

			// attach to the segment, remove the
			// segment and return 0 on failure
			pvt->_shmptr=shmAttach(pvt->_shmid);
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
			pvt->_shmptr=shmAttach(pvt->_shmid);
			return (reinterpret_cast<int64_t>(pvt->_shmptr)!=-1);
		}

	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)

		// get the map name
		char	*shmname=shmName(key);

		// calculate max mapping size
		DWORD	maxsizehigh=(((uint64_t)size)>>32);
		DWORD	maxsizelow=(((uint64_t)size)&0x0000FFFF);

		// attempt to create a named file mapping
		// (INVALID_HANDLE_VALUE means to map the page file)
		pvt->_map=CreateFileMapping(INVALID_HANDLE_VALUE,
						NULL,PAGE_READWRITE,
						maxsizehigh,maxsizelow,
						shmname);
		delete[] shmname;
		if (pvt->_map) {

			// mark for removal
			pvt->_created=true;

		} else {

			// if that failed, then attempt to
			// attach to the named file mapping
			pvt->_map=OpenFileMapping(FILE_MAP_ALL_ACCESS,
								FALSE,NULL);
			if (!pvt->_map) {
				return false;
			}
		}

		// create a view of the file mapping
		pvt->_shmptr=MapViewOfFile(pvt->_map,
						FILE_MAP_ALL_ACCESS,
						0,0,size);
		if (!pvt->_shmptr) {
			CloseHandle(pvt->_map);
			return false;
		}

		if (pvt->_created) {
			// init the segment to zero's
			rawbuffer::zero(pvt->_shmptr,size);
			return true;
		}
	#else
		error::setErrorNumber(ENOSYS);
	#endif
	return false;
}

const char *sharedmemory::getUserName() {
	delete[] pvt->_username;
	pvt->_username=passwdentry::getName(getUserId());
	return pvt->_username;
}

const char *sharedmemory::getGroupName() {
	delete[] pvt->_groupname;
	pvt->_groupname=groupentry::getName(getGroupId());
	return pvt->_groupname;
}

bool sharedmemory::setUserName(const char *username) {
	uid_t	userid=passwdentry::getUserId(username);
	return (userid!=(uid_t)-1 && setUserId(userid));
}

bool sharedmemory::setGroupName(const char *groupname) {
	gid_t	groupid=groupentry::getGroupId(groupname);
	return (groupid!=(gid_t)-1 && setGroupId(groupid));
}
