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
#ifdef HAVE_STATVFS
	return (fstatvfs(fd,&st)!=-1);
#else
	return (fstatfs(fd,&st)!=-1);
#endif
}

#ifdef HAVE_STATVFS
	#define STATFS(path,out,member) \
		struct statvfs st; \
		if (statvfs(path,&st)==-1) { \
			return 0; \
		} \
		*out=st.member; \
		return 1;

	#define FSTATFS(fd,out,member) \
		struct statvfs st; \
		if (fstatvfs(fd,&st)==-1) { \
			return 0; \
		} \
		*out=st.member; \
		return 1;
#else
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
#endif

RUDIMENTS_INLINE int filesystem::getType(const char *path, long *type) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS)
	STATFS(path,type,f_type)
#else
	*type=0;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getType(int fd, long *type) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS)
	FSTATFS(fd,type,f_type)
#else
	*type=0;
	return 1;
#endif
}

RUDIMENTS_INLINE long filesystem::getType() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS)
	return st.f_type;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getBlockSize(const char *path, long *size) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,size,f_bsize)
#else
	#ifdef HAVE_STATVFS
		STATFS(path,size,f_frsize)
	#else
		*size=0;
		return 1;
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getBlockSize(int fd, long *size) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,size,f_bsize)
#else
	#ifdef HAVE_STATVFS
		FSTATFS(fd,size,f_frsize)
	#else
		*size=0;
		return 1;
	#endif
#endif
}

RUDIMENTS_INLINE long filesystem::getBlockSize() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return st.f_bsize;
#else
	#ifdef HAVE_STATVFS
		return st.f_frsize;
	#else
		return 0;
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getOptimumTransferBlockSize(const char *path,
								long *size) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,size,f_bsize)
#else
	#if defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS)
		STATFS(path,size,f_iosize)
	#else
		*size=0;
		return 1;
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getOptimumTransferBlockSize(int fd,
								long *size) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,size,f_bsize)
#else
	#if defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS)
		FSTATFS(fd,size,f_iosize)
	#else
		*size=0;
		return 1;
	#endif
#endif
}

RUDIMENTS_INLINE long filesystem::getOptimumTransferBlockSize() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_STATVFS)
	return st.f_bsize;
#else
	#if defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS)
		return st.f_iosize;
	#else
		return 0;
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getTotalBlocks(const char *path,
							long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,blocks,f_blocks)
#else
	*blocks=0;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getTotalBlocks(int fd, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,blocks,f_blocks)
#else
	*blocks=0;
	return 1;
#endif
}

RUDIMENTS_INLINE long filesystem::getTotalBlocks() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	return st.f_blocks;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getFreeBlocks(const char *path, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,blocks,f_bfree)
#else
	*blocks=0;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getFreeBlocks(int fd, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,blocks,f_bfree)
#else
	*blocks=0;
	return 1;
#endif
}

RUDIMENTS_INLINE long filesystem::getFreeBlocks() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	return st.f_bfree;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getAvailableBlocks(const char *path,
								long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,blocks,f_bavail)
#else
	*blocks=0;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getAvailableBlocks(int fd, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,blocks,f_bavail)
#else
	*blocks=0;
	return 1;
#endif
}

RUDIMENTS_INLINE long filesystem::getAvailableBlocks() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	return st.f_bavail;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getTotalFileNodes(const char *path,
								long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,nodes,f_files)
#else
	*nodes=0;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getTotalFileNodes(int fd, long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,nodes,f_files)
#else
	*nodes=0;
	return 1;
#endif
}

RUDIMENTS_INLINE long filesystem::getTotalFileNodes() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	return st.f_files;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getFreeFileNodes(const char *path,
								long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,nodes,f_ffree)
#else
	*nodes=0;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getFreeFileNodes(int fd, long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,nodes,f_ffree)
#else
	*nodes=0;
	return 1;
#endif
}

