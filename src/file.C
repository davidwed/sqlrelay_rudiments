// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/file.h>
#include <rudiments/passwdentry.h>
#include <rudiments/groupentry.h>
#include <rudiments/charstring.h>
#include <rudiments/rawbuffer.h>


#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#ifdef HAVE_XATTRS
	#include <sys/xattr.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

file::file() : filedescriptor() {
	rawbuffer::zero(&st,sizeof(st));
	getcurrentpropertiesonopen=true;
	retryinterruptedlockops=true;
}

file::file(const file &f) : filedescriptor(f) {
	fileClone(f);
}

file &file::operator=(const file &f) {
	if (this!=&f) {
		filedescriptor::operator=(f);
		fileClone(f);
	}
	return *this;
}

void file::fileClone(const file &f) {
	st=f.st;
	getcurrentpropertiesonopen=f.getcurrentpropertiesonopen;
	retryinterruptedlockops=f.retryinterruptedlockops;
}

file::~file() {}

bool file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

ssize_t file::create(const char *name, mode_t perms,
						unsigned short number) {
	return create(name,perms,static_cast<const void *>(&number),
							sizeof(unsigned short));
}

ssize_t file::create(const char *name, mode_t perms, unsigned long number) {
	return create(name,perms,static_cast<const void *>(&number),
							sizeof(unsigned long));
}

ssize_t file::create(const char *name, mode_t perms, float number) {
	return create(name,perms,static_cast<const void *>(&number),
								sizeof(float));
}

ssize_t file::create(const char *name, mode_t perms, double number) {
	return create(name,perms,static_cast<const void *>(&number),
								sizeof(double));
}

ssize_t file::create(const char *name, mode_t perms, char number) {
	return create(name,perms,static_cast<const void *>(&number),
								sizeof(char));
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
	if (((fd=openInternal(name,O_CREAT|O_TRUNC|O_RDWR,perms))!=-1) &&
		((retval=write(data,size))==size) &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true)) {
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
	return ::open(name,flags);
}

int file::openInternal(const char *name, int flags, mode_t perms) {
	return ::open(name,flags,perms);
}

bool file::open(const char *name, int flags) {
	return ((fd=openInternal(name,flags))!=-1 &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true));
}

bool file::open(const char *name, int flags, mode_t perms) {
	return ((fd=openInternal(name,flags,perms))!=-1 &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true));
}

