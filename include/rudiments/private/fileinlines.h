// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE file::file() : filedescriptor() {
	getcurrentpropertiesonopen=true;
}

RUDIMENTS_INLINE file::file(int fd) : filedescriptor(fd) {
}

RUDIMENTS_INLINE bool file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

RUDIMENTS_INLINE ssize_t file::create(const char *name, mode_t perms,
						unsigned short number) {
	return create(name,perms,(void *)&number,sizeof(unsigned short));
}

RUDIMENTS_INLINE ssize_t file::create(const char *name, mode_t perms,
						unsigned long number) {
	return create(name,perms,(void *)&number,sizeof(unsigned long));
}

RUDIMENTS_INLINE ssize_t file::create(const char *name, mode_t perms,
								float number) {
	return create(name,perms,(void *)&number,sizeof(float));
}

RUDIMENTS_INLINE ssize_t file::create(const char *name, mode_t perms,
								double number) {
	return create(name,perms,(void *)&number,sizeof(double));
}

RUDIMENTS_INLINE ssize_t file::create(const char *name, mode_t perms,
								char number) {
	return create(name,perms,(void *)&number,sizeof(char));
}

RUDIMENTS_INLINE ssize_t file::create(const char *name, mode_t perms,
					const char *string, size_t size) {
	return create(name,perms,(void *)string,size);
}

RUDIMENTS_INLINE bool file::lock(int method, short type, short whence,
						off_t start, off_t len) {
	flock	lck;
	lck.l_type=type;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	return !fcntl(fd,method,&lck);
}

RUDIMENTS_INLINE bool file::checkLock(short type, short whence,
					off_t start, off_t len, flock *retlck) {
	flock	lck;
	lck.l_type=type;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	bool	retval=(!fcntl(fd,F_SETLKW,&lck));
	memcpy((void *)retlck,(void *)&lck,sizeof(flock));
	return retval;
}

RUDIMENTS_INLINE bool file::unlock(short whence, off_t start, off_t len) {
	flock	lck;
	lck.l_type=F_UNLCK;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	return !fcntl(fd,F_SETLK,&lck);
}

RUDIMENTS_INLINE bool file::tryLockFile(short type) {
	return tryLockRegion(type,0,0);
}

RUDIMENTS_INLINE bool file::lockFile(short type) {
	return lockRegion(type,0,0);
}

RUDIMENTS_INLINE bool file::checkLockFile(short type, flock *retlck) {
	return checkLockRegion(type,0,0,retlck);
}

RUDIMENTS_INLINE bool file::unlockFile(short type) {
	return unlockRegion(0,0);
}

RUDIMENTS_INLINE bool file::tryLockRegion(short type, off_t start, off_t len) {
	return lock(F_SETLK,type,SEEK_SET,start,len);
}

RUDIMENTS_INLINE bool file::lockRegion(short type, off_t start, off_t len) {
	return lock(F_SETLKW,type,SEEK_SET,start,len);
}

RUDIMENTS_INLINE bool file::checkLockRegion(short type, off_t start,
						off_t len, flock *retlck) {
	return checkLock(type,SEEK_SET,start,len,retlck);
}

RUDIMENTS_INLINE bool file::unlockRegion(off_t start, off_t len) {
	return unlock(SEEK_SET,start,len);
}

RUDIMENTS_INLINE bool file::tryLockFromCurrent(short type, off_t len) {
	return tryLockFromCurrent(type,0,len);
}

RUDIMENTS_INLINE bool file::tryLockFromCurrent(short type,
						off_t start, off_t len) {
	return lock(F_SETLK,type,SEEK_CUR,start,len);
}

RUDIMENTS_INLINE bool file::lockFromCurrent(short type, off_t len) {
	return lockFromCurrent(type,0,len);
}

RUDIMENTS_INLINE bool file::lockFromCurrent(short type,
						off_t start, off_t len) {
	return lock(F_SETLKW,type,SEEK_CUR,start,len);
}

RUDIMENTS_INLINE bool file::checkLockFromCurrent(short type,
						off_t len, flock *retlck) {
	return checkLockFromCurrent(type,0,len,retlck);
}

RUDIMENTS_INLINE bool file::checkLockFromCurrent(short type, off_t start,
						off_t len, flock *retlck) {
	return checkLock(type,SEEK_CUR,start,len,retlck);
}

RUDIMENTS_INLINE bool file::unlockFromCurrent(off_t len) {
	return unlockFromCurrent(0,len);
}

RUDIMENTS_INLINE bool file::unlockFromCurrent(off_t start, off_t len) {
	return unlock(SEEK_CUR,start,len);
}

