// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

inline file::file() : filedescriptor() {
	success=0;
}

inline file::file(int fd) : filedescriptor(fd) {
	success=0;
}

inline int file::open(const char *name, int flags) {
	return ((fd=::open(name,flags))!=-1 && getCurrentProperties());
}

inline int file::open(const char *name, int flags, mode_t perms) {
	return ((fd=::open(name,flags,perms))!=-1 && getCurrentProperties());
}

inline int file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

size_t	file::create(const char *name, mode_t perms, unsigned short number) {
	return create(name,perms,(void *)&number,sizeof(unsigned short));
}

size_t	file::create(const char *name, mode_t perms, unsigned long number) {
	return create(name,perms,(void *)&number,sizeof(unsigned long));
}

size_t	file::create(const char *name, mode_t perms, float number) {
	return create(name,perms,(void *)&number,sizeof(float));
}

size_t	file::create(const char *name, mode_t perms, double number) {
	return create(name,perms,(void *)&number,sizeof(double));
}

size_t	file::create(const char *name, mode_t perms, char number) {
	return create(name,perms,(void *)&number,sizeof(char));
}

size_t	file::create(const char *name, mode_t perms, const char *string) {
	return create(name,perms,(void *)string,strlen(string));
}

size_t	file::create(const char *name, mode_t perms,
					const char *string, size_t size) {
	return create(name,perms,(void *)string,size);
}

int file::getCurrentProperties() {
printf("getCurrentProperties()\n");
	return (success=(fstat(fd,&st)!=-1));
}

#define STAT(filename,out,member) \
	struct stat st; \
	if (stat(filename,&st)==-1) { \
		return -1; \
	} \
	*out=st.member; \
	return 1;

#define FSTAT(fd,out,member) \
	struct stat st; \
	if (fstat(fd,&st)==-1) { \
		return -1; \
	} \
	*out=st.member; \
	return 1;

#define RETURN_PROPERTY(out,member) \
	if (!success) { \
		return -1; \
	} \
	*out=st.member; \
	return 1;

inline int file::getPermissions(const char *filename, mode_t *mode) {
	STAT(filename,mode,st_mode)
}

inline int file::getPermissions(int fd, mode_t *mode) {
	FSTAT(fd,mode,st_mode)
}

inline int file::getOwnerUserId(const char *filename, uid_t *uid) {
	STAT(filename,uid,st_uid)
}

inline int file::getOwnerUserId(int fd, uid_t *uid) {
	FSTAT(fd,uid,st_uid)
}

inline int file::getOwnerGroupId(const char *filename, gid_t *gid) {
	STAT(filename,gid,st_gid)
}

inline int file::getOwnerGroupId(int fd, gid_t *gid) {
	FSTAT(fd,gid,st_gid)
}

inline int file::getSize(const char *filename, off_t *size) {
	STAT(filename,size,st_size)
}

inline int file::getSize(int fd, off_t *size) {
	FSTAT(fd,size,st_size)
}

inline int file::getBlockCount(const char *filename, blkcnt_t *blocks) {
	STAT(filename,blocks,st_blocks)
}

inline int file::getBlockCount(int fd, blkcnt_t *blocks) {
	FSTAT(fd,blocks,st_blocks)
}

