// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <string.h>

#include <unistd.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

INLINE file::file() : filedescriptor() {
}

INLINE file::file(int fd) : filedescriptor(fd) {
}

INLINE int file::open(const char *name, int flags) {
	return ((fd=::open(name,flags))!=-1 && getCurrentProperties());
}

INLINE int file::open(const char *name, int flags, mode_t perms) {
	return ((fd=::open(name,flags,perms))!=-1 && getCurrentProperties());
}

INLINE int file::create(const char *name, mode_t perms) {
	return open(name,O_CREAT|O_TRUNC|O_RDWR,perms);
}

INLINE size_t	file::create(const char *name, mode_t perms,
						unsigned short number) {
	return create(name,perms,(void *)&number,sizeof(unsigned short));
}

INLINE size_t	file::create(const char *name, mode_t perms,
						unsigned long number) {
	return create(name,perms,(void *)&number,sizeof(unsigned long));
}

INLINE size_t	file::create(const char *name, mode_t perms, float number) {
	return create(name,perms,(void *)&number,sizeof(float));
}

INLINE size_t	file::create(const char *name, mode_t perms, double number) {
	return create(name,perms,(void *)&number,sizeof(double));
}

INLINE size_t	file::create(const char *name, mode_t perms, char number) {
	return create(name,perms,(void *)&number,sizeof(char));
}

INLINE size_t	file::create(const char *name, mode_t perms,
						const char *string) {
	return create(name,perms,(void *)string,strlen(string));
}

INLINE size_t	file::create(const char *name, mode_t perms,
					const char *string, size_t size) {
	return create(name,perms,(void *)string,size);
}

INLINE off_t file::setPositionRelativeToBeginning(off_t offset) {
	return lseek(fd,offset,SEEK_SET);
}

INLINE off_t file::setPositionRelativeToCurrent(off_t offset) {
	return lseek(fd,offset,SEEK_CUR);
}

INLINE off_t file::setPositionRelativeToEnd(off_t offset) {
	return lseek(fd,offset,SEEK_END);
}

INLINE off_t file::getCurrentPosition() {
	return lseek(fd,0,SEEK_CUR);
}

INLINE int file::getCurrentProperties() {
	return (fstat(fd,&st)!=-1);
}

#define STAT(filename,out,member) \
	struct stat st; \
	if (stat(filename,&st)==-1) { \
		return 0; \
	} \
	*out=st.member; \
	return 1;

#define FSTAT(fd,out,member) \
	struct stat st; \
	if (fstat(fd,&st)==-1) { \
		return 0; \
	} \
	*out=st.member; \
	return 1;

INLINE int file::getPermissions(const char *filename, mode_t *mode) {
	STAT(filename,mode,st_mode)
}

INLINE int file::getPermissions(int fd, mode_t *mode) {
	FSTAT(fd,mode,st_mode)
}

INLINE int file::getOwnerUserId(const char *filename, uid_t *uid) {
	STAT(filename,uid,st_uid)
}

INLINE int file::getOwnerUserId(int fd, uid_t *uid) {
	FSTAT(fd,uid,st_uid)
}

INLINE int file::getOwnerGroupId(const char *filename, gid_t *gid) {
	STAT(filename,gid,st_gid)
}

INLINE int file::getOwnerGroupId(int fd, gid_t *gid) {
	FSTAT(fd,gid,st_gid)
}

INLINE int file::getSize(const char *filename, off_t *size) {
	STAT(filename,size,st_size)
}

INLINE int file::getSize(int fd, off_t *size) {
	FSTAT(fd,size,st_size)
}

INLINE int file::getBlockCount(const char *filename, blkcnt_t *blocks) {
	STAT(filename,blocks,st_blocks)
}

INLINE int file::getBlockCount(int fd, blkcnt_t *blocks) {
	FSTAT(fd,blocks,st_blocks)
}