RUDIMENTS_INLINE bool file::tryLockFromEnd(short type, off_t len) {
	return tryLockFromEnd(type,0,len);
}

RUDIMENTS_INLINE bool file::tryLockFromEnd(short type, off_t start, off_t len) {
	return lock(F_SETLK,type,SEEK_END,start,len);
}

RUDIMENTS_INLINE bool file::lockFromEnd(short type, off_t len) {
	return lockFromEnd(type,0,len);
}

RUDIMENTS_INLINE bool file::lockFromEnd(short type, off_t start, off_t len) {
	return lock(F_SETLKW,type,SEEK_END,start,len);
}

RUDIMENTS_INLINE bool file::checkLockFromEnd(short type,
						off_t len, flock *retlck) {
	return checkLockFromEnd(type,0,len,retlck);
}

RUDIMENTS_INLINE bool file::checkLockFromEnd(short type, off_t start,
						off_t len, flock *retlck) {
	return checkLock(type,SEEK_END,start,len,retlck);
}

RUDIMENTS_INLINE bool file::unlockFromEnd(off_t len) {
	return unlockFromEnd(0,len);
}

RUDIMENTS_INLINE bool file::unlockFromEnd(off_t start, off_t len) {
	return unlock(SEEK_END,start,len);
}

RUDIMENTS_INLINE bool file::tryLockRemainder(short type, off_t start) {
	return lock(F_SETLK,type,SEEK_SET,start,0);
}

RUDIMENTS_INLINE bool file::lockRemainder(short type, off_t start) {
	return lock(F_SETLKW,type,SEEK_SET,start,0);
}

RUDIMENTS_INLINE bool file::checkLockRemainder(short type,
						off_t start, flock *retlck) {
	return checkLock(type,SEEK_SET,start,0,retlck);
}

RUDIMENTS_INLINE bool file::unlockRemainder(off_t start) {
	return unlock(SEEK_SET,start,0);
}

RUDIMENTS_INLINE bool file::tryLockRemainderFromCurrent(short type) {
	return tryLockRemainderFromCurrent(type,0);
}

RUDIMENTS_INLINE bool file::tryLockRemainderFromCurrent(short type,
								off_t start) {
	return lock(F_SETLK,type,SEEK_CUR,start,0);
}

RUDIMENTS_INLINE bool file::lockRemainderFromCurrent(short type) {
	return lockRemainderFromCurrent(type,0);
}

RUDIMENTS_INLINE bool file::lockRemainderFromCurrent(short type, off_t start) {
	return lock(F_SETLKW,type,SEEK_CUR,start,0);
}

RUDIMENTS_INLINE bool file::checkLockRemainderFromCurrent(short type,
								flock *retlck) {
	return checkLockRemainderFromCurrent(type,0,retlck);
}

RUDIMENTS_INLINE bool file::checkLockRemainderFromCurrent(short type,
						off_t start, flock *retlck) {
	return checkLock(type,SEEK_CUR,start,0,retlck);
}

RUDIMENTS_INLINE bool file::unlockRemainderFromCurrent() {
	return unlockRemainderFromCurrent(0);
}

RUDIMENTS_INLINE bool file::unlockRemainderFromCurrent(off_t start) {
	return unlock(SEEK_CUR,start,0);
}

RUDIMENTS_INLINE bool file::tryLockRemainderFromEnd(short type) {
	return tryLockRemainderFromEnd(type,0);
}

RUDIMENTS_INLINE bool file::tryLockRemainderFromEnd(short type, off_t start) {
	return lock(F_SETLK,type,SEEK_END,start,0);
}

RUDIMENTS_INLINE bool file::lockRemainderFromEnd(short type) {
	return lockRemainderFromEnd(type,0);
}

RUDIMENTS_INLINE bool file::lockRemainderFromEnd(short type, off_t start) {
	return lock(F_SETLKW,type,SEEK_END,start,0);
}

RUDIMENTS_INLINE bool file::checkLockRemainderFromEnd(short type,
							flock *retlck) {
	return checkLockRemainderFromEnd(type,0,retlck);
}

RUDIMENTS_INLINE bool file::checkLockRemainderFromEnd(short type, off_t start,
								flock *retlck) {
	return checkLock(type,SEEK_END,start,0,retlck);
}

RUDIMENTS_INLINE bool file::truncate(const char *filename) {
	return !::truncate(filename,0);
}

RUDIMENTS_INLINE bool file::truncate(const char *filename, off_t length) {
	return !::truncate(filename,length);
}

RUDIMENTS_INLINE bool file::truncate() {
	return !::ftruncate(fd,0);
}

