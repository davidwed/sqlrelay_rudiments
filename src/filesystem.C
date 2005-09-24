// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/filesystem.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class filesystemprivate {
	friend class filesystem;
	private:
#ifdef HAVE_STATVFS
		struct	statvfs	_st;
#else
		struct	statfs	_st;
#endif
		int	_fd;
		bool	_closeflag;
};

filesystem::filesystem() {
	pvt=new filesystemprivate;
	pvt->_fd=-1;
	pvt->_closeflag=false;
	rawbuffer::zero(&pvt->_st,sizeof(pvt->_st));
}

filesystem::filesystem(const filesystem &f) {
	pvt=new filesystemprivate;
	filesystemClone(f);
}

filesystem &filesystem::operator=(const filesystem &f) {
	if (this!=&f) {
		filesystemClone(f);
	}
	return *this;
}

void filesystem::filesystemClone(const filesystem &f) {
	pvt->_fd=f.pvt->_fd;
	pvt->_closeflag=f.pvt->_closeflag;
	pvt->_st=f.pvt->_st;
}

filesystem::~filesystem() {
	close();
	delete pvt;
}

bool filesystem::initialize(const char *path) {
	close();
	pvt->_closeflag=true;
	do {
		pvt->_fd=::open(path,O_RDONLY);
	} while (pvt->_fd==-1 && error::getErrorNumber()==EINTR);
	return (pvt->_fd!=-1 && getCurrentProperties());
}

bool filesystem::initialize(int fd) {
	close();
	pvt->_closeflag=false;
	pvt->_fd=fd;
	return getCurrentProperties();
}

bool filesystem::close() {
	if (pvt->_fd>-1 && pvt->_closeflag) {
		int	result;
		do {
			result=::close(pvt->_fd);
		} while (result==-1 && error::getErrorNumber()==EINTR);
		pvt->_fd=-1;
		return !result;
	}
	return true;
}

bool filesystem::getCurrentProperties() {
	int	result;
	do {
		#ifdef HAVE_STATVFS
		result=fstatvfs(pvt->_fd,&pvt->_st);
		#else
		result=fstatfs(pvt->_fd,&pvt->_st);
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR);
	return !result;
}

#ifdef HAVE_STATVFS
	#define STATFS(path,out,member) \
		struct statvfs st; \
		int	result; \
		do { \
			result=::statvfs(path,&st); \
		} while (result==-1 && error::getErrorNumber()==EINTR); \
		if (result==-1) { \
			return false; \
		} \
		*out=st.member; \
		return true;

	#define FSTATFS(fd,out,member) \
		struct statvfs st; \
		int	result; \
		do { \
			result=fstatvfs(fd,&st); \
		} while (result==-1 && error::getErrorNumber()==EINTR); \
		if (result==-1) { \
			return false; \
		} \
		*out=st.member; \
		return true;
#else
	#define STATFS(path,out,member) \
		struct statfs st; \
		int	result; \
		do { \
			result=::statfs(path,&st); \
		} while (result==-1 && error::getErrorNumber()==EINTR); \
		if (result==-1) { \
			return false; \
		} \
		*out=st.member; \
		return true;

	#define FSTATFS(fd,out,member) \
		struct statfs st; \
		int	result; \
		do { \
			result=fstatfs(fd,&st); \
		} while (result==-1 && error::getErrorNumber()==EINTR); \
		if (result==-1) { \
			return false; \
		} \
		*out=st.member; \
		return true;
#endif

bool filesystem::getType(const char *path, long *type) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	STATFS(path,type,f_type)
#else
	*type=0;
	return true;
#endif
}

bool filesystem::getType(int fd, long *type) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	FSTATFS(fd,type,f_type)
#else
	*type=0;
	return true;
#endif
}

long filesystem::getType() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	return pvt->_st.f_type;
#else
	return 0;
#endif
}

bool filesystem::getBlockSize(const char *path, long *size) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	STATFS(path,size,f_bsize)
#else
	#ifdef HAVE_STATVFS
		STATFS(path,size,f_frsize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

bool filesystem::getBlockSize(int fd, long *size) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	FSTATFS(fd,size,f_bsize)
#else
	#ifdef HAVE_STATVFS
		FSTATFS(fd,size,f_frsize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

long filesystem::getBlockSize() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	return pvt->_st.f_bsize;
#else
	#ifdef HAVE_STATVFS
		return pvt->_st.f_frsize;
	#else
		return 0;
	#endif
#endif
}

bool filesystem::getOptimumTransferBlockSize(const char *path,
						long *size) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS) 
	STATFS(path,size,f_bsize)
