// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#include <rudiments/private/rudimentsinlines.h>

RUDIMENTS_INLINE filesystem::filesystem() {
	fd=-1;
	closeflag=0;
}

RUDIMENTS_INLINE filesystem::~filesystem() {
	close();
}

RUDIMENTS_INLINE void filesystem::close() {
	if (fd>-1 && closeflag) {
		::close(fd);
		fd=-1;
	}
}

RUDIMENTS_INLINE int filesystem::initialize(const char *path) {
	close();
	closeflag=1;
	return ((fd=::open(path,O_RDONLY))!=-1 && getCurrentProperties());
}

RUDIMENTS_INLINE int filesystem::initialize(int fd) {
	close();
	closeflag=0;
	this->fd=fd;
	return getCurrentProperties();
}

RUDIMENTS_INLINE int filesystem::getCurrentProperties() {
	return (fstatfs(fd,&st)!=-1);
}

#define STATFS(path,out,member) \
	struct statfs st; \
	if (statfs(path,&st)==-1) { \
		return 0; \
	} \
	*out=st.member; \
	return 1;

#define FSTATFS(fd,out,member) \
	struct statfs st; \
	if (fstatfs(fd,&st)==-1) { \
		return 0; \
	} \
	*out=st.member; \
	return 1;

RUDIMENTS_INLINE int filesystem::getType(const char *path, long *type) {
	STATFS(path,type,f_type)
}

RUDIMENTS_INLINE int filesystem::getType(int fd, long *type) {
	FSTATFS(fd,type,f_type)
}

RUDIMENTS_INLINE long filesystem::getType() const {
	return st.f_type;
}

RUDIMENTS_INLINE int filesystem::getOptimumTransferBlockSize(const char *path,
								long *size) {
	STATFS(path,size,f_bsize)
}

RUDIMENTS_INLINE int filesystem::getOptimumTransferBlockSize(int fd,
								long *size) {
	FSTATFS(fd,size,f_bsize)
}

RUDIMENTS_INLINE long filesystem::getOptimumTransferBlockSize() const {
	return st.f_bsize;
}

RUDIMENTS_INLINE int filesystem::getTotalBlocks(const char *path,
							long *blocks) {
	STATFS(path,blocks,f_blocks)
}

RUDIMENTS_INLINE int filesystem::getTotalBlocks(int fd, long *blocks) {
	FSTATFS(fd,blocks,f_blocks)
}

RUDIMENTS_INLINE long filesystem::getTotalBlocks() const {
	return st.f_blocks;
}

RUDIMENTS_INLINE int filesystem::getFreeBlocks(const char *path, long *blocks) {
	STATFS(path,blocks,f_bfree)
}

RUDIMENTS_INLINE int filesystem::getFreeBlocks(int fd, long *blocks) {
	FSTATFS(fd,blocks,f_bfree)
}

RUDIMENTS_INLINE long filesystem::getFreeBlocks() const {
	return st.f_bfree;
}

RUDIMENTS_INLINE int filesystem::getAvailableBlocks(const char *path,
								long *blocks) {
	STATFS(path,blocks,f_bavail)
}

RUDIMENTS_INLINE int filesystem::getAvailableBlocks(int fd, long *blocks) {
	FSTATFS(fd,blocks,f_bavail)
}

RUDIMENTS_INLINE long filesystem::getAvailableBlocks() const {
	return st.f_bavail;
}

RUDIMENTS_INLINE int filesystem::getTotalFileNodes(const char *path,
								long *nodes) {
	STATFS(path,nodes,f_files)
}

RUDIMENTS_INLINE int filesystem::getTotalFileNodes(int fd, long *nodes) {
	FSTATFS(fd,nodes,f_files)
}

RUDIMENTS_INLINE long filesystem::getTotalFileNodes() const {
	return st.f_files;
}

RUDIMENTS_INLINE int filesystem::getFreeFileNodes(const char *path,
								long *nodes) {
	STATFS(path,nodes,f_ffree)
}

RUDIMENTS_INLINE int filesystem::getFreeFileNodes(int fd, long *nodes) {
	FSTATFS(fd,nodes,f_ffree)
}

RUDIMENTS_INLINE long filesystem::getFreeFileNodes() const {
	return st.f_ffree;
}

RUDIMENTS_INLINE int filesystem::getFileSystemId(const char *path, fsid_t *id) {
	STATFS(path,id,f_fsid)
}

RUDIMENTS_INLINE int filesystem::getFileSystemId(int fd, fsid_t *id) {
	FSTATFS(fd,id,f_fsid)
}

RUDIMENTS_INLINE fsid_t filesystem::getFileSystemId() const {
	return st.f_fsid;
}

RUDIMENTS_INLINE int filesystem::getMaximumFileNameLength(const char *path,
								long *length) {
	STATFS(path,length,f_namelen)
}

RUDIMENTS_INLINE int filesystem::getMaximumFileNameLength(int fd,
								long *length) {
	FSTATFS(fd,length,f_namelen)
}

RUDIMENTS_INLINE long filesystem::getMaximumFileNameLength() const {
	return st.f_namelen;
}

RUDIMENTS_INLINE struct statfs *filesystem::getStatfs() {
	return &st;
}