RUDIMENTS_INLINE bool file::truncate(off_t length) {
	return !::ftruncate(fd,length);
}

RUDIMENTS_INLINE bool file::unlockRemainderFromEnd() {
	return unlockRemainderFromEnd(0);
}

RUDIMENTS_INLINE bool file::unlockRemainderFromEnd(off_t start) {
	return unlock(SEEK_END,start,0);
}

RUDIMENTS_INLINE off_t file::setPositionRelativeToBeginning(off_t offset) {
	return lseek(fd,offset,SEEK_SET);
}

RUDIMENTS_INLINE off_t file::setPositionRelativeToCurrent(off_t offset) {
	return lseek(fd,offset,SEEK_CUR);
}

RUDIMENTS_INLINE off_t file::setPositionRelativeToEnd(off_t offset) {
	return lseek(fd,offset,SEEK_END);
}

RUDIMENTS_INLINE off_t file::getCurrentPosition() {
	return lseek(fd,0,SEEK_CUR);
}

RUDIMENTS_INLINE bool file::exists(const char *filename) {
	struct stat	st;
	return !stat(filename,&st);
}

RUDIMENTS_INLINE bool file::getCurrentProperties() {
	return (fstat(fd,&st)!=-1);
}

#define STAT(filename,out,member) \
	struct stat st; \
	if (stat(filename,&st)==-1) { \
		return false; \
	} \
	*out=st.member; \
	return true;

#define FSTAT(fd,out,member) \
	struct stat st; \
	if (fstat(fd,&st)==-1) { \
		return false; \
	} \
	*out=st.member; \
	return true;

RUDIMENTS_INLINE bool file::getPermissions(const char *filename, mode_t *mode) {
	STAT(filename,mode,st_mode)
}

RUDIMENTS_INLINE bool file::getPermissions(int fd, mode_t *mode) {
	FSTAT(fd,mode,st_mode)
}

RUDIMENTS_INLINE bool file::getOwnerUserId(const char *filename, uid_t *uid) {
	STAT(filename,uid,st_uid)
}

RUDIMENTS_INLINE bool file::getOwnerUserId(int fd, uid_t *uid) {
	FSTAT(fd,uid,st_uid)
}

RUDIMENTS_INLINE bool file::getOwnerGroupId(const char *filename, gid_t *gid) {
	STAT(filename,gid,st_gid)
}

RUDIMENTS_INLINE bool file::getOwnerGroupId(int fd, gid_t *gid) {
	FSTAT(fd,gid,st_gid)
}

RUDIMENTS_INLINE bool file::getSize(const char *filename, off_t *size) {
	STAT(filename,size,st_size)
}

RUDIMENTS_INLINE bool file::getSize(int fd, off_t *size) {
	FSTAT(fd,size,st_size)
}

RUDIMENTS_INLINE bool file::getBlockSize(const char *filename,
							blksize_t *size) {
#ifdef HAVE_BLKSIZE_T
	STAT(filename,size,st_blksize)
#else
	*size=-1;
	return true;
#endif
}

RUDIMENTS_INLINE bool file::getBlockSize(int fd, blksize_t *size) {
#ifdef HAVE_BLKSIZE_T
	FSTAT(fd,size,st_blksize)
#else
	*size=-1;
	return true;
#endif
}


RUDIMENTS_INLINE bool file::getBlockCount(const char *filename,
							blkcnt_t *blocks) {
#ifdef HAVE_BLKCNT_T
	STAT(filename,blocks,st_blocks)
#else
	*blocks=-1;
	return true;
#endif
}

RUDIMENTS_INLINE bool file::getBlockCount(int fd, blkcnt_t *blocks) {
#ifdef HAVE_BLKCNT_T
	FSTAT(fd,blocks,st_blocks)
#else
	*blocks=-1;
	return true;
#endif
}

#ifndef HAVE_S_ISSOCK
	#define S_ISSOCK(m) ((m&040000==040000)?1:0)
#endif