inline int file::isSocket(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

inline int file::isSocket(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

inline int file::isSymbolicLink(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

inline int file::isSymbolicLink(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

inline int file::isRegularFile(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISREG(st.st_mode):-1;
}

inline int file::isRegularFile(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISREG(st.st_mode):-1;
}

inline int file::isBlockDevice(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

inline int file::isBlockDevice(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

inline int file::isDirectory(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

inline int file::isDirectory(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

inline int file::isCharacterDevice(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

inline int file::isCharacterDevice(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

inline int file::isFifo(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}

inline int file::isFifo(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}


inline int file::getLastAccessTime(const char *filename, time_t *atime) {
	STAT(filename,atime,st_atime)
}

inline int file::getLastAccessTime(int fd, time_t *atime) {
	FSTAT(fd,atime,st_atime)
}

inline int file::getLastModificationTime(const char *filename, time_t *mtime) {
	STAT(filename,mtime,st_mtime)
}

inline int file::getLastModificationTime(int fd, time_t *mtime) {
	FSTAT(fd,mtime,st_mtime)
}

inline int file::getLastChangeTime(const char *filename, time_t *ctime) {
	STAT(filename,ctime,st_ctime)
}

inline int file::getLastChangeTime(int fd, time_t *ctime) {
	FSTAT(fd,ctime,st_ctime)
}


inline int file::getDevice(const char *filename, dev_t *dev) {
	STAT(filename,dev,st_dev)
}

inline int file::getDevice(int fd, dev_t *dev) {
	FSTAT(fd,dev,st_dev)
}

inline int file::getDeviceType(const char *filename, dev_t *devtype) {
	STAT(filename,devtype,st_rdev)
}

inline int file::getDeviceType(int fd, dev_t *devtype) {
	FSTAT(fd,devtype,st_rdev)
}

inline int file::getInode(const char *filename, ino_t *inode) {
	STAT(filename,inode,st_ino)
}

inline int file::getInode(int fd, ino_t *inode) {
	FSTAT(fd,inode,st_ino)
}

inline int file::getNumberOfHardLinks(const char *filename, nlink_t *nlink) {
	STAT(filename,nlink,st_nlink);
}

inline int file::getNumberOfHardLinks(int fd, nlink_t *nlink) {
	FSTAT(fd,nlink,st_nlink);
}

inline int file::getPermissions(mode_t *mode) const {
	RETURN_PROPERTY(mode,st_mode);
}

inline int file::getOwnerUserId(uid_t *uid) const {
	RETURN_PROPERTY(uid,st_uid);
}

inline int file::getOwnerGroupId(gid_t *gid) const {
	RETURN_PROPERTY(gid,st_gid);
}

inline int file::getSize(off_t *size) const {
	RETURN_PROPERTY(size,st_size);
}

inline int file::getBlockCount(blkcnt_t *blocks) const {
	RETURN_PROPERTY(blocks,st_blocks);
}

inline int file::isSocket() const {
	return (success)?S_ISSOCK(st.st_mode):-1;
}

inline int file::isSymbolicLink() const {
	return (success)?S_ISLNK(st.st_mode):-1;
}

inline int file::isRegularFile() const {
	return (success)?S_ISREG(st.st_mode):-1;
}

inline int file::isBlockDevice() const {
	return (success)?S_ISBLK(st.st_mode):-1;
}

inline int file::isDirectory() const {
	return (success)?S_ISDIR(st.st_mode):-1;
}

inline int file::isCharacterDevice() const {
	return (success)?S_ISCHR(st.st_mode):-1;
}

inline int file::isFifo() const {
	return (success)?S_ISFIFO(st.st_mode):-1;
}

inline int file::getLastAccessTime(time_t *atime) const {
	RETURN_PROPERTY(atime,st_atime);
}

inline int file::getLastModificationTime(time_t *mtime) const {
	RETURN_PROPERTY(mtime,st_mtime);
}

inline int file::getLastChangeTime(time_t *ctime) const {
	RETURN_PROPERTY(ctime,st_ctime);
}


inline int file::getDevice(dev_t *dev) const {
	RETURN_PROPERTY(dev,st_dev);
}

inline int file::getDeviceType(dev_t *devtype) const {
	RETURN_PROPERTY(devtype,st_rdev);
}

inline int file::getInode(ino_t *inode) const {
	RETURN_PROPERTY(inode,st_ino);
}

inline int file::getNumberOfHardLinks(nlink_t *nlink) const {
	RETURN_PROPERTY(nlink,st_nlink);
}

struct stat *file::getStat() {
	return &st;
}