RUDIMENTS_INLINE long filesystem::getFreeFileNodes() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_STATVFS)
	return st.f_ffree;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getAvailableFileNodes(const char *path,
								long *nodes) {
#ifdef HAVE_STATVFS
	STATFS(path,nodes,f_ffree)
#else
	*nodes=0;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getAvailableFileNodes(int fd, long *nodes) {
#ifdef HAVE_STATVFS
	FSTATFS(fd,nodes,f_ffree)
#else
	*nodes=0;
	return 1;
#endif
}

RUDIMENTS_INLINE long filesystem::getAvailableFileNodes() const {
#ifdef HAVE_STATVFS
	return st.f_ffree;
#else
	return 0;
#endif
}

#ifdef HAVE_STATVFS
	RUDIMENTS_INLINE int filesystem::getFileSystemId(const char *path,
								long *id) {
		STATFS(path,id,f_fsid)
	}

	RUDIMENTS_INLINE int filesystem::getFileSystemId(int fd, long *id) {
		FSTATFS(fd,id,f_fsid)
	}

	RUDIMENTS_INLINE long filesystem::getFileSystemId() const {
		return st.f_fsid;
	}
#else
	RUDIMENTS_INLINE int filesystem::getFileSystemId(const char *path,
								fsid_t *id) {
	#if defined(HAVE_LINUX_STATFS) || \
		defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS)
		STATFS(path,id,f_fsid)
	#else
		*id=0;
		return 1;
	#endif
	}

	RUDIMENTS_INLINE int filesystem::getFileSystemId(int fd, fsid_t *id) {
	#if defined(HAVE_LINUX_STATFS) || \
		defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS)
		FSTATFS(fd,id,f_fsid)
	#else
		*id=0;
		return 1;
	#endif
	}

	RUDIMENTS_INLINE fsid_t filesystem::getFileSystemId() const {
	#if defined(HAVE_LINUX_STATFS) || \
		defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS)
		return st.f_fsid;
	#else
		return 0;
	#endif
	}
#endif

RUDIMENTS_INLINE int filesystem::getMaximumFileNameLength(const char *path,
								long *length) {
#ifdef HAVE_LINUX_STATFS
	STATFS(path,length,f_namelen)
#else
	#ifdef HAVE_STATVFS
		STATFS(path,length,f_namemax)
	#else
		*length=0;
		return 1;
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getMaximumFileNameLength(int fd,
								long *length) {
#ifdef HAVE_LINUX_STATFS
	FSTATFS(fd,length,f_namelen)
#else
	#ifdef HAVE_STATVFS
		FSTATFS(fd,length,f_namemax)
	#else
		*length=0;
		return 1;
	#endif
#endif
}

RUDIMENTS_INLINE long filesystem::getMaximumFileNameLength() const {
#ifdef HAVE_LINUX_STATFS
	return st.f_namelen;
#else
	#ifdef HAVE_STATVFS
		return st.f_namemax;
	#else
		return 0;
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getOwner(const char *path, uid_t *owner) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,owner,f_owner)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getOwner(int fd, uid_t *owner) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,owner,f_owner)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE uid_t filesystem::getOwner() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return st.f_owner;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getSyncWrites(const char *path,
							long *swrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,swrites,f_syncwrites)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getSyncWrites(int fd, long *swrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,swrites,f_syncwrites)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE long filesystem::getSyncWrites() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return st.f_syncwrites;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getAsyncWrites(const char *path,
							long *aswrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,aswrites,f_asyncwrites)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getAsyncWrites(int fd, long *aswrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,aswrites,f_asyncwrites)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE long filesystem::getAsyncWrites() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return st.f_asyncwrites;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getTypeName(const char *path, char **name) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,name,f_fstypename)