char *file::getContents() {
	off_t	size=(fd>-1)?st.st_size:0;
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

bool file::unlockFile(short type) const {
	return unlockRegion(0,0);
}

bool file::tryLockRegion(short type, off_t start, off_t len) const {
	return lock(F_SETLK,type,SEEK_SET,start,len);
}

bool file::lockRegion(short type, off_t start, off_t len) const {
	return lock(F_SETLKW,type,SEEK_SET,start,len);
}

bool file::checkLockRegion(short type, off_t start, off_t len,
						struct flock *retlck) const {
	return checkLock(type,SEEK_SET,start,len,retlck);
}

bool file::unlockRegion(off_t start, off_t len) const {
	return unlock(SEEK_SET,start,len);
}

bool file::tryLockFromCurrent(short type, off_t len) const {
	return tryLockFromCurrent(type,0,len);
}

bool file::tryLockFromCurrent(short type, off_t start, off_t len) const {
	return lock(F_SETLK,type,SEEK_CUR,start,len);
}

bool file::lockFromCurrent(short type, off_t len) const {
	return lockFromCurrent(type,0,len);
}

bool file::lockFromCurrent(short type, off_t start, off_t len) const {
	return lock(F_SETLKW,type,SEEK_CUR,start,len);
}

bool file::checkLockFromCurrent(short type, off_t len,
				struct flock *retlck) const {
	return checkLockFromCurrent(type,0,len,retlck);
}

bool file::checkLockFromCurrent(short type, off_t start, off_t len,
						struct flock *retlck) const {
	return checkLock(type,SEEK_CUR,start,len,retlck);
}

bool file::unlockFromCurrent(off_t len) const {
	return unlockFromCurrent(0,len);
}

bool file::unlockFromCurrent(off_t start, off_t len) const {
	return unlock(SEEK_CUR,start,len);
}

bool file::tryLockFromEnd(short type, off_t len) const {
	return tryLockFromEnd(type,0,len);
}

bool file::tryLockFromEnd(short type, off_t start, off_t len) const {
	return lock(F_SETLK,type,SEEK_END,start,len);
}

bool file::lockFromEnd(short type, off_t len) const {
	return lockFromEnd(type,0,len);
}

bool file::lockFromEnd(short type, off_t start, off_t len) const {
	return lock(F_SETLKW,type,SEEK_END,start,len);
}

bool file::checkLockFromEnd(short type, off_t len, struct flock *retlck) const {
	return checkLockFromEnd(type,0,len,retlck);
}

bool file::checkLockFromEnd(short type, off_t start, off_t len,
						struct flock *retlck) const {
	return checkLock(type,SEEK_END,start,len,retlck);
}

bool file::unlockFromEnd(off_t len) const {
	return unlockFromEnd(0,len);
}

bool file::unlockFromEnd(off_t start, off_t len) const {
	return unlock(SEEK_END,start,len);
}

bool file::tryLockRemainder(short type, off_t start) const {
	return lock(F_SETLK,type,SEEK_SET,start,0);
}

bool file::lockRemainder(short type, off_t start) const {
	return lock(F_SETLKW,type,SEEK_SET,start,0);
}

bool file::checkLockRemainder(short type, off_t start,
					struct flock *retlck) const {
	return checkLock(type,SEEK_SET,start,0,retlck);
}

bool file::unlockRemainder(off_t start) const {
	return unlock(SEEK_SET,start,0);
}

bool file::tryLockRemainderFromCurrent(short type) const {
	return tryLockRemainderFromCurrent(type,0);
}

bool file::tryLockRemainderFromCurrent(short type, off_t start) const {
	return lock(F_SETLK,type,SEEK_CUR,start,0);
}

bool file::lockRemainderFromCurrent(short type) const {
	return lockRemainderFromCurrent(type,0);
}

bool file::lockRemainderFromCurrent(short type, off_t start) const {
	return lock(F_SETLKW,type,SEEK_CUR,start,0);
}

bool file::checkLockRemainderFromCurrent(short type,
					struct flock *retlck) const {
	return checkLockRemainderFromCurrent(type,0,retlck);
}

bool file::checkLockRemainderFromCurrent(short type, off_t start,
					struct flock *retlck) const {
	return checkLock(type,SEEK_CUR,start,0,retlck);
}

bool file::unlockRemainderFromCurrent() const {
	return unlockRemainderFromCurrent(0);
}

bool file::unlockRemainderFromCurrent(off_t start) const {
	return unlock(SEEK_CUR,start,0);
}

bool file::tryLockRemainderFromEnd(short type) const {
	return tryLockRemainderFromEnd(type,0);
}

bool file::tryLockRemainderFromEnd(short type, off_t start) const {
	return lock(F_SETLK,type,SEEK_END,start,0);
}

bool file::lockRemainderFromEnd(short type) const {
	return lockRemainderFromEnd(type,0);
}

bool file::lockRemainderFromEnd(short type, off_t start) const {
	return lock(F_SETLKW,type,SEEK_END,start,0);
}

bool file::checkLockRemainderFromEnd(short type, struct flock *retlck) const {
	return checkLockRemainderFromEnd(type,0,retlck);
}

bool file::checkLockRemainderFromEnd(short type, off_t start,
						struct flock *retlck) const {
	return checkLock(type,SEEK_END,start,0,retlck);
}

bool file::sequentialAccess(off_t start, size_t len) const {
	#ifdef HAVE_POSIX_FADVISE
	return !posix_fadvise(fd,start,len,POSIX_FADV_SEQUENTIAL);
	#else
	return true;
	#endif
}

bool file::randomAccess(off_t start, size_t len) const {
	#ifdef HAVE_POSIX_FADVISE
	return !posix_fadvise(fd,start,len,POSIX_FADV_RANDOM);
	#else
	return true;
	#endif
}

bool file::onlyOnce(off_t start, size_t len) const {
	#ifdef HAVE_POSIX_FADVISE
	return !posix_fadvise(fd,start,len,POSIX_FADV_NOREUSE);
	#else
	return true;
	#endif
}

bool file::willNeed(off_t start, size_t len) const {
	#ifdef HAVE_POSIX_FADVISE
	return !posix_fadvise(fd,start,len,POSIX_FADV_WILLNEED);
	#else
	return true;
	#endif
}

bool file::wontNeed(off_t start, size_t len) const {
	#ifdef HAVE_POSIX_FADVISE
	return !posix_fadvise(fd,start,len,POSIX_FADV_DONTNEED);
	#else
	return true;
	#endif
}

bool file::normalAccess(off_t start, size_t len) const {
	#ifdef HAVE_POSIX_FADVISE
	return !posix_fadvise(fd,start,len,POSIX_FADV_NORMAL);
	#else
	return true;
	#endif
}

#ifdef HAVE_POSIX_FALLOCATE
bool file::reserve(off_t start, size_t len) const {
	return !posix_fallocate(fd,start,len);
}
#endif

bool file::truncate(const char *filename) {
	return !::truncate(filename,0);
}

bool file::truncate(const char *filename, off_t length) {
	return !::truncate(filename,length);
}

bool file::truncate() const {
	return !::ftruncate(fd,0);
}

bool file::truncate(off_t length) const {
	return !::ftruncate(fd,length);
}

bool file::unlockRemainderFromEnd() const {
	return unlockRemainderFromEnd(0);
}

bool file::unlockRemainderFromEnd(off_t start) const {
	return unlock(SEEK_END,start,0);
}

off_t file::setPositionRelativeToBeginning(off_t offset) const {
	return lseek(fd,offset,SEEK_SET);
}

off_t file::setPositionRelativeToCurrent(off_t offset) const {
	return lseek(fd,offset,SEEK_CUR);
}

off_t file::setPositionRelativeToEnd(off_t offset) const {
	return lseek(fd,offset,SEEK_END);
}

off_t file::getCurrentPosition() const {
	return lseek(fd,0,SEEK_CUR);
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
	return !access(filename,mode);
}

bool file::getCurrentProperties() {
	return (fstat(fd,&st)!=-1);
}

#define STAT(filename,out,member) \
	struct stat st; \
	if (stat(filename,&st)==-1) { \
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

bool file::getSize(const char *filename, off_t *size) {
	STAT(filename,size,st_size)
}

bool file::getBlockSize(const char *filename, blksize_t *size) {
#ifdef HAVE_BLKSIZE_T
	STAT(filename,size,st_blksize)
#else
	*size=-1;
	return true;
#endif
}

bool file::getBlockCount(const char *filename, blkcnt_t *blocks) {
#ifdef HAVE_BLKCNT_T
	STAT(filename,blocks,st_blocks)
#else
	*blocks=-1;
	return true;
#endif
}

#ifndef HAVE_S_ISSOCK
	#define S_ISSOCK(m) ((m&040000==040000)?1:0)
#endif

int file::isSocket(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

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
	return st.st_mode;
}

uid_t file::getOwnerUserId() const {
	return st.st_uid;
}

gid_t file::getOwnerGroupId() const {
	return st.st_gid;
}

off_t file::getSize() const {
	return st.st_size;
}


blksize_t file::getBlockSize() const {
#ifdef HAVE_BLKSIZE_T
	return st.st_blksize;
#else
	return -1;
#endif
}

blkcnt_t file::getBlockCount() const {
#ifdef HAVE_BLKCNT_T
	return st.st_blocks;
#else
	return -1;
#endif
}

int file::isSocket() const {
	return S_ISSOCK(st.st_mode);
}

int file::isSymbolicLink() const {
	return S_ISLNK(st.st_mode);
}

int file::isRegularFile() const {
	return S_ISREG(st.st_mode);
}

int file::isBlockDevice() const {
	return S_ISBLK(st.st_mode);
}

int file::isDirectory() const {
	return S_ISDIR(st.st_mode);
}

int file::isCharacterDevice() const {
	return S_ISCHR(st.st_mode);
}

int file::isFifo() const {
	return S_ISFIFO(st.st_mode);
}

time_t file::getLastAccessTime() const {
	return st.st_atime;
}

time_t file::getLastModificationTime() const {
	return st.st_mtime;
}

time_t file::getLastChangeTime() const {
	return st.st_ctime;
}


dev_t file::getDevice() const {
	return st.st_dev;
}

dev_t file::getDeviceType() const {
	return st.st_rdev;
}

ino_t file::getInode() const {
	return st.st_ino;
}

nlink_t file::getNumberOfHardLinks() const {
	return st.st_nlink;
}

struct stat *file::getStat() {
	return &st;
}

void file::getCurrentPropertiesOnOpen() {
	getcurrentpropertiesonopen=true;
}

void file::dontGetCurrentPropertiesOnOpen() {
	getcurrentpropertiesonopen=false;
}

bool file::lock(int method, short type, short whence,
				off_t start, off_t len) const {
	struct flock	lck;
	lck.l_type=type;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	return !fcntl(method,reinterpret_cast<long>(&lck));
}

bool file::checkLock(short type, short whence, off_t start, off_t len,
						struct flock *retlck) const {
	struct flock	lck;
	lck.l_type=type;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	bool	retval=(!fcntl(F_SETLKW,reinterpret_cast<long>(&lck)));
	*retlck=lck;
	return retval;
}

bool file::unlock(short whence, off_t start, off_t len) const {
	struct flock	lck;
	lck.l_type=F_UNLCK;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	return !fcntl(F_SETLK,reinterpret_cast<long>(&lck));
}

bool file::changeOwner(const char *newuser, const char *newgroup) const {
	uid_t	uid;
	gid_t	gid;
	return (passwdentry::getUserId(newuser,&uid) &&
		groupentry::getGroupId(newgroup,&gid) &&
		changeOwner(uid,gid));
}

bool file::changeOwner(uid_t uid, gid_t gid) const {
	return !fchown(fd,uid,gid);
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
	return !chown(filename,uid,gid);
}


bool file::changeOwnerUserId(const char *newuser) const {
	uid_t	uid;
	return (passwdentry::getUserId(newuser,&uid) &&
				changeOwnerUserId(uid));
}

bool file::changeOwnerUserId(uid_t uid) const {
	return !fchown(fd,uid,(gid_t)-1);
}

bool file::changeOwnerUserId(const char *filename, const char *newuser) {
	uid_t	uid;
	return (passwdentry::getUserId(newuser,&uid) &&
				changeOwnerUserId(filename,uid));
}

bool file::changeOwnerUserId(const char *filename, uid_t uid) {
	return !chown(filename,uid,(gid_t)-1);
}


bool file::changeOwnerGroupId(const char *newgroup) const {
	gid_t	gid;
	return (groupentry::getGroupId(newgroup,&gid) &&
				changeOwnerGroupId(gid));
}

bool file::changeOwnerGroupId(gid_t gid) const {
	return !fchown(fd,(uid_t)-1,gid);
}

bool file::changeOwnerGroupId(const char *filename, const char *newgroup) {
	gid_t	gid;
	return (groupentry::getGroupId(newgroup,&gid) &&
				changeOwnerGroupId(filename,gid));
}

bool file::changeOwnerGroupId(const char *filename, gid_t gid) {
	return !chown(filename,(uid_t)-1,gid);
}

bool file::canChangeOwner(const char *filename) {
	return !pathconf(filename,_PC_CHOWN_RESTRICTED);
}

bool file::canChangeOwner() const {
	return !fpathconf(fd,_PC_CHOWN_RESTRICTED);
}

bool file::rename(const char *oldpath, const char *newpath) {
	return !::rename(oldpath,newpath);
}

bool file::remove(const char *filename) {
	return !unlink(filename);
}

bool file::createHardLink(const char *oldpath, const char *newpath) {
	return !link(oldpath,newpath);
}

bool file::createSymbolicLink(const char *oldpath, const char *newpath) {
	return !symlink(oldpath,newpath);
}

char *file::resolveSymbolicLink(const char *filename) {

	size_t	buffersize=1024;
	for (;;) {

		// create a buffer to store the path
		char	*buffer=new char[buffersize];

		// read the path into the buffer
		int	len=::readlink(filename,buffer,buffersize);

		if (len==-1) {

			// if the call to readlink failed, delete the buffer
			// and return NULL
			delete[] buffer;
			return NULL;

		} else if ((size_t)len==buffersize) {

			// if the length of the path was the same as the buffer
			// size the we didn't get the entire path, increase the
			// size of the buffer and try again
			delete[] buffer;
			buffersize=buffersize+1024;

			// if the buffer size exceeds 10k then return failure
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
}

bool file::sync() const {
	return !fsync(fd);
}

#ifdef HAVE_FDATASYNC
bool file::dataSync() const {
	return !fdatasync(fd);
}
#endif

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
	timeval	tv[2];
	tv[0].tv_sec=static_cast<long>(lastaccesstime);
	tv[0].tv_usec=0;
	tv[1].tv_sec=static_cast<long>(lastmodtime);
	tv[1].tv_usec=0;
	return !utimes(filename,tv);
}

bool file::setLastAccessAndModificationTimes(const char *filename) {
	return !utimes(filename,NULL);
}

bool file::createFifo(const char *filename, mode_t perms) {
	return !mkfifo(filename,perms);
}

int file::createTemporaryFile(char *templatefilename) {
	return mkstemp(templatefilename);
}

#ifdef HAVE_XATTRS
char **file::listAttributes() const {

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
		ssize_t	newsize=flistxattr(fd,buffer,size);

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
			char	**retval=attributeArray(buffer,size);
			delete[] buffer;
			return retval;
		}
	}

	// if we couldn't get the attribute after 100 tries, give up
	return NULL;
}

bool file::getAttribute(const char *name, unsigned short *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(number),
			static_cast<const void *>(buffer),
			sizeof(unsigned short));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, unsigned long *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(number),
			static_cast<const void *>(buffer),
			sizeof(unsigned long));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, short *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(number),
			static_cast<const void *>(buffer),
			sizeof(short));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, long *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(number),
			static_cast<const void *>(buffer),
			sizeof(long));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, float *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(number),
			static_cast<const void *>(buffer),
			sizeof(float));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, double *number) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(number),
			static_cast<const void *>(buffer),
			sizeof(double));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, unsigned char *character) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(character),
			static_cast<const void *>(buffer),
			sizeof(unsigned char));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, char *character) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(character),
			static_cast<const void *>(buffer),
			sizeof(char));
	delete[] buffer;
	return retval;
}