#else
	#if defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS) || \
		defined(HAVE_DARWIN_STATFS)
		STATFS(path,size,f_iosize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

bool filesystem::getOptimumTransferBlockSize(int fd, long *size) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,size,f_bsize)
#else
	#if defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS) || \
		defined(HAVE_DARWIN_STATFS)
		FSTATFS(fd,size,f_iosize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

long filesystem::getOptimumTransferBlockSize() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	return pvt->_st.f_bsize;
#else
	#if defined(HAVE_FREEBSD_STATFS) || \
		defined(HAVE_NETBSD_STATFS) || \
		defined(HAVE_OPENBSD_STATFS) || \
		defined(HAVE_DARWIN_STATFS)
		return pvt->_st.f_iosize;
	#else
		return 0;
	#endif
#endif
}

bool filesystem::getTotalBlocks(const char *path, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,blocks,f_blocks)
#else
	*blocks=0;
	return true;
#endif
}

bool filesystem::getTotalBlocks(int fd, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,blocks,f_blocks)
#else
	*blocks=0;
	return true;
#endif
}

long filesystem::getTotalBlocks() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	return pvt->_st.f_blocks;
#else
	return 0;
#endif
}

bool filesystem::getFreeBlocks(const char *path, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,blocks,f_bfree)
#else
	*blocks=0;
	return true;
#endif
}

bool filesystem::getFreeBlocks(int fd, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,blocks,f_bfree)
#else
	*blocks=0;
	return true;
#endif
}

long filesystem::getFreeBlocks() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	return pvt->_st.f_bfree;
#else
	return 0;
#endif
}

bool filesystem::getAvailableBlocks(const char *path, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,blocks,f_bavail)
#else
	*blocks=0;
	return true;
#endif
}

bool filesystem::getAvailableBlocks(int fd, long *blocks) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,blocks,f_bavail)
#else
	*blocks=0;
	return true;
#endif
}

long filesystem::getAvailableBlocks() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	return pvt->_st.f_bavail;
#else
	return 0;
#endif
}

bool filesystem::getTotalFileNodes(const char *path, long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,nodes,f_files)
#else
	*nodes=0;
	return true;
#endif
}

bool filesystem::getTotalFileNodes(int fd, long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,nodes,f_files)
#else
	*nodes=0;
	return true;
#endif
}

long filesystem::getTotalFileNodes() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	return pvt->_st.f_files;
#else
	return 0;
#endif
}

bool filesystem::getFreeFileNodes(const char *path, long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	STATFS(path,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

bool filesystem::getFreeFileNodes(int fd, long *nodes) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	FSTATFS(fd,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

long filesystem::getFreeFileNodes() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS) || \
	defined(HAVE_CYGWIN_STATFS) || \
	defined(HAVE_STATVFS)
	return pvt->_st.f_ffree;
#else
	return 0;
#endif
}

bool filesystem::getAvailableFileNodes(const char *path, long *nodes) {
#ifdef HAVE_STATVFS
	STATFS(path,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

bool filesystem::getAvailableFileNodes(int fd, long *nodes) {
#ifdef HAVE_STATVFS
	FSTATFS(fd,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

long filesystem::getAvailableFileNodes() const {
#ifdef HAVE_STATVFS
	return pvt->_st.f_ffree;
#else
	return 0;
#endif
}

#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
bool filesystem::getFileSystemId(const char *path, fsid_t *id) {
	STATFS(path,id,f_fsid)
}

bool filesystem::getFileSystemId(int fd, fsid_t *id) {
	FSTATFS(fd,id,f_fsid)
}

fsid_t filesystem::getFileSystemId() const {
	return pvt->_st.f_fsid;
}
#else
bool filesystem::getFileSystemId(const char *path, long *id) {
#if defined(HAVE_STATVFS) || \
	defined(HAVE_CYGWIN_STATFS)
	STATFS(path,id,f_fsid)
#else
	*id=0;
	return true;
#endif
}

bool filesystem::getFileSystemId(int fd, long *id) {
#if defined(HAVE_STATVFS) || \
	defined(HAVE_CYGWIN_STATFS)
	FSTATFS(fd,id,f_fsid)
#else
	*id=0;
	return true;
#endif
}

long filesystem::getFileSystemId() const {
#if defined(HAVE_STATVFS) || \
	defined(HAVE_CYGWIN_STATFS)
	return pvt->_st.f_fsid;
#else
	return 0;
#endif
}
#endif

bool filesystem::getMaximumFileNameLength(const char *path,
						long *length) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	STATFS(path,length,f_namelen)
#else
	#ifdef HAVE_STATVFS
		STATFS(path,length,f_namemax)
	#else
		*length=0;
		return true;
	#endif
#endif
}

bool filesystem::getMaximumFileNameLength(int fd, long *length) {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	FSTATFS(fd,length,f_namelen)
#else
	#ifdef HAVE_STATVFS
		FSTATFS(fd,length,f_namemax)
	#else
		*length=0;
		return true;
	#endif
#endif
}

long filesystem::getMaximumFileNameLength() const {
#if defined(HAVE_LINUX_STATFS) || \
	defined(HAVE_CYGWIN_STATFS)
	return pvt->_st.f_namelen;
#else
	#ifdef HAVE_STATVFS
		return pvt->_st.f_namemax;
	#else
		return 0;
	#endif
#endif
}

bool filesystem::getOwner(const char *path, uid_t *owner) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	STATFS(path,owner,f_owner)
#else
	*owner=0;
	return true;
#endif
}

bool filesystem::getOwner(int fd, uid_t *owner) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	FSTATFS(fd,owner,f_owner)
#else
	*owner=0;
	return true;
#endif
}

uid_t filesystem::getOwner() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	return pvt->_st.f_owner;
#else
	return 0;
#endif
}

bool filesystem::getSyncWrites(const char *path, long *swrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,swrites,f_syncwrites)
#else
	*swrites=0;
	return true;
#endif
}

