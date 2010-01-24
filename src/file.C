// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/file.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>
#ifndef RUDIMENTS_HAVE_UTIMES
	#include <rudiments/datetime.h>
#endif
#ifndef RUDIMENTS_HAVE_MKSTEMP
	#include <rudiments/datetime.h>
	#include <rudiments/randomnumber.h>
	#include <rudiments/permissions.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#ifdef RUDIMENTS_HAVE_SYS_IPC_H
	#include <sys/ipc.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#include <sys/time.h>
#ifdef RUDIMENTS_HAVE_XATTRS
	#include <sys/xattr.h>
#endif

#ifdef MINGW32
	#include <windows.h>
	// windows doesn't define these, but we need them
	// internally to this file
	#ifndef F_GETLK	
		#define F_GETLK		0
	#endif
	#ifndef F_SETLK	
		#define F_SETLK		1
	#endif
	#ifndef F_SETLKW	
		#define F_SETLKW	2
	#endif
	#ifndef _PC_LINK_MAX
		#define _PC_LINK_MAX		0
	#endif
	#ifndef _PC_CHOWN_RESTRICTED
		#define _PC_CHOWN_RESTRICTED	1
	#endif
#endif

#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif
	
#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class fileprivate {
	friend class file;
	private:
		struct	stat	_st;
		bool		_getcurrentpropertiesonopen;
};

file::file() : filedescriptor() {
	pvt=new fileprivate;
	rawbuffer::zero(&pvt->_st,sizeof(pvt->_st));
	pvt->_getcurrentpropertiesonopen=true;
	type("file");
}

file::file(const file &f) : filedescriptor(f) {
	pvt=new fileprivate;
	fileClone(f);
	type("file");
}

file &file::operator=(const file &f) {
	if (this!=&f) {
		filedescriptor::operator=(f);
		fileClone(f);
	}
	return *this;
}

void file::fileClone(const file &f) {
	pvt->_st=f.pvt->_st;
	pvt->_getcurrentpropertiesonopen=f.pvt->_getcurrentpropertiesonopen;
}

file::~file() {
	delete pvt;
}

bool file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

ssize_t file::create(const char *name, mode_t perms, unsigned short number) {
	return create(name,perms,&number,sizeof(unsigned short));
}

ssize_t file::create(const char *name, mode_t perms, unsigned long number) {
	return create(name,perms,&number,sizeof(unsigned long));
}

ssize_t file::create(const char *name, mode_t perms, float number) {
	return create(name,perms,&number,sizeof(float));
}

ssize_t file::create(const char *name, mode_t perms, double number) {
	return create(name,perms,&number,sizeof(double));
}

ssize_t file::create(const char *name, mode_t perms, char number) {
	return create(name,perms,&number,sizeof(char));
}

ssize_t file::create(const char *name, mode_t perms,
					const char *string, size_t size) {
	return create(name,perms,static_cast<const void *>(string),size);
}

ssize_t file::create(const char *name, mode_t perms, const char *string) {
	return create(name,perms,static_cast<const void *>(string),
						charstring::length(string));
}

ssize_t file::create(const char *name, mode_t perms,
					const void *data, size_t size) {
	size_t	retval;
	fd(openInternal(name,O_CREAT|O_TRUNC|O_RDWR,perms));
	if (fd()!=-1 &&
		((retval=write(data,size))==size) &&
		((pvt->_getcurrentpropertiesonopen)?
				getCurrentProperties():true)) {
		return retval;
	}
	close();
	return -1;
}

bool file::createFile(const char *name, mode_t perms) {
	file	fl;
	return fl.create(name,perms);
}

ssize_t file::createFile(const char *name, mode_t perms,
						unsigned short number) {
	file	fl;
	return fl.create(name,perms,number);
}

ssize_t file::createFile(const char *name, mode_t perms, unsigned long number) {
	file	fl;
	return fl.create(name,perms,number);
}

ssize_t file::createFile(const char *name, mode_t perms, float number) {
	file	fl;
	return fl.create(name,perms,number);
}

ssize_t file::createFile(const char *name, mode_t perms, double number) {
	file	fl;
	return fl.create(name,perms,number);
}

ssize_t file::createFile(const char *name, mode_t perms, char number) {
	file	fl;
	return fl.create(name,perms,number);
}

ssize_t file::createFile(const char *name, mode_t perms,
					const char *string, size_t size) {
	file	fl;
	return fl.create(name,perms,string,size);
}

ssize_t file::createFile(const char *name, mode_t perms, const char *string) {
	file	fl;
	return fl.create(name,perms,string);
}

ssize_t file::createFile(const char *name, mode_t perms,
					const void *data, size_t size) {
	file	fl;
	return fl.create(name,perms,data,size);
}