bool file::getAttribute(const char *name, bool *value) const {
	size_t		size;
	unsigned char	*buffer;
	bool	retval=getAttribute(name,
				reinterpret_cast<void **>(&buffer),
				&size);
	rawbuffer::copy(static_cast<void *>(value),
			static_cast<const void *>(buffer),
			sizeof(bool));
	delete[] buffer;
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
		ssize_t	newsize=fgetxattr(fd,name,(*buffer),(*size));

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
}

bool file::createAttribute(const char *name, unsigned short number) const {
	return createAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::createAttribute(const char *name, unsigned long number) const {
	return createAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::createAttribute(const char *name, short number) const {
	return createAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::createAttribute(const char *name, long number) const {
	return createAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::createAttribute(const char *name, float number) const {
	return createAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::createAttribute(const char *name, double number) const {
	return createAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::createAttribute(const char *name, unsigned char character) const {
	return createAttribute(name,static_cast<const void *>(&character),
							sizeof(character));
}

bool file::createAttribute(const char *name, char character) const {
	return createAttribute(name,static_cast<const void *>(&character),
							sizeof(character));
}

bool file::createAttribute(const char *name, bool value) const {
	return createAttribute(name,static_cast<const void *>(&value),
							sizeof(value));
}

bool file::createAttribute(const char *name,
				const unsigned char *string) const {
	return createAttribute(name,static_cast<const void *>(string),
						charstring::length(string));
}

bool file::createAttribute(const char *name, const char *string) const {
	return createAttribute(name,static_cast<const void *>(string),
						charstring::length(string));
}

bool file::createAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return createAttribute(name,static_cast<const void *>(string),size);
}

bool file::createAttribute(const char *name, const char *string,
							size_t size) const {
	return createAttribute(name,static_cast<const void *>(string),size);
}

bool file::createAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,XATTR_CREATE);
}

bool file::replaceAttribute(const char *name, unsigned short number) const {
	return replaceAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::replaceAttribute(const char *name, unsigned long number) const {
	return replaceAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::replaceAttribute(const char *name, short number) const {
	return replaceAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::replaceAttribute(const char *name, long number) const {
	return replaceAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::replaceAttribute(const char *name, float number) const {
	return replaceAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::replaceAttribute(const char *name, double number) const {
	return replaceAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::replaceAttribute(const char *name, unsigned char character) const {
	return replaceAttribute(name,static_cast<const void *>(&character),
							sizeof(character));
}

bool file::replaceAttribute(const char *name, char character) const {
	return replaceAttribute(name,static_cast<const void *>(&character),
							sizeof(character));
}

bool file::replaceAttribute(const char *name, bool value) const {
	return replaceAttribute(name,static_cast<const void *>(&value),
							sizeof(value));
}

bool file::replaceAttribute(const char *name,
				const unsigned char *string) const {
	return replaceAttribute(name,static_cast<const void *>(string),
						charstring::length(string));
}

bool file::replaceAttribute(const char *name, const char *string) const {
	return replaceAttribute(name,static_cast<const void *>(string),
						charstring::length(string));
}

bool file::replaceAttribute(const char *name, const unsigned char *string,
							size_t size) const {
	return replaceAttribute(name,static_cast<const void *>(string),size);
}

bool file::replaceAttribute(const char *name, const char *string,
							size_t size) const {
	return replaceAttribute(name,static_cast<const void *>(string),size);
}

bool file::replaceAttribute(const char *name, const void *value,
							size_t size) const {
	return setAttribute(name,value,size,XATTR_REPLACE);
}

bool file::setAttribute(const char *name, unsigned short number) const {
	return setAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::setAttribute(const char *name, unsigned long number) const {
	return setAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::setAttribute(const char *name, short number) const {
	return setAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::setAttribute(const char *name, long number) const {
	return setAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::setAttribute(const char *name, float number) const {
	return setAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::setAttribute(const char *name, double number) const {
	return setAttribute(name,static_cast<const void *>(&number),
							sizeof(number));
}

bool file::setAttribute(const char *name, unsigned char character) const {
	return setAttribute(name,static_cast<const void *>(&character),
							sizeof(character));
}

bool file::setAttribute(const char *name, char character) const {
	return setAttribute(name,static_cast<const void *>(&character),
							sizeof(character));
}

bool file::setAttribute(const char *name, bool value) const {
	return setAttribute(name,static_cast<const void *>(&value),
							sizeof(value));
}

bool file::setAttribute(const char *name, const unsigned char *string) const {
	return setAttribute(name,static_cast<const void *>(string),
						charstring::length(string));
}

bool file::setAttribute(const char *name, const char *string) const {
	return setAttribute(name,static_cast<const void *>(string),
						charstring::length(string));
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
	return fsetxattr(fd,name,value,size,flags);
}

bool file::removeAttribute(const char *name) const {
	return fremovexattr(fd,name);
}

char **file::attributeArray(const char *buffer, size_t size) const {

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
}
#endif

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
	return ::ftok(filename,id);
}

long file::maxLinks(const char *filename) {
	return pathconf(filename,_PC_LINK_MAX);
}

long file::maxLinks() const {
	return fpathconf(fd,_PC_LINK_MAX);
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