RUDIMENTS_INLINE int file::isSocket(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isSocket(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isSymbolicLink(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isSymbolicLink(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isRegularFile(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISREG(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isRegularFile(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISREG(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isBlockDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isBlockDevice(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isDirectory(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isDirectory(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isCharacterDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isCharacterDevice(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isFifo(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}

RUDIMENTS_INLINE int file::isFifo(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}


RUDIMENTS_INLINE bool file::getLastAccessTime(const char *filename,
								time_t *atime) {
	STAT(filename,atime,st_atime)
}

RUDIMENTS_INLINE bool file::getLastAccessTime(int fd, time_t *atime) {
	FSTAT(fd,atime,st_atime)
}

RUDIMENTS_INLINE bool file::getLastModificationTime(const char *filename,
								time_t *mtime) {
	STAT(filename,mtime,st_mtime)
}

RUDIMENTS_INLINE bool file::getLastModificationTime(int fd, time_t *mtime) {
	FSTAT(fd,mtime,st_mtime)
}

RUDIMENTS_INLINE bool file::getLastChangeTime(const char *filename,
								time_t *ctime) {
	STAT(filename,ctime,st_ctime)
}

RUDIMENTS_INLINE bool file::getLastChangeTime(int fd, time_t *ctime) {
	FSTAT(fd,ctime,st_ctime)
}


RUDIMENTS_INLINE bool file::getDevice(const char *filename, dev_t *dev) {
	STAT(filename,dev,st_dev)
}

RUDIMENTS_INLINE bool file::getDevice(int fd, dev_t *dev) {
	FSTAT(fd,dev,st_dev)
}

RUDIMENTS_INLINE bool file::getDeviceType(const char *filename,
							dev_t *devtype) {
	STAT(filename,devtype,st_rdev)
}

RUDIMENTS_INLINE bool file::getDeviceType(int fd, dev_t *devtype) {
	FSTAT(fd,devtype,st_rdev)
}

RUDIMENTS_INLINE bool file::getInode(const char *filename, ino_t *inode) {
	STAT(filename,inode,st_ino)
}

RUDIMENTS_INLINE bool file::getInode(int fd, ino_t *inode) {
	FSTAT(fd,inode,st_ino)
}

RUDIMENTS_INLINE bool file::getNumberOfHardLinks(const char *filename,
							nlink_t *nlink) {
	STAT(filename,nlink,st_nlink)
}

RUDIMENTS_INLINE bool file::getNumberOfHardLinks(int fd, nlink_t *nlink) {
	FSTAT(fd,nlink,st_nlink)
}

RUDIMENTS_INLINE mode_t file::getPermissions() const {
	return st.st_mode;
}

RUDIMENTS_INLINE uid_t file::getOwnerUserId() const {
	return st.st_uid;
}

RUDIMENTS_INLINE gid_t file::getOwnerGroupId() const {
	return st.st_gid;
}

RUDIMENTS_INLINE off_t file::getSize() const {
	return st.st_size;
}


RUDIMENTS_INLINE blksize_t file::getBlockSize() const {
#ifdef HAVE_BLKSIZE_T
	return st.st_blksize;
#else
	return -1;
#endif
}

RUDIMENTS_INLINE blkcnt_t file::getBlockCount() const {
#ifdef HAVE_BLKCNT_T
	return st.st_blocks;
#else
	return -1;
#endif
}

RUDIMENTS_INLINE int file::isSocket() const {
	return S_ISSOCK(st.st_mode);
}

RUDIMENTS_INLINE int file::isSymbolicLink() const {
	return S_ISLNK(st.st_mode);
}

RUDIMENTS_INLINE int file::isRegularFile() const {
	return S_ISREG(st.st_mode);
}

RUDIMENTS_INLINE int file::isBlockDevice() const {
	return S_ISBLK(st.st_mode);
}

RUDIMENTS_INLINE int file::isDirectory() const {
	return S_ISDIR(st.st_mode);
}

RUDIMENTS_INLINE int file::isCharacterDevice() const {
	return S_ISCHR(st.st_mode);
}

RUDIMENTS_INLINE int file::isFifo() const {
	return S_ISFIFO(st.st_mode);
}

RUDIMENTS_INLINE time_t file::getLastAccessTime() const {
	return st.st_atime;
}

RUDIMENTS_INLINE time_t file::getLastModificationTime() const {
	return st.st_mtime;
}

RUDIMENTS_INLINE time_t file::getLastChangeTime() const {
	return st.st_ctime;
}


RUDIMENTS_INLINE dev_t file::getDevice() const {
	return st.st_dev;
}

RUDIMENTS_INLINE dev_t file::getDeviceType() const {
	return st.st_rdev;
}

RUDIMENTS_INLINE ino_t file::getInode() const {
	return st.st_ino;
}

RUDIMENTS_INLINE nlink_t file::getNumberOfHardLinks() const {
	return st.st_nlink;
}

RUDIMENTS_INLINE struct stat *file::getStat() {
	return &st;
}

RUDIMENTS_INLINE void file::getCurrentPropertiesOnOpen() {
	getcurrentpropertiesonopen=true;
}

RUDIMENTS_INLINE void file::dontGetCurrentPropertiesOnOpen() {
	getcurrentpropertiesonopen=false;
}