int file::openInternal(const char *name, int flags) {
	int	result;
	do {
		result=::open(name,flags);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

int file::openInternal(const char *name, int flags, mode_t perms) {
	int	result;
	do {
		result=::open(name,flags,perms);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool file::open(const char *name, int flags) {
	fd(openInternal(name,flags));
	return (fd()!=-1 &&
		((pvt->_getcurrentpropertiesonopen)?
				getCurrentProperties():true));
}

bool file::open(const char *name, int flags, mode_t perms) {
	fd(openInternal(name,flags,perms));
	return (fd()!=-1 &&
		((pvt->_getcurrentpropertiesonopen)?
				getCurrentProperties():true));
}

char *file::getContents() {
	off64_t	size=(fd()>-1)?pvt->_st.st_size:0;
	char	*contents=new char[size+1];
	contents[size]='\0';
	return (size==0 || read(contents,size)==size)?contents:NULL;
}

char *file::getContents(const char *name) {
	file	fl;
	fl.open(name,O_RDONLY);
	char	*contents=fl.getContents();
	fl.close();
	return contents;
}

ssize_t file::getContents(const char *name, unsigned char *buffer,
						size_t buffersize) {
	file	fl;
	fl.open(name,O_RDONLY);
	ssize_t	bytes=fl.getContents(buffer,buffersize);
	fl.close();
	return bytes;
}

ssize_t file::getContents(unsigned char *buffer, size_t buffersize) {
	return read(buffer,(buffersize<(size_t)getSize())?buffersize:getSize());
}


bool file::tryLockFile(short type) const {
	return tryLockRegion(type,0,0);
}

bool file::lockFile(short type) const {
	return lockRegion(type,0,0);
}

bool file::checkLockFile(short type, struct flock *retlck) const {
	return checkLockRegion(type,0,0,retlck);
}

bool file::unlockFile() const {
	return unlockRegion(0,0);
}

bool file::tryLockRegion(short type, off64_t start, off64_t len) const {
	return lock(F_SETLK,type,SEEK_SET,start,len);
}

bool file::lockRegion(short type, off64_t start, off64_t len) const {
	return lock(F_SETLKW,type,SEEK_SET,start,len);
}

bool file::checkLockRegion(short type, off64_t start, off64_t len,
						struct flock *retlck) const {
	return checkLock(type,SEEK_SET,start,len,retlck);
}

bool file::unlockRegion(off64_t start, off64_t len) const {
	return unlock(SEEK_SET,start,len);
}

bool file::tryLockFromCurrent(short type, off64_t len) const {
	return tryLockFromCurrent(type,0,len);
}

bool file::tryLockFromCurrent(short type, off64_t start, off64_t len) const {
	return lock(F_SETLK,type,SEEK_CUR,start,len);
}

bool file::lockFromCurrent(short type, off64_t len) const {
	return lockFromCurrent(type,0,len);
}

bool file::lockFromCurrent(short type, off64_t start, off64_t len) const {
	return lock(F_SETLKW,type,SEEK_CUR,start,len);
}

bool file::checkLockFromCurrent(short type, off64_t len,
				struct flock *retlck) const {
	return checkLockFromCurrent(type,0,len,retlck);
}

bool file::checkLockFromCurrent(short type, off64_t start, off64_t len,
						struct flock *retlck) const {
	return checkLock(type,SEEK_CUR,start,len,retlck);
}

bool file::unlockFromCurrent(off64_t len) const {
	return unlockFromCurrent(0,len);
}

bool file::unlockFromCurrent(off64_t start, off64_t len) const {
	return unlock(SEEK_CUR,start,len);
}

bool file::tryLockFromEnd(short type, off64_t len) const {
	return tryLockFromEnd(type,0,len);
}

bool file::tryLockFromEnd(short type, off64_t start, off64_t len) const {
	return lock(F_SETLK,type,SEEK_END,start,len);
}

bool file::lockFromEnd(short type, off64_t len) const {
	return lockFromEnd(type,0,len);
}

bool file::lockFromEnd(short type, off64_t start, off64_t len) const {
	return lock(F_SETLKW,type,SEEK_END,start,len);
}

bool file::checkLockFromEnd(short type, off64_t len, struct flock *retlck) const {
	return checkLockFromEnd(type,0,len,retlck);
}

bool file::checkLockFromEnd(short type, off64_t start, off64_t len,
						struct flock *retlck) const {
	return checkLock(type,SEEK_END,start,len,retlck);
}

bool file::unlockFromEnd(off64_t len) const {
	return unlockFromEnd(0,len);
}

bool file::unlockFromEnd(off64_t start, off64_t len) const {
	return unlock(SEEK_END,start,len);
}

bool file::tryLockRemainder(short type, off64_t start) const {
	return lock(F_SETLK,type,SEEK_SET,start,0);
}

bool file::lockRemainder(short type, off64_t start) const {
	return lock(F_SETLKW,type,SEEK_SET,start,0);
}

bool file::checkLockRemainder(short type, off64_t start,
					struct flock *retlck) const {
	return checkLock(type,SEEK_SET,start,0,retlck);
}

bool file::unlockRemainder(off64_t start) const {
	return unlock(SEEK_SET,start,0);
}

bool file::tryLockRemainderFromCurrent(short type) const {
	return tryLockRemainderFromCurrent(type,0);
}

bool file::tryLockRemainderFromCurrent(short type, off64_t start) const {
	return lock(F_SETLK,type,SEEK_CUR,start,0);
}

bool file::lockRemainderFromCurrent(short type) const {
	return lockRemainderFromCurrent(type,0);
}

bool file::lockRemainderFromCurrent(short type, off64_t start) const {
	return lock(F_SETLKW,type,SEEK_CUR,start,0);
}

bool file::checkLockRemainderFromCurrent(short type,
					struct flock *retlck) const {
	return checkLockRemainderFromCurrent(type,0,retlck);
}

bool file::checkLockRemainderFromCurrent(short type, off64_t start,
					struct flock *retlck) const {
	return checkLock(type,SEEK_CUR,start,0,retlck);
}

bool file::unlockRemainderFromCurrent() const {
	return unlockRemainderFromCurrent(0);
}

bool file::unlockRemainderFromCurrent(off64_t start) const {
	return unlock(SEEK_CUR,start,0);
}

bool file::tryLockRemainderFromEnd(short type) const {
	return tryLockRemainderFromEnd(type,0);
}

bool file::tryLockRemainderFromEnd(short type, off64_t start) const {
	return lock(F_SETLK,type,SEEK_END,start,0);
}

bool file::lockRemainderFromEnd(short type) const {
	return lockRemainderFromEnd(type,0);
}

bool file::lockRemainderFromEnd(short type, off64_t start) const {
	return lock(F_SETLKW,type,SEEK_END,start,0);
}

bool file::checkLockRemainderFromEnd(short type, struct flock *retlck) const {
	return checkLockRemainderFromEnd(type,0,retlck);
}

bool file::checkLockRemainderFromEnd(short type, off64_t start,
						struct flock *retlck) const {
	return checkLock(type,SEEK_END,start,0,retlck);
}

bool file::sequentialAccess(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_SEQUENTIAL)
	return posixFadvise(start,len,POSIX_FADV_SEQUENTIAL);
	#else
	return true;
	#endif
}

bool file::randomAccess(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_RANDOM)
	return posixFadvise(start,len,POSIX_FADV_RANDOM);
	#else
	return true;
	#endif
}

bool file::onlyOnce(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_NOREUSE)
	return posixFadvise(start,len,POSIX_FADV_NOREUSE);
	#else
	return true;
	#endif
}

bool file::willNeed(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_WILLNEED)
	return posixFadvise(start,len,POSIX_FADV_WILLNEED);
	#else
	return true;
	#endif
}