INLINE int file::isSocket(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

INLINE int file::isSocket(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

INLINE int file::isSymbolicLink(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

INLINE int file::isSymbolicLink(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

INLINE int file::isRegularFile(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISREG(st.st_mode):-1;
}

INLINE int file::isRegularFile(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISREG(st.st_mode):-1;
}

INLINE int file::isBlockDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

INLINE int file::isBlockDevice(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

INLINE int file::isDirectory(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

INLINE int file::isDirectory(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

INLINE int file::isCharacterDevice(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

INLINE int file::isCharacterDevice(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

INLINE int file::isFifo(const char *filename) {
	struct stat	st;
	return (stat(filename,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}

INLINE int file::isFifo(int fd) {
	struct stat	st;
	return (fstat(fd,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}


INLINE int file::getLastAccessTime(const char *filename, time_t *atime) {
	STAT(filename,atime,st_atime)
}

INLINE int file::getLastAccessTime(int fd, time_t *atime) {
	FSTAT(fd,atime,st_atime)
}

INLINE int file::getLastModificationTime(const char *filename, time_t *mtime) {
	STAT(filename,mtime,st_mtime)
}

INLINE int file::getLastModificationTime(int fd, time_t *mtime) {
	FSTAT(fd,mtime,st_mtime)
}

INLINE int file::getLastChangeTime(const char *filename, time_t *ctime) {
	STAT(filename,ctime,st_ctime)
}

INLINE int file::getLastChangeTime(int fd, time_t *ctime) {
	FSTAT(fd,ctime,st_ctime)
}


INLINE int file::getDevice(const char *filename, dev_t *dev) {
	STAT(filename,dev,st_dev)
}

INLINE int file::getDevice(int fd, dev_t *dev) {
	FSTAT(fd,dev,st_dev)
}

INLINE int file::getDeviceType(const char *filename, dev_t *devtype) {
	STAT(filename,devtype,st_rdev)
}

INLINE int file::getDeviceType(int fd, dev_t *devtype) {
	FSTAT(fd,devtype,st_rdev)
}

INLINE int file::getInode(const char *filename, ino_t *inode) {
	STAT(filename,inode,st_ino)
}

INLINE int file::getInode(int fd, ino_t *inode) {
	FSTAT(fd,inode,st_ino)
}

INLINE int file::getNumberOfHardLinks(const char *filename, nlink_t *nlink) {
	STAT(filename,nlink,st_nlink)
}

INLINE int file::getNumberOfHardLinks(int fd, nlink_t *nlink) {
	FSTAT(fd,nlink,st_nlink)
}

INLINE mode_t file::getPermissions() const {
	return st.st_mode;
}

INLINE uid_t file::getOwnerUserId() const {
	return st.st_uid;
}

INLINE gid_t file::getOwnerGroupId() const {
	return st.st_gid;
}

INLINE off_t file::getSize() const {
	return st.st_size;
}

INLINE blkcnt_t file::getBlockCount() const {
	return st.st_blocks;
}

INLINE int file::isSocket() const {
	return S_ISSOCK(st.st_mode);
}

INLINE int file::isSymbolicLink() const {
	return S_ISLNK(st.st_mode);
}

INLINE int file::isRegularFile() const {
	return S_ISREG(st.st_mode);
}

INLINE int file::isBlockDevice() const {
	return S_ISBLK(st.st_mode);
}

INLINE int file::isDirectory() const {
	return S_ISDIR(st.st_mode);
}

INLINE int file::isCharacterDevice() const {
	return S_ISCHR(st.st_mode);
}

INLINE int file::isFifo() const {
	return S_ISFIFO(st.st_mode);
}

INLINE time_t file::getLastAccessTime() const {
	return st.st_atime;
}

INLINE time_t file::getLastModificationTime() const {
	return st.st_mtime;
}

INLINE time_t file::getLastChangeTime() const {
	return st.st_ctime;
}


INLINE dev_t file::getDevice() const {
	return st.st_dev;
}

INLINE dev_t file::getDeviceType() const {
	return st.st_rdev;
}

INLINE ino_t file::getInode() const {
	return st.st_ino;
}

INLINE nlink_t file::getNumberOfHardLinks() const {
	return st.st_nlink;
}

INLINE struct stat *file::getStat() {
	return &st;
}