#else
	#ifdef HAVE_STATVFS
		STATFS(path,name,f_basetype)
	#else
		#ifdef HAVE_LINUX_STATFS
			struct statfs st;
			if (statfs(path,&st)==-1) {
				return 0;
			}
			*name=filesystem::getFsTypeName(st.f_type);
			return 1;
		#else
			*name=NULL;
		#endif
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getTypeName(int fd, char **name) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,name,f_fstypename)
#else
	#ifdef HAVE_STATVFS
		FSTATFS(fd,name,f_basetype)
	#else
		#ifdef HAVE_LINUX_STATFS
			struct statfs st;
			if (fstatfs(fd,&st)==-1) {
				return 0;
			}
			*name=filesystem::getFsTypeName(st.f_type);
			return 1;
		#else
			*name=NULL;
		#endif
	#endif
#endif
}

RUDIMENTS_INLINE char *filesystem::getTypeName() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return (char *)st.f_fstypename;
#else
	#ifdef HAVE_STATVFS
		return (char *)st.f_basetype;
	#else
		#ifdef HAVE_LINUX_STATFS
			return filesystem::getFsTypeName(st.f_type);
		#else
			return NULL;
		#endif
	#endif
#endif
}

RUDIMENTS_INLINE int filesystem::getMountPoint(const char *path, char **mtpt) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,mtpt,f_mntonname)
#else
	*mtpt=NULL;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getMountPoint(int fd, char **mtpt) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,mtpt,f_mntonname)
#else
	*mtpt=NULL;
	return 1;
#endif
}

RUDIMENTS_INLINE char *filesystem::getMountPoint() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return (char *)st.f_mntonname;
#else
	return NULL;
#endif
}

RUDIMENTS_INLINE int filesystem::getSyncReads(const char *path,
							long *sreads) {
#if defined(HAVE_FREEBSD_STATFS)
	STATFS(path,sreads,f_syncreads)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getSyncReads(int fd, long *sreads) {
#if defined(HAVE_FREEBSD_STATFS)
	FSTATFS(fd,sreads,f_syncreads)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE long filesystem::getSyncReads() const {
#if defined(HAVE_FREEBSD_STATFS)
	return st.f_syncreads;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getAsyncReads(const char *path,
							long *asreads) {
#if defined(HAVE_FREEBSD_STATFS)
	STATFS(path,asreads,f_asyncreads)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getAsyncReads(int fd, long *asreads) {
#if defined(HAVE_FREEBSD_STATFS)
	FSTATFS(fd,asreads,f_asyncreads)
#else
	return 0;
#endif
}

RUDIMENTS_INLINE long filesystem::getAsyncReads() const {
#if defined(HAVE_FREEBSD_STATFS)
	return st.f_asyncreads;
#else
	return 0;
#endif
}

RUDIMENTS_INLINE int filesystem::getDeviceName(const char *path,
							char **devname) {
#if defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,devname,f_mntfromname)
#else
	*devname=NULL;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getDeviceName(int fd, char **devname) {
#if defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,devname,f_mntfromname)
#else
	*devname=NULL;
	return 1;
#endif
}

RUDIMENTS_INLINE char *filesystem::getDeviceName() const {
#if defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return (char *)st.f_mntfromname;
#else
	return NULL;
#endif
}

RUDIMENTS_INLINE int filesystem::getFilesystemSpecificString(const char *path,
								char **str) {
#ifdef HAVE_STATVFS
	STATFS(path,str,f_fstr)
#else
	*str=NULL;
	return 1;
#endif
}

RUDIMENTS_INLINE int filesystem::getFilesystemSpecificString(int fd,
								char **str) {
#ifdef HAVE_STATVFS
	FSTATFS(fd,str,f_fstr)
#else
	*str=NULL;
	return 1;
#endif
}

RUDIMENTS_INLINE char *filesystem::getFilesystemSpecificString() const {
#ifdef HAVE_STATVFS
	return (char *)st.f_fstr;
#else
	return NULL;
#endif
}


#ifdef HAVE_STATVFS
RUDIMENTS_INLINE struct statvfs *filesystem::getStatfs() {
	return &st;
}
#else
RUDIMENTS_INLINE struct statfs *filesystem::getStatfs() {
	return &st;
}
#endif