bool file::wontNeed(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_DONTNEED)
	return posixFadvise(start,len,POSIX_FADV_DONTNEED);
	#else
	return true;
	#endif
}

bool file::normalAccess(off64_t start, size_t len) const {
	#if defined(RUDIMENTS_HAVE_POSIX_FADVISE) && \
			defined(POSIX_FADV_NORMAL)
	return posixFadvise(start,len,POSIX_FADV_NORMAL);
	#else
	return true;
	#endif
}

bool file::reserve(off64_t start, size_t len) const {
	#ifdef RUDIMENTS_HAVE_POSIX_FALLOCATE
	int	result;
	do {
		result=posix_fallocate(fd(),start,len);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
	#else
	return false;
	#endif
}

bool file::truncate(const char *filename) {
	return truncate(filename,0);
}

bool file::truncate(const char *filename, off64_t length) {
	#ifdef RUDIMENTS_HAVE_TRUNCATE
		int	result;
		do {
			result=::truncate(filename,length);
		} while (result==-1 && error::getErrorNumber()==-1);
		return !result;
	#else
		file	f;
		return f.open(filename,O_WRONLY) && f.truncate(length);
	#endif
}

bool file::truncate() const {
	return ftruncate(fd(),0);
}

bool file::truncate(off64_t length) const {
	int	result;
	do {
		result=::ftruncate(fd(),length);
	} while (result==-1 && error::getErrorNumber()==-1);
	return !result;
}

bool file::unlockRemainderFromEnd() const {
	return unlockRemainderFromEnd(0);
}

bool file::unlockRemainderFromEnd(off64_t start) const {
	return unlock(SEEK_END,start,0);
}

off64_t file::setPositionRelativeToBeginning(off64_t offset) const {
	return lseek(offset,SEEK_SET);
}

off64_t file::setPositionRelativeToCurrent(off64_t offset) const {
	return lseek(offset,SEEK_CUR);
}

off64_t file::setPositionRelativeToEnd(off64_t offset) const {
	return lseek(offset,SEEK_END);
}

off64_t file::getCurrentPosition() const {
	return lseek(0,SEEK_CUR);
}

off64_t file::lseek(off64_t offset, int whence) const {
	int	result;
	do {
		result=::lseek(fd(),offset,whence);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return result;
}

bool file::exists(const char *filename) {
	return accessible(filename,F_OK);
}

bool file::readable(const char *filename) {
	return accessible(filename,R_OK);
}

bool file::writeable(const char *filename) {
	return accessible(filename,W_OK);
}

bool file::executable(const char *filename) {
	return accessible(filename,X_OK);
}

bool file::accessible(const char *filename, int mode) {
	int	result;
	do {
		result=access(filename,mode);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool file::getCurrentProperties() {
	int	result;
	do {
		result=fstat(fd(),&pvt->_st);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

#define STAT(filename,out,member) \
	struct stat st; \
	int	result; \
	do { \
		result=stat(filename,&st); \
	} while (result==-1 && error::getErrorNumber()==EINTR); \
	if (result==-1) { \
		return false; \
	} \
	*out=st.member; \
	return true;

bool file::getPermissions(const char *filename, mode_t *mode) {
	STAT(filename,mode,st_mode)
}

bool file::getOwnerUserId(const char *filename, uid_t *uid) {
	STAT(filename,uid,st_uid)
}

bool file::getOwnerGroupId(const char *filename, gid_t *gid) {
	STAT(filename,gid,st_gid)
}

bool file::getSize(const char *filename, off64_t *size) {
	STAT(filename,size,st_size)
}

bool file::getBlockSize(const char *filename, blksize_t *size) {
#ifdef RUDIMENTS_HAVE_BLKSIZE_T
	STAT(filename,size,st_blksize)
#else
	*size=-1;
	return true;
#endif
}

bool file::getBlockCount(const char *filename, blkcnt_t *blocks) {
#ifdef RUDIMENTS_HAVE_BLKCNT_T
	STAT(filename,blocks,st_blocks)
#else
	*blocks=-1;
	return true;
#endif
}

#ifndef RUDIMENTS_HAVE_S_ISSOCK
	#define S_ISSOCK(m) ((m&0140000==0140000)?1:0)
#endif

int file::isSocket(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

#ifndef RUDIMENTS_HAVE_S_ISLNK
	#define S_ISLNK(m) ((m&0120000==0120000)?1:0)
#endif

int file::isSymbolicLink(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

int file::isRegularFile(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISREG(st.st_mode):-1;
}

int file::isBlockDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

int file::isDirectory(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

int file::isCharacterDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

int file::isFifo(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}

bool file::getLastAccessTime(const char *filename, time_t *atime) {
	STAT(filename,atime,st_atime)
}

bool file::getLastModificationTime(const char *filename, time_t *mtime) {
	STAT(filename,mtime,st_mtime)
}

bool file::getLastChangeTime(const char *filename, time_t *ctime) {
	STAT(filename,ctime,st_ctime)
}

bool file::getDevice(const char *filename, dev_t *dev) {
	STAT(filename,dev,st_dev)
}

bool file::getDeviceType(const char *filename, dev_t *devtype) {
	STAT(filename,devtype,st_rdev)
}

bool file::getInode(const char *filename, ino_t *inode) {
	STAT(filename,inode,st_ino)
}

bool file::getNumberOfHardLinks(const char *filename, nlink_t *nlink) {
	STAT(filename,nlink,st_nlink)
}

mode_t file::getPermissions() const {
	return pvt->_st.st_mode;
}

uid_t file::getOwnerUserId() const {
	return pvt->_st.st_uid;
}

gid_t file::getOwnerGroupId() const {
	return pvt->_st.st_gid;
}

off64_t file::getSize() const {
	return pvt->_st.st_size;
}


blksize_t file::getBlockSize() const {
#ifdef RUDIMENTS_HAVE_BLKSIZE_T
	return pvt->_st.st_blksize;
#else
	return -1;
#endif
}

blkcnt_t file::getBlockCount() const {
#ifdef RUDIMENTS_HAVE_BLKCNT_T
	return pvt->_st.st_blocks;
#else
	return -1;
#endif
}

int file::isSocket() const {
	return S_ISSOCK(pvt->_st.st_mode);
}

int file::isSymbolicLink() const {
	return S_ISLNK(pvt->_st.st_mode);
}

int file::isRegularFile() const {
	return S_ISREG(pvt->_st.st_mode);
}

int file::isBlockDevice() const {
	return S_ISBLK(pvt->_st.st_mode);
}

int file::isDirectory() const {
	return S_ISDIR(pvt->_st.st_mode);
}

int file::isCharacterDevice() const {
	return S_ISCHR(pvt->_st.st_mode);
}

int file::isFifo() const {
	return S_ISFIFO(pvt->_st.st_mode);
}

time_t file::getLastAccessTime() const {
	return pvt->_st.st_atime;
}

time_t file::getLastModificationTime() const {
	return pvt->_st.st_mtime;
}

time_t file::getLastChangeTime() const {
	return pvt->_st.st_ctime;
}


dev_t file::getDevice() const {
	return pvt->_st.st_dev;
}

dev_t file::getDeviceType() const {
	return pvt->_st.st_rdev;
}

ino_t file::getInode() const {
	return pvt->_st.st_ino;
}

nlink_t file::getNumberOfHardLinks() const {
	return pvt->_st.st_nlink;
}

void file::getCurrentPropertiesOnOpen() {
	pvt->_getcurrentpropertiesonopen=true;
}

void file::dontGetCurrentPropertiesOnOpen() {
	pvt->_getcurrentpropertiesonopen=false;
}

bool file::lock(int method, short type, short whence,
				off64_t start, off64_t len) const {
	#if defined(RUDIMENTS_HAVE_FCNTL)
		struct flock	lck;
		lck.l_type=type;
		lck.l_whence=whence;
		lck.l_start=start;
		lck.l_len=len;
		return !fcntl(method,reinterpret_cast<long>(&lck));
	#elif defined(RUDIMENTS_HAVE_LOCKFILEEX)
		// how do I specify read vs. write/write?
		off64_t	cur=getCurrentPosition();
		if (cur==-1) {
			return false;
		}
		LARGE_INTEGER	lockstart;
		lockstart.QuadPart=lseek(start,whence);
		if (lockstart.QuadPart==-1) {
			return false;
		}
		if (setPositionRelativeToBeginning(cur)==-1) {
			return false;
		}
		OVERLAPPED	ol;
		rawbuffer::zero((void *)&ol,sizeof(ol));
		ol.Offset=lockstart.LowPart;
		ol.OffsetHigh=lockstart.HighPart;
		LARGE_INTEGER	locklength;
		locklength.QuadPart=len;
		return LockFileEx((HANDLE)_get_osfhandle(fd()),
					(method==F_SETLK)?
						LOCKFILE_FAIL_IMMEDIATELY:0,
					0,
					locklength.LowPart,
					locklength.HighPart,
					&ol);
	#else
		#error no fcntl, LockFile or anything like it
	#endif
}

bool file::checkLock(short type, short whence, off64_t start, off64_t len,
						struct flock *retlck) const {
	#if defined(RUDIMENTS_HAVE_FCNTL)
		struct flock	lck;
		lck.l_type=type;
		lck.l_whence=whence;
		lck.l_start=start;
		lck.l_len=len;
		int	result=fcntl(F_GETLK,reinterpret_cast<long>(&lck));
		*retlck=lck;
		return !result;
	#elif defined(RUDIMENTS_HAVE_LOCKFILEEX)
		// Windows doesn't appear to support this at all.
		// I guess we'll return false, meaning not locked.
		return false;
	#else
		#error no fcntl(F_GETLK), LockFile or anything like it
	#endif
}

bool file::unlock(short whence, off64_t start, off64_t len) const {
	#if defined(RUDIMENTS_HAVE_FCNTL)
		struct flock	lck;
		lck.l_type=F_UNLCK;
		lck.l_whence=whence;
		lck.l_start=start;
		lck.l_len=len;
		return !fcntl(F_SETLK,reinterpret_cast<long>(&lck));
	#elif defined(RUDIMENTS_HAVE_LOCKFILEEX)
		off64_t	cur=getCurrentPosition();
		if (cur==-1) {
			return false;
		}
		LARGE_INTEGER	lockstart;
		lockstart.QuadPart=lseek(start,whence);
		if (lockstart.QuadPart==-1) {
			return false;
		}
		if (setPositionRelativeToBeginning(cur)==-1) {
			return false;
		}
		LARGE_INTEGER	locklength;
		locklength.QuadPart=len;
		return UnlockFile((HANDLE)_get_osfhandle(fd()),
					lockstart.LowPart,
					lockstart.HighPart,
					locklength.LowPart,
					locklength.HighPart);
	#else
		#error no fcntl, UnlockFile or anything like it
	#endif
}

bool file::changeOwner(const char *newuser, const char *newgroup) const {
	uid_t	uid;
	gid_t	gid;
	return (passwdentry::getUserId(newuser,&uid) &&
		groupentry::getGroupId(newgroup,&gid) &&
		changeOwner(uid,gid));
}

bool file::changeOwner(uid_t uid, gid_t gid) const {
	#if defined(RUDIMENTS_HAVE_FCHOWN)
		int	result;
		do {
			result=fchown(fd(),uid,gid);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(MINGW32)
		// windows doesn't support anything like this
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		// other platforms should support something like this
		#error no fchown or anything like it
	#endif
}

bool file::changeOwner(const char *filename, const char *newuser,
						const char *newgroup) {
	uid_t	uid;
	gid_t	gid;
	return (passwdentry::getUserId(newuser,&uid) &&
		groupentry::getGroupId(newgroup,&gid) &&
		changeOwner(filename,uid,gid));
}

bool file::changeOwner(const char *filename, uid_t uid, gid_t gid) {
	file	fl;
	return (fl.open(filename,O_RDWR) && fl.changeOwner(uid,gid));
}

bool file::changeOwnerUserId(const char *newuser) const {
	uid_t	uid;
	return (passwdentry::getUserId(newuser,&uid) &&
				changeOwnerUserId(uid));
}

bool file::changeOwnerUserId(uid_t uid) const {
	return changeOwner(uid,(gid_t)-1);
}

bool file::changeOwnerUserId(const char *filename, const char *newuser) {
	uid_t	uid;
	return (passwdentry::getUserId(newuser,&uid) &&
				changeOwnerUserId(filename,uid));
}

bool file::changeOwnerUserId(const char *filename, uid_t uid) {
	return changeOwner(filename,uid,(gid_t)-1);
}


bool file::changeOwnerGroupId(const char *newgroup) const {
	gid_t	gid;
	return (groupentry::getGroupId(newgroup,&gid) &&
				changeOwnerGroupId(gid));
}

bool file::changeOwnerGroupId(gid_t gid) const {
	return changeOwner((uid_t)-1,gid);
}

bool file::changeOwnerGroupId(const char *filename, const char *newgroup) {
	gid_t	gid;
	return (groupentry::getGroupId(newgroup,&gid) &&
				changeOwnerGroupId(filename,gid));
}

bool file::changeOwnerGroupId(const char *filename, gid_t gid) {
	return changeOwner(filename,(uid_t)-1,gid);
}

bool file::canChangeOwner(const char *filename) {
	return !pathConf(filename,_PC_CHOWN_RESTRICTED);
}

bool file::canChangeOwner() const {
	return !fpathConf(_PC_CHOWN_RESTRICTED);
}

bool file::rename(const char *oldpath, const char *newpath) {
	int	result;
	do {
		result=::rename(oldpath,newpath);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool file::remove(const char *filename) {
	int	result;
	do {
		result=unlink(filename);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

bool file::createHardLink(const char *oldpath, const char *newpath) {
	#if defined(RUDIMENTS_HAVE_LINK)
		int	result;
		do {
			result=link(oldpath,newpath);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_LOADLIBRARY)
		bool	retval=false;
		HMODULE	lib=LoadLibrary("KERNEL32");
		if (lib) {
			bool	(*proc)(LPCSTR,LPCSTR,
					LPSECURITY_ATTRIBUTES)=
				(bool (*)(LPCSTR,LPCSTR,
					LPSECURITY_ATTRIBUTES))
					GetProcAddress(lib,"CreateHardLinkA");
			if (proc) {
				retval=proc(newpath,oldpath,NULL);
			}
			FreeLibrary(lib);
		}
		return retval;
	#else
		#error no link or anything like it
	#endif
}

bool file::createSymbolicLink(const char *oldpath, const char *newpath) {
	#if defined(RUDIMENTS_HAVE_SYMLINK)
		int	result;
		do {
			result=symlink(oldpath,newpath);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(MINGW32)
		// windows doesn't support symlinks
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		// other platforms should support symlinks,
		// so something may be wrong
		#error no symlink or anything like it
	#endif
}

char *file::resolveSymbolicLink(const char *filename) {

	#if defined(RUDIMENTS_HAVE_READLINK)
		size_t	buffersize=1024;
		for (;;) {

			// create a buffer to store the path
			char	*buffer=new char[buffersize];

			// read the path into the buffer
			int	len;
			do {
				len=::readlink(filename,buffer,buffersize);
			} while (len==-1 && error::getErrorNumber()==EINTR);

			if (len==-1) {

				// if the call to readlink failed, delete the
				// buffer and return NULL
				delete[] buffer;
				return NULL;

			} else if ((size_t)len==buffersize) {

				// if the length of the path was the same as
				// the buffer size the we didn't get the entire
				// path, increase the size of the buffer and
				// try again
				delete[] buffer;
				buffersize=buffersize+1024;

				// if the buffer size exceeds 10k
				// then return failure
				if (buffersize>10240) {
					return NULL;
				}

			} else {
				// NULL-terminate the buffer, readlink()
				// doesn't do this for us
				buffer[len]='\0';
				return buffer;
			}
		}
	#elif defined(MINGW32)
		// windows doesn't support symlinks
		error::setErrorNumber(ENOSYS);
		return NULL;
	#else
		// other platforms should support symlinks,
		// so something may be wrong
		#error no readlink or anything like it
	#endif
}

bool file::sync() const {
	#if defined(RUDIMENTS_HAVE_FSYNC)
		int	result;
		do {
			result=fsync(fd());
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_COMMIT)
		return _commit(fd());
	#else
		#error no fsync or anything like it
	#endif
}

bool file::dataSync() const {
	#ifdef RUDIMENTS_HAVE_FDATASYNC
		int	result;
		do {
			result=fdatasync(fd());
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		return sync();
	#endif
}

bool file::setLastAccessTime(const char *filename,
					time_t lastaccesstime) {
	time_t	lastmodtime;
	if (!getLastModificationTime(filename,&lastmodtime)) {
		return false;
	}
	return setLastAccessAndModificationTimes(filename,
						lastaccesstime,
						lastmodtime);
}

bool file::setLastModificationTime(const char *filename,
					time_t lastmodtime) {
	time_t	lastaccesstime;
	if (!getLastAccessTime(filename,&lastaccesstime)) {
		return false;
	}
	return setLastAccessAndModificationTimes(filename,
						lastaccesstime,
						lastmodtime);
}

bool file::setLastAccessAndModificationTimes(const char *filename,
						time_t lastaccesstime,
						time_t lastmodtime) {
	#if defined(RUDIMENTS_HAVE_UTIMES)
		timeval	tv[2];
		tv[0].tv_sec=static_cast<long>(lastaccesstime);
		tv[0].tv_usec=0;
		tv[1].tv_sec=static_cast<long>(lastmodtime);
		tv[1].tv_usec=0;
		int	result;
		do {
			result=utimes(filename,tv);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_SETFILETIME)

		// open the file
		HANDLE	handle=CreateFile(filename,
						FILE_WRITE_ATTRIBUTES,
						FILE_SHARE_DELETE|
						FILE_SHARE_READ|
						FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL|
						FILE_FLAG_BACKUP_SEMANTICS,
						NULL);
		if (handle==INVALID_HANDLE_VALUE) {
			return false;
		}

		// windows time starts at 1/1/1601
		// unix time starts at 1/1/1970
		// this is the number of seconds between those dates
		#define	timediff	11644473600LLU

		// convert the times from unix to windows format
		ULARGE_INTEGER	lastaccessfiletime;
		lastaccessfiletime.QuadPart=(lastaccesstime+timediff)*10000000;
		ULARGE_INTEGER	lastmodfiletime;
		lastmodfiletime.QuadPart=(lastmodtime+timediff)*10000000;

		// set the file times
		bool	retval=SetFileTime(handle,NULL,
						(FILETIME *)&lastaccesstime,
						(FILETIME *)&lastmodtime);

		// close the file
		CloseHandle(handle);

		// return the result
		return retval;
	#else
		#error no utimes or anything like it
	#endif
}

bool file::setLastAccessAndModificationTimes(const char *filename) {
	#if defined(RUDIMENTS_HAVE_UTIMES)
		int	result;
		do {
			result=utimes(filename,NULL);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		datetime	dt;
		dt.getSystemDateAndTime();
		return setLastAccessAndModificationTimes(filename,
							dt.getEpoch(),
							dt.getEpoch());
	#endif
}

bool file::createFifo(const char *filename, mode_t perms) {
	#if defined(RUDIMENTS_HAVE_MKFIFO)
		int	result;
		do {
			result=mkfifo(filename,perms);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATENAMEDPIPE)
		HANDLE	handle=CreateNamedPipe(filename,0,0,1,
						1024,1024,5000,NULL);
		if (handle==INVALID_HANDLE_VALUE) {
			return false;
		}
		CloseHandle(handle);
		return true;
	#else
		#error no mkfifo or anything like it
	#endif
}

int file::createTemporaryFile(char *templatefilename) {
	#if defined(RUDIMENTS_HAVE_MKSTEMP)
		int	result;
		do {
			result=mkstemp(templatefilename);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#else
		// sanity check on templatefilename
		char	*lastsix=templatefilename+
				charstring::length(templatefilename)-6;
		if (charstring::compare(lastsix,"XXXXXX")) {
			error::setErrorNumber(EINVAL);
			return -1;
		}

		// replace X's with random characters...

		// seed the random number
		int	seed=randomnumber::getSeed();

		// for each of the 6 characters...
		for (uint8_t i=0; i<6; i++) {

			// get a random number, scale it to 0-60
			seed=randomnumber::generateNumber(seed);
			char	ch=(char)randomnumber::scaleNumber(seed,0,59);

			// translate...
			//  0-9  -> '0' - '9'
			// 10-34 -> 'A' - 'Z'
			// 35-59 -> 'a' - 'z'
			if (ch<10) {
				ch=ch+'0';
			} else if (ch<35) {
				ch=ch-10+'A';
			} else {
				ch=ch-35+'a';
			}

			// set character
			lastsix[i]=ch;
		}

		// create the file
		file	f;
		if (!f.create(templatefilename,
				permissions::evalPermString("rw-r--r--"))) {
			return -1;
		}

		// fake it out so the file won't get
		// closed when f is deallocated
		int	fdesc=f.getFileDescriptor();
		f.setFileDescriptor(-1);

		// return the file descriptor
		return fdesc;
	#endif
}

const char * const *file::listAttributes() const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return NULL;
	#else

	// The flistxattr interface is designed such that you have to guess the
	// size of the buffer that it will need, call flistxattr, then see if
	// your guess was correct.  If not, you have to try again.

	// try with a 8 byte buffer to start
	size_t	size=8;
	char	*buffer;

	// try 100 times...
	for (int i=0; i<100; i++) {

		// allocate a buffer
		buffer=new char[size];

		// attempt to read the attribute into the buffer
		ssize_t	newsize;
		do {
			newsize=flistxattr(fd(),buffer,size);
		} while (newsize==-1 && error::getErrorNumber()==EINTR);

		// it's possible that someone changed the attribute between
		// the previous 2 calls to fgetxattr and increased the size
		// of the buffer necessary to retrieve it, if so, try again
		// with a bigger buffer.
		if (newsize==-1) {
			delete[] buffer;
			return NULL;
		} else if (newsize>(ssize_t)size) {
			delete[] buffer;
			size=newsize;
		} else {
			const char * const	*retval=
						attributeArray(buffer,size);
			delete[] buffer;
			return retval;
		}
	}

	// if we couldn't get the attribute after 100 tries, give up
	return NULL;
	#endif
}

bool file::getAttribute(const char *name, unsigned short *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(number,buffer,sizeof(unsigned short));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, unsigned long *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(number,buffer,sizeof(unsigned long));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, short *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(number,buffer,sizeof(short));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, long *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(number,buffer,sizeof(long));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, float *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(number,buffer,sizeof(float));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, double *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(number,buffer,sizeof(double));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, unsigned char *character) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(character,buffer,sizeof(unsigned char));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, char *character) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(character,buffer,sizeof(char));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, bool *value) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	if (retval) {
		rawbuffer::copy(value,buffer,sizeof(bool));
		delete[] buffer;
	}
	return retval;
}

bool file::getAttribute(const char *name, unsigned char **string) const {
	size_t	size;
	return getAttribute(name,reinterpret_cast<void **>(string),&size);
}

bool file::getAttribute(const char *name, char **string) const {
	size_t	size;
	return getAttribute(name,reinterpret_cast<void **>(string),&size);
}

bool file::getAttribute(const char *name,
				unsigned char **string, size_t *size) const {
	return getAttribute(name,reinterpret_cast<void **>(string),size);
}

bool file::getAttribute(const char *name, char **string, size_t *size) const {
	return getAttribute(name,reinterpret_cast<void **>(string),size);
}

bool file::getAttribute(const char *name, void **buffer, size_t *size) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	*buffer=NULL;
	*size=0;
	return false;
	#else

	// The fgetxattr interface is designed such that you have to guess the
	// size of the buffer that it will need, call fgetxattr, then see if
	// your guess was correct.  If not, you have to try again.

	// try with a 8 byte buffer to start
	(*size)=8;

	// try 100 times...
	for (int i=0; i<100; i++) {

		// allocate a buffer
		(*buffer)=static_cast<void *>(new unsigned char[(*size)]);

		// attempt to read the attribute into the buffer
		ssize_t	newsize;
		do {
			newsize=fgetxattr(fd(),name,(*buffer),(*size));
		} while (newsize==-1 && error::getErrorNumber()==EINTR);

		// it's possible that someone changed the attribute between
		// the previous 2 calls to fgetxattr and increased the size
		// of the buffer necessary to retrieve it, if so, try again
		// with a bigger buffer.
		if (newsize==-1) {
			delete[] static_cast<unsigned char *>(*buffer);
			return false;
		} else if (newsize>(ssize_t)(*size)) {
			delete[] static_cast<unsigned char *>(*buffer);
			(*size)=newsize;
		} else {
			return true;
		}
	}

	// if we couldn't get the attribute after 100 tries, give up
	return false;

	#endif
}

bool file::createAttribute(const char *name, unsigned short number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, unsigned long number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, short number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, long number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, float number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, double number) const {
	return createAttribute(name,&number,sizeof(number));
}

bool file::createAttribute(const char *name, unsigned char character) const {
	return createAttribute(name,&character,sizeof(character));
}

bool file::createAttribute(const char *name, char character) const {
	return createAttribute(name,&character,sizeof(character));
}

bool file::createAttribute(const char *name, bool value) const {
	return createAttribute(name,&value,sizeof(value));
}

bool file::createAttribute(const char *name,
				const unsigned char *string) const {
	return createAttribute(name,string,charstring::length(string));
}

bool file::createAttribute(const char *name, const char *string) const {
	return createAttribute(name,string,charstring::length(string));
}

bool file::createAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return createAttribute(name,static_cast<const void *>(string),size);
}

bool file::createAttribute(const char *name, const char *string,
							size_t size) const {
	return createAttribute(name,static_cast<const void *>(string),size);
}

#ifndef XATTR_CREATE
#define XATTR_CREATE 0
#endif
bool file::createAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,XATTR_CREATE);
}

bool file::replaceAttribute(const char *name, unsigned short number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, unsigned long number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, short number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, long number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, float number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, double number) const {
	return replaceAttribute(name,&number,sizeof(number));
}

bool file::replaceAttribute(const char *name, unsigned char character) const {
	return replaceAttribute(name,&character,sizeof(character));
}

bool file::replaceAttribute(const char *name, char character) const {
	return replaceAttribute(name,&character,sizeof(character));
}

bool file::replaceAttribute(const char *name, bool value) const {
	return replaceAttribute(name,&value,sizeof(value));
}

bool file::replaceAttribute(const char *name,
				const unsigned char *string) const {
	return replaceAttribute(name,string,charstring::length(string));
}

bool file::replaceAttribute(const char *name, const char *string) const {
	return replaceAttribute(name,string,charstring::length(string));
}

bool file::replaceAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return replaceAttribute(name,static_cast<const void *>(string),size);
}

bool file::replaceAttribute(const char *name, const char *string,
							size_t size) const {
	return replaceAttribute(name,static_cast<const void *>(string),size);
}

#ifndef XATTR_REPLACE
#define XATTR_REPLACE 0
#endif
bool file::replaceAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,XATTR_REPLACE);
}

bool file::setAttribute(const char *name, unsigned short number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, unsigned long number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, short number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, long number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, float number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, double number) const {
	return setAttribute(name,&number,sizeof(number));
}

bool file::setAttribute(const char *name, unsigned char character) const {
	return setAttribute(name,&character,sizeof(character));
}

bool file::setAttribute(const char *name, char character) const {
	return setAttribute(name,&character,sizeof(character));
}

bool file::setAttribute(const char *name, bool value) const {
	return setAttribute(name,&value,sizeof(value));
}

bool file::setAttribute(const char *name, const unsigned char *string) const {
	return setAttribute(name,string,charstring::length(string));
}

bool file::setAttribute(const char *name, const char *string) const {
	return setAttribute(name,string,charstring::length(string));
}

bool file::setAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return setAttribute(name,static_cast<const void *>(string),size);
}

bool file::setAttribute(const char *name, const char *string,
							size_t size) const {
	return setAttribute(name,static_cast<const void *>(string),size);
}

bool file::setAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,0);
}

bool file::setAttribute(const char *name, const void *value,
						size_t size, int flags) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return false;
	#else

	ssize_t	result;
	do {
		result=fsetxattr(fd(),name,value,size,flags);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;

	#endif
}

bool file::removeAttribute(const char *name) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return false;
	#else

	ssize_t	result;
	do {
		result=fremovexattr(fd(),name);
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;

	#endif
}

const char * const *file::attributeArray(const char *buffer,
						size_t size) const {

	#ifndef RUDIMENTS_HAVE_XATTRS
	return NULL;
	#else


	// count the number of attributes
	int	counter=0;
	for (size_t index=0; index<size; index++) {
		if (!buffer[index]) {
			counter++;
		}
	}

	// create an array to hold them
	char	**attributes=new char *[counter+1];
	attributes[counter]=NULL;

	// copy the attributes into the array
	const char	*ptr=buffer;
	counter=0;
	for (size_t index=0; index<size; index++) {
		if (!buffer[index]) {
			attributes[counter]=charstring::duplicate(ptr);
			if (index<size-1) {
				ptr=&buffer[index+1];
			}
			counter++;
		}
	}

	// return the array
	return attributes;

	#endif
}

struct stat *file::getStat() {
	return &(pvt->_st);
}

char *file::dirname(const char *filename) {

	if (!filename) {
		return NULL;
	} else if (!charstring::contains(filename,'/') ||
			!charstring::compare(filename,".")) {
		return charstring::duplicate(".");
	} else if (!charstring::compare(filename,"..")) {
		return charstring::duplicate("..");
	} else if (!charstring::compare(filename,"/")) {
		return charstring::duplicate("/");
	}

	char	*retval=charstring::duplicate(filename);
	charstring::rightTrim(retval,'/');
	char	*lastslash=charstring::findLast(retval,'/');
	if (lastslash==retval) {
		(*(lastslash+1))='\0';
	} else {
		(*lastslash)='\0';
	}
	return retval;
}

char *file::basename(const char *filename) {

	if (!filename) {
		return NULL;
	}

	char	*copy=charstring::duplicate(filename);
	charstring::rightTrim(copy,'/');

	char	*retval;
	char	*ptr=charstring::findLast(copy,'/');
	if (!ptr) {
		retval=charstring::duplicate(copy);
	} else {
		retval=charstring::duplicate(ptr+1);
	}
	delete[] copy;
	return retval;
}

char *file::basename(const char *filename, const char *suffix) {
	char	*retval=basename(filename);
	char	*ptr=charstring::findLast(retval,suffix);
	if (!(*(ptr+charstring::length(suffix)))) {
		(*ptr)='\0';
	}
	return retval;
}

key_t file::generateKey(const char *filename, int id) {
#ifdef RUDIMENTS_HAVE_FTOK
	#ifdef RUDIMENTS_HAVE_CONST_CHAR_FTOK
		return ::ftok(filename,id);
	#else
		return ::ftok(const_cast<char *>(filename),id);
	#endif
#else
	file	f;
	if (!f.open(filename,O_RDONLY)) {
		return -1;
	}
	return (key_t)((f.getInode() & 0xffff) |
			((f.getDevice() & 0xff) << 16) |
			((id & 0xff)  << 24));
#endif
}

long file::maxLinks(const char *filename) {
	return pathConf(filename,_PC_LINK_MAX);
}

long file::maxLinks() const {
	return fpathConf(_PC_LINK_MAX);
}

bool file::posixFadvise(off64_t offset, off64_t len, int advice) const {
	#ifdef RUDIMENTS_HAVE_POSIX_FADVISE
		int	result;
		do {
			result=posix_fadvise(fd(),offset,len,advice);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#else
		// on platforms that don't support this, it's ok to just return
		// success since really it only gives the filesystem hints
		return true;
	#endif
}

long file::pathConf(const char *path, int name) {
	#if defined(RUDIMENTS_HAVE_PATHCONF)
		long	result;
		do {
			result=pathconf(path,name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#elif defined(MINGW32)
		// no idea how to support this on windows
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no pathconf or anything like it
	#endif
}

long file::fpathConf(int name) const {
	#if defined(RUDIMENTS_HAVE_FPATHCONF)
		long	result;
		do {
			result=fpathconf(fd(),name);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return result;
	#elif defined(MINGW32)
		// no idea how to support this on windows
		error::setErrorNumber(ENOSYS);
		return -1;
	#else
		#error no fpathconf or anything like it
	#endif
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
