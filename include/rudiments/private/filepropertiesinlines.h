// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#include <unistd.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

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

inline int fileproperties::getPermissions(const char *filename,
							mode_t *mode) {
	STAT(filename,mode,st_mode)
}

inline int fileproperties::getPermissions(int fd, mode_t *mode) {
	FSTAT(fd,mode,st_mode)
}

inline int fileproperties::getOwnerUserId(const char *filename,
							uid_t *uid) {
	STAT(filename,uid,st_uid)
}

inline int fileproperties::getOwnerUserId(int fd, uid_t *uid) {
	FSTAT(fd,uid,st_uid)
}

inline int fileproperties::getOwnerGroupId(const char *filename,
							gid_t *gid) {
	STAT(filename,gid,st_gid)
}

inline int fileproperties::getOwnerGroupId(int fd, gid_t *gid) {
	FSTAT(fd,gid,st_gid)
}

inline int fileproperties::getSize(const char *filename, off_t *size) {
	STAT(filename,size,st_size)
}

inline int fileproperties::getSize(int fd, off_t *size) {
	FSTAT(fd,size,st_size)
}

inline int fileproperties::getBlockCount(const char *filename,
							blkcnt_t *blocks) {
	STAT(filename,blocks,st_blocks)
}

inline int fileproperties::getBlockCount(int fd, blkcnt_t *blocks) {
	FSTAT(fd,blocks,st_blocks)
}

inline int fileproperties::isSocket(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

inline int fileproperties::isSocket(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISSOCK(st.st_mode):-1;
}

inline int fileproperties::isSymbolicLink(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

inline int fileproperties::isSymbolicLink(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISLNK(st.st_mode):-1;
}

inline int fileproperties::isRegularFile(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISREG(st.st_mode):-1;
}

inline int fileproperties::isRegularFile(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISREG(st.st_mode):-1;
}

inline int fileproperties::isBlockDevice(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

inline int fileproperties::isBlockDevice(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISBLK(st.st_mode):-1;
}

inline int fileproperties::isDirectory(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

inline int fileproperties::isDirectory(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISDIR(st.st_mode):-1;
}

inline int fileproperties::isCharacterDevice(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

inline int fileproperties::isCharacterDevice(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISCHR(st.st_mode):-1;
}

inline int fileproperties::isFifo(const char *filename) {
	struct	stat	st;
	return (stat(filename,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}

inline int fileproperties::isFifo(int fd) {
	struct	stat	st;
	return (fstat(fd,&st)>-1)?S_ISFIFO(st.st_mode):-1;
}


inline int fileproperties::getLastAccessTime(const char *filename,
							time_t *atime) {
	STAT(filename,atime,st_atime)
}

inline int fileproperties::getLastAccessTime(int fd, time_t *atime) {
	FSTAT(fd,atime,st_atime)
}

inline int fileproperties::getLastModificationTime(const char *filename,
							time_t *mtime) {
	STAT(filename,mtime,st_mtime)
}

inline int fileproperties::getLastModificationTime(int fd, time_t *mtime) {
	FSTAT(fd,mtime,st_mtime)
}

inline int fileproperties::getLastChangeTime(const char *filename,
							time_t *ctime) {
	STAT(filename,ctime,st_ctime)
}

inline int fileproperties::getLastChangeTime(int fd, time_t *ctime) {
	FSTAT(fd,ctime,st_ctime)
}


inline int fileproperties::getDevice(const char *filename, dev_t *dev) {
	STAT(filename,dev,st_dev)
}

inline int fileproperties::getDevice(int fd, dev_t *dev) {
	FSTAT(fd,dev,st_dev)
}

inline int fileproperties::getDeviceType(const char *filename,
							dev_t *devtype) {
	STAT(filename,devtype,st_rdev)
}

inline int fileproperties::getDeviceType(int fd, dev_t *devtype) {
	FSTAT(fd,devtype,st_rdev)
}

inline int fileproperties::getInode(const char *filename, ino_t *inode) {
	STAT(filename,inode,st_ino)
}

inline int fileproperties::getInode(int fd, ino_t *inode) {
	FSTAT(fd,inode,st_ino)
}

inline int fileproperties::getNumberOfHardLinks(const char *filename,
							nlink_t *nlink) {
	STAT(filename,nlink,st_nlink);
}

inline int fileproperties::getNumberOfHardLinks(int fd, nlink_t *nlink) {
	FSTAT(fd,nlink,st_nlink);
}

inline fileproperties::fileproperties() {
	success=0;
}

inline int fileproperties::getFileProperties(const char *filename) {
	return (success=(stat(filename,&st)!=-1));
}

inline int fileproperties::getFileProperties(int fd) {
	return (success=(fstat(fd,&st)!=-1));
}

inline int fileproperties::getPermissions(mode_t *mode) const {
	RETURN_PROPERTY(mode,st_mode);
}

inline int fileproperties::getOwnerUserId(uid_t *uid) const {
	RETURN_PROPERTY(uid,st_uid);
}

inline int fileproperties::getOwnerGroupId(gid_t *gid) const {
	RETURN_PROPERTY(gid,st_gid);
}

inline int fileproperties::getSize(off_t *size) const {
	RETURN_PROPERTY(size,st_size);
}

inline int fileproperties::getBlockCount(blkcnt_t *blocks) const {
	RETURN_PROPERTY(blocks,st_blocks);
}

inline int fileproperties::isSocket() const {
	return (success)?S_ISSOCK(st.st_mode):-1;
}

inline int fileproperties::isSymbolicLink() const {
	return (success)?S_ISLNK(st.st_mode):-1;
}

inline int fileproperties::isRegularFile() const {
	return (success)?S_ISREG(st.st_mode):-1;
}

inline int fileproperties::isBlockDevice() const {
	return (success)?S_ISBLK(st.st_mode):-1;
}

inline int fileproperties::isDirectory() const {
	return (success)?S_ISDIR(st.st_mode):-1;
}

inline int fileproperties::isCharacterDevice() const {
	return (success)?S_ISCHR(st.st_mode):-1;
}

inline int fileproperties::isFifo() const {
	return (success)?S_ISFIFO(st.st_mode):-1;
}

inline int fileproperties::getLastAccessTime(time_t *atime) const {
	RETURN_PROPERTY(atime,st_atime);
}

inline int fileproperties::getLastModificationTime(time_t *mtime) const {
	RETURN_PROPERTY(mtime,st_mtime);
}

inline int fileproperties::getLastChangeTime(time_t *ctime) const {
	RETURN_PROPERTY(ctime,st_ctime);
}


inline int fileproperties::getDevice(dev_t *dev) const {
	RETURN_PROPERTY(dev,st_dev);
}

inline int fileproperties::getDeviceType(dev_t *devtype) const {
	RETURN_PROPERTY(devtype,st_rdev);
}

inline int fileproperties::getInode(ino_t *inode) const {
	RETURN_PROPERTY(inode,st_ino);
}

inline int fileproperties::getNumberOfHardLinks(nlink_t *nlink) const {
	RETURN_PROPERTY(nlink,st_nlink);
}

struct stat *fileproperties::getStat() {
	return &st;
}