bool filesystem::getSyncWrites(int fd, long *swrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,swrites,f_syncwrites)
#else
	*swrites=0;
	return true;
#endif
}

long filesystem::getSyncWrites() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return pvt->_st.f_syncwrites;
#else
	return 0;
#endif
}

bool filesystem::getAsyncWrites(const char *path, long *aswrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	STATFS(path,aswrites,f_asyncwrites)
#else
	*aswrites=0;
	return true;
#endif
}

bool filesystem::getAsyncWrites(int fd, long *aswrites) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	FSTATFS(fd,aswrites,f_asyncwrites)
#else
	*aswrites=0;
	return true;
#endif
}

long filesystem::getAsyncWrites() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS)
	return pvt->_st.f_asyncwrites;
#else
	return 0;
#endif
}

bool filesystem::getMountPoint(const char *path, const char **mtpt) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	STATFS(path,mtpt,f_mntonname)
#else
	*mtpt=NULL;
	return true;
#endif
}

bool filesystem::getMountPoint(int fd, const char **mtpt) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	FSTATFS(fd,mtpt,f_mntonname)
#else
	*mtpt=NULL;
	return true;
#endif
}

const char *filesystem::getMountPoint() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_mntonname;
#else
	return NULL;
#endif
}

bool filesystem::getSyncReads(const char *path, long *sreads) {
#if defined(HAVE_FREEBSD_STATFS)
	STATFS(path,sreads,f_syncreads)
#else
	*sreads=0;
	return true;
#endif
}

bool filesystem::getSyncReads(int fd, long *sreads) {
#if defined(HAVE_FREEBSD_STATFS)
	FSTATFS(fd,sreads,f_syncreads)
#else
	*sreads=0;
	return true;
#endif
}

long filesystem::getSyncReads() const {
#if defined(HAVE_FREEBSD_STATFS)
	return pvt->_st.f_syncreads;
#else
	return 0;
#endif
}

bool filesystem::getAsyncReads(const char *path, long *asreads) {
#if defined(HAVE_FREEBSD_STATFS)
	STATFS(path,asreads,f_asyncreads)
#else
	*asreads=0;
	return false;
#endif
}

bool filesystem::getAsyncReads(int fd, long *asreads) {
#if defined(HAVE_FREEBSD_STATFS)
	FSTATFS(fd,asreads,f_asyncreads)
#else
	*asreads=0;
	return false;
#endif
}

long filesystem::getAsyncReads() const {
#if defined(HAVE_FREEBSD_STATFS)
	return pvt->_st.f_asyncreads;
#else
	return 0;
#endif
}

bool filesystem::getDeviceName(const char *path, const char **devname) {
#if defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	STATFS(path,devname,f_mntfromname)
#else
	*devname=NULL;
	return true;
#endif
}

bool filesystem::getDeviceName(int fd, const char **devname) {
#if defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	FSTATFS(fd,devname,f_mntfromname)
#else
	*devname=NULL;
	return true;
#endif
}

const char *filesystem::getDeviceName() const {
#if defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_mntfromname;
#else
	return NULL;
#endif
}

bool filesystem::getFilesystemSpecificString(const char *path,
						const char **str) {
#ifdef HAVE_STATVFS
	STATFS(path,str,f_fstr)
#else
	*str=NULL;
	return true;
#endif
}

