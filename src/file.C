// Copyright (c) 2002 David Muse
// See the COPYING file for more information

#include <rudiments/file.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#ifdef HAVE_STRINGS_H
	#include <strings.h>
#endif
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

file::file() : filedescriptor() {
	getcurrentpropertiesonopen=true;
}

file::file(int fd) : filedescriptor(fd) {
}

bool file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

ssize_t file::create(const char *name, mode_t perms,
						unsigned short number) {
	return create(name,perms,(void *)&number,sizeof(unsigned short));
}

ssize_t file::create(const char *name, mode_t perms, unsigned long number) {
	return create(name,perms,(void *)&number,sizeof(unsigned long));
}

ssize_t file::create(const char *name, mode_t perms, float number) {
	return create(name,perms,(void *)&number,sizeof(float));
}

ssize_t file::create(const char *name, mode_t perms, double number) {
	return create(name,perms,(void *)&number,sizeof(double));
}

ssize_t file::create(const char *name, mode_t perms, char number) {
	return create(name,perms,(void *)&number,sizeof(char));
}

ssize_t file::create(const char *name, mode_t perms,
					const char *string, size_t size) {
	return create(name,perms,(void *)string,size);
}

ssize_t file::create(const char *name, mode_t perms, const char *string) {
	return create(name,perms,(void *)string,strlen(string));
}

ssize_t file::create(const char *name, mode_t permissions,
					const void *data, size_t size) {
	size_t	retval;
	if (((fd=::open(name,O_CREAT|O_TRUNC|O_RDWR,permissions))!=-1) &&
		((retval=write(data,size))==size) &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true)) {
		return retval;
	}
	close();
	return -1;
}

bool file::open(const char *name, int flags) {
	return ((fd=::open(name,flags))!=-1 &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true));
}

bool file::open(const char *name, int flags, mode_t perms) {
	return ((fd=::open(name,flags,perms))!=-1 &&
		((getcurrentpropertiesonopen)?getCurrentProperties():true));
}

char *file::getContents() {
	off_t	size=(fd>-1)?st.st_size:0;
	char	*contents=new char[size+1];
	contents[size]=(char)NULL;
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


bool file::tryLockFile(short type) {
	return tryLockRegion(type,0,0);
}

bool file::lockFile(short type) {
	return lockRegion(type,0,0);
}

bool file::checkLockFile(short type, struct flock *retlck) {
	return checkLockRegion(type,0,0,retlck);
}

bool file::unlockFile(short type) {
	return unlockRegion(0,0);
}

bool file::tryLockRegion(short type, off_t start, off_t len) {
	return lock(F_SETLK,type,SEEK_SET,start,len);
}

bool file::lockRegion(short type, off_t start, off_t len) {
	return lock(F_SETLKW,type,SEEK_SET,start,len);
}

bool file::checkLockRegion(short type, off_t start, off_t len,
						struct flock *retlck) {
	return checkLock(type,SEEK_SET,start,len,retlck);
}

bool file::unlockRegion(off_t start, off_t len) {
	return unlock(SEEK_SET,start,len);
}

bool file::tryLockFromCurrent(short type, off_t len) {
	return tryLockFromCurrent(type,0,len);
}

bool file::tryLockFromCurrent(short type, off_t start, off_t len) {
	return lock(F_SETLK,type,SEEK_CUR,start,len);
}

bool file::lockFromCurrent(short type, off_t len) {
	return lockFromCurrent(type,0,len);
}

bool file::lockFromCurrent(short type, off_t start, off_t len) {
	return lock(F_SETLKW,type,SEEK_CUR,start,len);
}

bool file::checkLockFromCurrent(short type, off_t len, struct flock *retlck) {
	return checkLockFromCurrent(type,0,len,retlck);
}

bool file::checkLockFromCurrent(short type, off_t start, off_t len,
							struct flock *retlck) {
	return checkLock(type,SEEK_CUR,start,len,retlck);
}

bool file::unlockFromCurrent(off_t len) {
	return unlockFromCurrent(0,len);
}

bool file::unlockFromCurrent(off_t start, off_t len) {
	return unlock(SEEK_CUR,start,len);
}

bool file::tryLockFromEnd(short type, off_t len) {
	return tryLockFromEnd(type,0,len);
}

bool file::tryLockFromEnd(short type, off_t start, off_t len) {
	return lock(F_SETLK,type,SEEK_END,start,len);
}

bool file::lockFromEnd(short type, off_t len) {
	return lockFromEnd(type,0,len);
}

bool file::lockFromEnd(short type, off_t start, off_t len) {
	return lock(F_SETLKW,type,SEEK_END,start,len);
}

bool file::checkLockFromEnd(short type, off_t len, struct flock *retlck) {
	return checkLockFromEnd(type,0,len,retlck);
}

bool file::checkLockFromEnd(short type, off_t start, off_t len,
							struct flock *retlck) {
	return checkLock(type,SEEK_END,start,len,retlck);
}

bool file::unlockFromEnd(off_t len) {
	return unlockFromEnd(0,len);
}

bool file::unlockFromEnd(off_t start, off_t len) {
	return unlock(SEEK_END,start,len);
}

bool file::tryLockRemainder(short type, off_t start) {
	return lock(F_SETLK,type,SEEK_SET,start,0);
}

bool file::lockRemainder(short type, off_t start) {
	return lock(F_SETLKW,type,SEEK_SET,start,0);
}

bool file::checkLockRemainder(short type, off_t start, struct flock *retlck) {
	return checkLock(type,SEEK_SET,start,0,retlck);
}

bool file::unlockRemainder(off_t start) {
	return unlock(SEEK_SET,start,0);
}

bool file::tryLockRemainderFromCurrent(short type) {
	return tryLockRemainderFromCurrent(type,0);
}

bool file::tryLockRemainderFromCurrent(short type, off_t start) {
	return lock(F_SETLK,type,SEEK_CUR,start,0);
}

bool file::lockRemainderFromCurrent(short type) {
	return lockRemainderFromCurrent(type,0);
}

bool file::lockRemainderFromCurrent(short type, off_t start) {
	return lock(F_SETLKW,type,SEEK_CUR,start,0);
}

bool file::checkLockRemainderFromCurrent(short type, struct flock *retlck) {
	return checkLockRemainderFromCurrent(type,0,retlck);
}

bool file::checkLockRemainderFromCurrent(short type, off_t start,
							struct flock *retlck) {
	return checkLock(type,SEEK_CUR,start,0,retlck);
}

bool file::unlockRemainderFromCurrent() {
	return unlockRemainderFromCurrent(0);
}

bool file::unlockRemainderFromCurrent(off_t start) {
	return unlock(SEEK_CUR,start,0);
}

bool file::tryLockRemainderFromEnd(short type) {
	return tryLockRemainderFromEnd(type,0);
}

bool file::tryLockRemainderFromEnd(short type, off_t start) {
	return lock(F_SETLK,type,SEEK_END,start,0);
}

bool file::lockRemainderFromEnd(short type) {
	return lockRemainderFromEnd(type,0);
}

bool file::lockRemainderFromEnd(short type, off_t start) {
	return lock(F_SETLKW,type,SEEK_END,start,0);
}

bool file::checkLockRemainderFromEnd(short type, struct flock *retlck) {
	return checkLockRemainderFromEnd(type,0,retlck);
}

bool file::checkLockRemainderFromEnd(short type, off_t start,
							struct flock *retlck) {
	return checkLock(type,SEEK_END,start,0,retlck);
}

bool file::truncate(const char *filename) {
	return !::truncate(filename,0);
}

bool file::truncate(const char *filename, off_t length) {
	return !::truncate(filename,length);
}

bool file::truncate() {
	return !::ftruncate(fd,0);
}

bool file::truncate(off_t length) {
	return !::ftruncate(fd,length);
}

bool file::unlockRemainderFromEnd() {
	return unlockRemainderFromEnd(0);
}

bool file::unlockRemainderFromEnd(off_t start) {
	return unlock(SEEK_END,start,0);
}

off_t file::setPositionRelativeToBeginning(off_t offset) {
	return lseek(fd,offset,SEEK_SET);
}

off_t file::setPositionRelativeToCurrent(off_t offset) {
	return lseek(fd,offset,SEEK_CUR);
}

off_t file::setPositionRelativeToEnd(off_t offset) {
	return lseek(fd,offset,SEEK_END);
}

off_t file::getCurrentPosition() {
	return lseek(fd,0,SEEK_CUR);
}

bool file::exists(const char *filename) {
	struct stat	st;
	return !stat(filename,&st);
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

#define FSTAT(fd,out,member) \
	struct stat st; \
	if (fstat(fd,&st)==-1) { \
		return false; \
	} \
	*out=st.member; \
	return true;

bool file::getPermissions(const char *filename, mode_t *mode) {
	STAT(filename,mode,st_mode)
}

bool file::getPermissions(int fd, mode_t *mode) {
	FSTAT(fd,mode,st_mode)
}

bool file::getOwnerUserId(const char *filename, uid_t *uid) {
	STAT(filename,uid,st_uid)
}

bool file::getOwnerUserId(int fd, uid_t *uid) {
	FSTAT(fd,uid,st_uid)
}

bool file::getOwnerGroupId(const char *filename, gid_t *gid) {
	STAT(filename,gid,st_gid)
}

bool file::getOwnerGroupId(int fd, gid_t *gid) {
	FSTAT(fd,gid,st_gid)
}

bool file::getSize(const char *filename, off_t *size) {
	STAT(filename,size,st_size)
}

bool file::getSize(int fd, off_t *size) {
	FSTAT(fd,size,st_size)
}

bool file::getBlockSize(const char *filename, blksize_t *size) {
#ifdef HAVE_BLKSIZE_T
	STAT(filename,size,st_blksize)
#else
	*size=-1;
	return true;
#endif
}

bool file::getBlockSize(int fd, blksize_t *size) {
#ifdef HAVE_BLKSIZE_T
	FSTAT(fd,size,st_blksize)
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

bool file::getBlockCount(int fd, blkcnt_t *blocks) {
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

int file::isSocket(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

int file::isSocket(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

int file::isSymbolicLink(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

int file::isSymbolicLink(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

int file::isRegularFile(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISREG(st.st_mode):-1;
}

int file::isRegularFile(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISREG(st.st_mode):-1;
}

int file::isBlockDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

int file::isBlockDevice(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

int file::isDirectory(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

int file::isDirectory(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

int file::isCharacterDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

int file::isCharacterDevice(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

int file::isFifo(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}

int file::isFifo(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}


bool file::getLastAccessTime(const char *filename, time_t *atime) {
	STAT(filename,atime,st_atime)
}

bool file::getLastAccessTime(int fd, time_t *atime) {
	FSTAT(fd,atime,st_atime)
}

bool file::getLastModificationTime(const char *filename, time_t *mtime) {
	STAT(filename,mtime,st_mtime)
}

bool file::getLastModificationTime(int fd, time_t *mtime) {
	FSTAT(fd,mtime,st_mtime)
}

bool file::getLastChangeTime(const char *filename, time_t *ctime) {
	STAT(filename,ctime,st_ctime)
}

bool file::getLastChangeTime(int fd, time_t *ctime) {
	FSTAT(fd,ctime,st_ctime)
}


bool file::getDevice(const char *filename, dev_t *dev) {
	STAT(filename,dev,st_dev)
}

bool file::getDevice(int fd, dev_t *dev) {
	FSTAT(fd,dev,st_dev)
}

bool file::getDeviceType(const char *filename, dev_t *devtype) {
	STAT(filename,devtype,st_rdev)
}

bool file::getDeviceType(int fd, dev_t *devtype) {
	FSTAT(fd,devtype,st_rdev)
}

bool file::getInode(const char *filename, ino_t *inode) {
	STAT(filename,inode,st_ino)
}

bool file::getInode(int fd, ino_t *inode) {
	FSTAT(fd,inode,st_ino)
}

bool file::getNumberOfHardLinks(const char *filename, nlink_t *nlink) {
	STAT(filename,nlink,st_nlink)
}

bool file::getNumberOfHardLinks(int fd, nlink_t *nlink) {
	FSTAT(fd,nlink,st_nlink)
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

bool file::lock(int method, short type, short whence, off_t start, off_t len) {
	struct flock	lck;
	lck.l_type=type;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	// FIXME: if a signal interrupts this, we should retry
	return !fcntl(fd,method,&lck);
}

bool file::checkLock(short type, short whence, off_t start, off_t len,
						struct flock *retlck) {
	struct flock	lck;
	lck.l_type=type;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	// FIXME: if a signal interrupts this, we should retry
	bool	retval=(!fcntl(fd,F_SETLKW,&lck));
	memcpy((void *)retlck,(void *)&lck,sizeof(struct flock));
	return retval;
}

bool file::unlock(short whence, off_t start, off_t len) {
	struct flock	lck;
	lck.l_type=F_UNLCK;
	lck.l_whence=whence;
	lck.l_start=start;
	lck.l_len=len;
	// FIXME: if a signal interrupts this, we should retry
	return !fcntl(fd,F_SETLK,&lck);
}