bool filesystem::getFilesystemSpecificString(int fd, const char **str) {
#ifdef HAVE_STATVFS
	FSTATFS(fd,str,f_fstr)
#else
	*str=NULL;
	return true;
#endif
}

const char *filesystem::getFilesystemSpecificString() const {
#ifdef HAVE_STATVFS
	return (const char *)pvt->_st.f_fstr;
#else
	return NULL;
#endif
}


#ifdef HAVE_STATVFS
struct statvfs *filesystem::getStatfs() {
	return &pvt->_st;
}
#else
struct statfs *filesystem::getStatfs() {
	return &pvt->_st;
}
#endif

bool filesystem::getTypeName(const char *path, const char **name) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	STATFS(path,name,f_fstypename)
#else
	#ifdef HAVE_STATVFS
		STATFS(path,name,f_basetype)
	#else
		#if defined(HAVE_LINUX_STATFS)
			struct statfs st;
			int	result;
			do {
				result=::statfs(path,&st);
			} while (result==-1 && error::getErrorNumber()==EINTR);
			if (result==-1) {
				return false;
			}
			*name=filesystem::getFsTypeName(st.f_type);
			return true;
		#else
			*name=NULL;
			return true;
		#endif
	#endif
#endif
}

bool filesystem::getTypeName(int fd, const char **name) {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	FSTATFS(fd,name,f_fstypename)
#else
	#ifdef HAVE_STATVFS
		FSTATFS(fd,name,f_basetype)
	#else
		#if defined(HAVE_LINUX_STATFS)
			struct statfs st;
			int	result;
			do {
				result=fstatfs(fd,&st);
			} while (result==-1 && error::getErrorNumber()==EINTR);
			if (result==-1) {
				return false;
			}
			*name=filesystem::getFsTypeName(st.f_type);
			return true;
		#else
			*name=NULL;
			return true;
		#endif
	#endif
#endif
}

const char *filesystem::getTypeName() const {
#if defined(HAVE_FREEBSD_STATFS) || \
	defined(HAVE_NETBSD_STATFS) || \
	defined(HAVE_OPENBSD_STATFS) || \
	defined(HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_fstypename;
#else
	#ifdef HAVE_STATVFS
		return (const char *)pvt->_st.f_basetype;
	#else
		#if defined(HAVE_LINUX_STATFS)
			return filesystem::getFsTypeName(pvt->_st.f_type);
		#else
			return NULL;
		#endif
	#endif
#endif
}

#if defined(HAVE_LINUX_STATFS)
	const char *filesystem::getFsTypeName(long fstype) {
		if (fstype==0xADF5) {
			return "adfs";
		} else if (fstype==0xADFF) {
			return "affs";
		} else if (fstype==0x1BADFACE) {
			return "bfs";
		} else if (fstype==0x28cd3d45) {
			return "cramfs";
		} else if (fstype==0x00414A53) {
			return "efs";
		} else if (fstype==0x137D) {
			return "ext";
		} else if (fstype==0xEF51) {
			return "ext2 (old)";
		} else if (fstype==0xEF53) {
			return "ext2";
		// hfs???
		} else if (fstype==(long)0xF995E849) {
			return "hpfs";
		} else if (fstype==0x9660) {
			return "iso";
		} else if (fstype==0x137F) {
			return "minix (original)";
		} else if (fstype==0x138F) {
			return "minix (30 char)";
		} else if (fstype==0x2468) {
			return "minix2 (original)";
		} else if (fstype==0x2478) {
			return "minix2 (30 char)";
		} else if (fstype==0x4d44) {
			return "msdos";
		} else if (fstype==0x564c) {
			return "ncp";
		} else if (fstype==0x6969) {
			return "nfs";
		} else if (fstype==0x9fa1) {
			return "openprom";
		} else if (fstype==0x9fa0) {
			return "proc";
		} else if (fstype==0x002f) {
			return "qnx4";
		} else if (fstype==0x52654973) {
			return "reiserfs";
		} else if (fstype==0x7275) {
			return "romfs";
		} else if (fstype==0x517B) {
			return "smb";
		} else if (fstype==0x012FF7B4) {
			return "xenix";
		} else if (fstype==0x012FF7B5) {
			return "sysv4";
		} else if (fstype==0x012FF7B6) {
			return "sysv2";
		} else if (fstype==0x012FF7B7) {
			return "coherent";
		} else if (fstype==0x00011954) {
			return "ufs";
		} else if (fstype==0x58465342) {
			return "xfs";
		} else if (fstype==0x012FD16D) {
			return "xiafs";
		} else if (fstype==0x9fa2) {
			return "usbdevfs";
		}
		return NULL;
	}
#endif

#ifdef RUDIMENTS_NAMESPACE
}
#endif
