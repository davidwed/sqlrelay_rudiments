// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/filesystem.h>
#include <rudiments/rawbuffer.h>
#include <rudiments/error.h>
#ifdef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
	#include <rudiments/charstring.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_STAT_H
	#include <sys/stat.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

#ifdef RUDIMENTS_HAVE_IO_H
	#undef _POSIX_
	#include <io.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
	#ifdef RUDIMENTS_HAVE_SYS_PARAM_H
		#include <sys/param.h>
	#endif
	struct	statfs {
		int64_t	f_type;
		DWORD	f_bsize;
		int64_t	f_blocks;
		int64_t	f_bfree;
		int64_t	f_bavail;
		int64_t	f_files;
		int64_t	f_ffree;
		int64_t	f_fsid;
		DWORD	f_namelen;
	};
#endif

#ifndef PATH_MAX
	#define PATH_MAX 1024
#endif

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class filesystemprivate {
	friend class filesystem;
	private:
#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS)
		struct	statvfs	_st;
#elif defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS) || \
			defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
		struct	statfs	_st;
#else
	#error no statvfs, statfs or anything like it
#endif
		int32_t	_fd;
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
		#if defined(RUDIMENTS_HAVE__OPEN)
			pvt->_fd=_open(path,O_RDONLY);
		#elif defined(RUDIMENTS_HAVE_OPEN)
			pvt->_fd=::open(path,O_RDONLY);
		#else
			#error no open or anything like it
		#endif
	} while (pvt->_fd==-1 && error::getErrorNumber()==EINTR);
	return (pvt->_fd!=-1 && getCurrentProperties());
}

bool filesystem::initialize(int32_t fd) {
	close();
	pvt->_closeflag=false;
	pvt->_fd=fd;
	return getCurrentProperties();
}

bool filesystem::close() {
	if (pvt->_fd>-1 && pvt->_closeflag) {
		int32_t	result;
		do {
			#if defined(RUDIMENTS_HAVE__CLOSE)
				result=_close(pvt->_fd);
			#elif defined(RUDIMENTS_HAVE_CLOSE)
				result=::close(pvt->_fd);
			#else
				#error no close or anything like it
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		pvt->_fd=-1;
		return !result;
	}
	return true;
}

bool filesystem::getCurrentProperties() {
	return filesystem::getCurrentPropertiesInternal(
				pvt->_fd,(void *)&pvt->_st);
}

bool filesystem::getCurrentPropertiesInternal(int32_t fd, void *st) {
	#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS) || \
			defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS)
		int32_t	result;
		do {
			#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS)
				result=fstatvfs(fd,(struct statvfs *)st);
			#elif defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS)
				result=fstatfs(fd,(struct statfs *)st);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;
	#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
		error::setErrorNumber(ENOSYS);
		return false;
	#else
		#error no fstatvfs, fstatfs or anything like it
	#endif
}

bool filesystem::getCurrentPropertiesInternal(const char *path, void *st) {
	#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS) || \
			defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS)

		int32_t	result;
		do {
			#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS)
				result=::statvfs(path,(struct statvfs *)st);
			#elif defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS)
				result=::statfs(path,(struct statfs *)st);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;

	#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)

		// convert st to statfs type
		struct statfs	*stfs=(struct statfs *)st;

		// Get "old-school" free space - sectors and clusters,
		// if it fails, fall back to some safe defaults
		DWORD	sectorspercluster;
		DWORD	freeclusters;
		DWORD	totalclusters;
		if (!GetDiskFreeSpace(path,&sectorspercluster,
						&(stfs->f_bsize),
						&freeclusters,
						&totalclusters)) {
			sectorspercluster=1;
			stfs->f_bsize=1;
			freeclusters=0;
			totalclusters=0;
		}

		// Attempt to get free disk space directly using
		// GetDiskFreeSpaceEx.  This method isn't available
		// on all version of windows though, so we'll have to
		// do it like this...
		ULARGE_INTEGER	availbytes;
		ULARGE_INTEGER	totalbytes;
		ULARGE_INTEGER	freebytes;
		HMODULE	lib=LoadLibrary("KERNEL32");
		bool	(*getfreediskspaceex)
				(LPCSTR,PULARGE_INTEGER,
						PULARGE_INTEGER,
						PULARGE_INTEGER)=NULL;
		if (lib) {
			getfreediskspaceex=
				(bool (*)(LPCSTR,PULARGE_INTEGER,
						PULARGE_INTEGER,
						PULARGE_INTEGER))
				GetProcAddress(lib,
						"GetDiskFreeSpaceEx");
			if (getfreediskspaceex) {
				if (!getfreediskspaceex(path,
						&availbytes,
						&totalbytes,
						&freebytes)) {
					FreeLibrary(lib);
					return false;
				}
			}
			FreeLibrary(lib);
		}

		// if we couldn't get free space directly, calculate it
		// from sectors and clusters
		if (!getfreediskspaceex) {
			DWORD	bytespercluster=
					sectorspercluster*stfs->f_bsize;
			totalbytes.QuadPart=totalclusters*bytespercluster;
			freebytes.QuadPart=freeclusters*bytespercluster;
			availbytes=freebytes;
		}
		stfs->f_bavail=availbytes.QuadPart/stfs->f_bsize;
		stfs->f_bfree=freebytes.QuadPart/stfs->f_bsize;
		stfs->f_ffree=stfs->f_bfree/sectorspercluster;
		stfs->f_blocks=totalbytes.QuadPart/stfs->f_bsize;
		stfs->f_files=stfs->f_blocks/sectorspercluster;

		// get volume information
		CHAR	volumename[PATH_MAX];
		DWORD	volumeserialnumber;
		DWORD	filesystemflags;
		CHAR	filesystemname[PATH_MAX];
		if (!GetVolumeInformation(path,
					volumename,
					PATH_MAX,
					&volumeserialnumber,
					&(stfs->f_namelen),
					&filesystemflags,
					filesystemname,
					PATH_MAX)) {
			return false;
		}

		union fsid {
			int32_t	val32[2];
		};

		fsid	*f_fsid=(union fsid *)&(stfs->f_fsid);
		f_fsid->val32[0]=HIWORD(volumeserialnumber);
		f_fsid->val32[1]=LOWORD(volumeserialnumber);

		// translate the filesystem type name to 
		struct fsnames {
			const char	*uppername;
			int64_t		type;
		};
		fsnames	fsns[]={
			{"FAT",0x4d44},		// same as "msdos"
			{"FAT32",0x4d44},	// same as fat for now
			{"NTFS",0x72b6},	// same as ntfs
			{"CDFS",0x9660},	// same as "isofs"
			{NULL,0}
		};
		stfs->f_type=0x0;
		for (uint8_t i=0; fsns[i].uppername; i++) {
			if (!charstring::compare(filesystemname,
						fsns[i].uppername)) {
				stfs->f_type=fsns[i].type;
				break;
			}
		}

		return true;
	#else
		#error no statvfs, statfs or anything like it
	#endif
}

#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS)
#define FSTATFS(fd,out,member) \
	struct statvfs st; \
	int32_t	result=getCurrentPropertiesInternal(fd,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=(int64_t)st.member; \
	return true;
#define CHARFSTATFS(fd,out,member) \
	struct statvfs st; \
	int32_t	result=getCurrentPropertiesInternal(fd,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=st.member; \
	return true;
#else
#define FSTATFS(fd,out,member) \
	struct statfs st; \
	int32_t	result=getCurrentPropertiesInternal(fd,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=(int64_t)st.member; \
	return true;
#define CHARFSTATFS(fd,out,member) \
	struct statfs st; \
	int32_t	result=getCurrentPropertiesInternal(fd,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=st.member; \
	return true;
#endif

#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS)
#define STATFS(path,out,member) \
	struct statvfs st; \
	int32_t	result=getCurrentPropertiesInternal(path,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=(int64_t)st.member; \
	return true;
#define CHARSTATFS(path,out,member) \
	struct statvfs st; \
	int32_t	result=getCurrentPropertiesInternal(path,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=st.member; \
	return true;
#else
#define STATFS(path,out,member) \
	struct statfs st; \
	int32_t	result=getCurrentPropertiesInternal(path,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=(int64_t)st.member; \
	return true;
#define CHARSTATFS(path,out,member) \
	struct statfs st; \
	int32_t	result=getCurrentPropertiesInternal(path,(void *)&st); \
	if (!result) { \
		return false; \
	} \
	*out=st.member; \
	return true;
#endif

bool filesystem::getType(const char *path, int64_t *type) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,type,f_type)
#else
	*type=0;
	return true;
#endif
}

bool filesystem::getType(int32_t fd, int64_t *type) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,type,f_type)
#else
	*type=0;
	return true;
#endif
}

int64_t filesystem::getType() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_type;
#else
	return 0;
#endif
}

bool filesystem::getBlockSize(const char *path, int64_t *size) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,size,f_bsize)
#else
	#if defined(RUDIMENTS_HAVE_STATVFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
		defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS)
		STATFS(path,size,f_frsize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

bool filesystem::getBlockSize(int32_t fd, int64_t *size) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,size,f_bsize)
#else
	#if defined(RUDIMENTS_HAVE_STATVFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
		FSTATFS(fd,size,f_frsize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

int64_t filesystem::getBlockSize() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_bsize;
#else
	#if defined(RUDIMENTS_HAVE_STATVFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
		return pvt->_st.f_frsize;
	#else
		return 0;
	#endif
#endif
}

bool filesystem::getOptimumTransferBlockSize(const char *path,
						int64_t *size) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,size,f_bsize)
#else
	#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_DARWIN_STATFS)
		STATFS(path,size,f_iosize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

bool filesystem::getOptimumTransferBlockSize(int32_t fd, int64_t *size) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,size,f_bsize)
#else
	#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_DARWIN_STATFS)
		FSTATFS(fd,size,f_iosize)
	#else
		*size=0;
		return true;
	#endif
#endif
}

int64_t filesystem::getOptimumTransferBlockSize() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_bsize;
#else
	#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
		defined(RUDIMENTS_HAVE_DARWIN_STATFS)
		return pvt->_st.f_iosize;
	#else
		return 0;
	#endif
#endif
}

bool filesystem::getTotalBlocks(const char *path, int64_t *blocks) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,blocks,f_blocks)
#else
	*blocks=0;
	return true;
#endif
}

bool filesystem::getTotalBlocks(int32_t fd, int64_t *blocks) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,blocks,f_blocks)
#else
	*blocks=0;
	return true;
#endif
}

int64_t filesystem::getTotalBlocks() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_blocks;
#else
	return 0;
#endif
}

bool filesystem::getFreeBlocks(const char *path, int64_t *blocks) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,blocks,f_bfree)
#else
	*blocks=0;
	return true;
#endif
}

bool filesystem::getFreeBlocks(int32_t fd, int64_t *blocks) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,blocks,f_bfree)
#else
	*blocks=0;
	return true;
#endif
}

int64_t filesystem::getFreeBlocks() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_bfree;
#else
	return 0;
#endif
}

bool filesystem::getAvailableBlocks(const char *path, int64_t *blocks) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,blocks,f_bavail)
#else
	*blocks=0;
	return true;
#endif
}

bool filesystem::getAvailableBlocks(int32_t fd, int64_t *blocks) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,blocks,f_bavail)
#else
	*blocks=0;
	return true;
#endif
}

int64_t filesystem::getAvailableBlocks() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_bavail;
#else
	return 0;
#endif
}

bool filesystem::getTotalFileNodes(const char *path, int64_t *nodes) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,nodes,f_files)
#else
	*nodes=0;
	return true;
#endif
}

bool filesystem::getTotalFileNodes(int32_t fd, int64_t *nodes) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,nodes,f_files)
#else
	*nodes=0;
	return true;
#endif
}

int64_t filesystem::getTotalFileNodes() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_files;
#else
	return 0;
#endif
}

bool filesystem::getFreeFileNodes(const char *path, int64_t *nodes) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

bool filesystem::getFreeFileNodes(int32_t fd, int64_t *nodes) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

int64_t filesystem::getFreeFileNodes() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_ffree;
#else
	return 0;
#endif
}

bool filesystem::getAvailableFileNodes(const char *path, int64_t *nodes) {
#if defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS)
	STATFS(path,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

bool filesystem::getAvailableFileNodes(int32_t fd, int64_t *nodes) {
#if defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS)
	FSTATFS(fd,nodes,f_ffree)
#else
	*nodes=0;
	return true;
#endif
}

int64_t filesystem::getAvailableFileNodes() const {
#if defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS)
	return pvt->_st.f_ffree;
#else
	return 0;
#endif
}

bool filesystem::getFileSystemId(const char *path, int64_t *id) {
#if defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,id,f_fsid)
#else
	*id=0;
	return true;
#endif
}

bool filesystem::getFileSystemId(int32_t fd, int64_t *id) {
#if defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,id,f_fsid)
#else
	*id=0;
	return true;
#endif
}

int64_t filesystem::getFileSystemId() const {
#if defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return (int64_t)pvt->_st.f_fsid;
#else
	return 0;
#endif
}

bool filesystem::getMaximumFileNameLength(const char *path,
						int64_t *length) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	STATFS(path,length,f_namelen)
#else
	#if defined(RUDIMENTS_HAVE_STATVFS) || \
		defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
		STATFS(path,length,f_namemax)
	#else
		*length=0;
		return true;
	#endif
#endif
}

bool filesystem::getMaximumFileNameLength(int32_t fd, int64_t *length) {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	FSTATFS(fd,length,f_namelen)
#else
	#if defined(RUDIMENTS_HAVE_STATVFS) || \
		defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
		FSTATFS(fd,length,f_namemax)
	#else
		*length=0;
		return true;
	#endif
#endif
}

int64_t filesystem::getMaximumFileNameLength() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_namelen;
#else
	#if defined(RUDIMENTS_HAVE_STATVFS) || \
		defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
		defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
		return pvt->_st.f_namemax;
	#else
		return 0;
	#endif
#endif
}

bool filesystem::getOwner(const char *path, uid_t *owner) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	STATFS(path,owner,f_owner)
#else
	*owner=0;
	return true;
#endif
}

bool filesystem::getOwner(int32_t fd, uid_t *owner) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	FSTATFS(fd,owner,f_owner)
#else
	*owner=0;
	return true;
#endif
}

uid_t filesystem::getOwner() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	return pvt->_st.f_owner;
#else
	return 0;
#endif
}

bool filesystem::getSyncWrites(const char *path, int64_t *swrites) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS)
	STATFS(path,swrites,f_syncwrites)
#else
	*swrites=0;
	return true;
#endif
}

bool filesystem::getSyncWrites(int32_t fd, int64_t *swrites) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS)
	FSTATFS(fd,swrites,f_syncwrites)
#else
	*swrites=0;
	return true;
#endif
}

int64_t filesystem::getSyncWrites() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS)
	return pvt->_st.f_syncwrites;
#else
	return 0;
#endif
}

bool filesystem::getAsyncWrites(const char *path, int64_t *aswrites) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS)
	STATFS(path,aswrites,f_asyncwrites)
#else
	*aswrites=0;
	return true;
#endif
}

bool filesystem::getAsyncWrites(int32_t fd, int64_t *aswrites) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS)
	FSTATFS(fd,aswrites,f_asyncwrites)
#else
	*aswrites=0;
	return true;
#endif
}

int64_t filesystem::getAsyncWrites() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS)
	return pvt->_st.f_asyncwrites;
#else
	return 0;
#endif
}

bool filesystem::getMountPoint(const char *path, const char **mtpt) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	CHARSTATFS(path,mtpt,f_mntonname)
#else
	*mtpt=NULL;
	return true;
#endif
}

bool filesystem::getMountPoint(int32_t fd, const char **mtpt) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	CHARFSTATFS(fd,mtpt,f_mntonname)
#else
	*mtpt=NULL;
	return true;
#endif
}

const char *filesystem::getMountPoint() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_mntonname;
#else
	return NULL;
#endif
}

bool filesystem::getSyncReads(const char *path, int64_t *sreads) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	STATFS(path,sreads,f_syncreads)
#else
	*sreads=0;
	return true;
#endif
}

bool filesystem::getSyncReads(int32_t fd, int64_t *sreads) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	FSTATFS(fd,sreads,f_syncreads)
#else
	*sreads=0;
	return true;
#endif
}

int64_t filesystem::getSyncReads() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	return pvt->_st.f_syncreads;
#else
	return 0;
#endif
}

bool filesystem::getAsyncReads(const char *path, int64_t *asreads) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	STATFS(path,asreads,f_asyncreads)
#else
	*asreads=0;
	return false;
#endif
}

bool filesystem::getAsyncReads(int32_t fd, int64_t *asreads) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	FSTATFS(fd,asreads,f_asyncreads)
#else
	*asreads=0;
	return false;
#endif
}

int64_t filesystem::getAsyncReads() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	return pvt->_st.f_asyncreads;
#else
	return 0;
#endif
}

bool filesystem::getDeviceName(const char *path, const char **devname) {
#if defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	CHARSTATFS(path,devname,f_mntfromname)
#else
	*devname=NULL;
	return true;
#endif
}

bool filesystem::getDeviceName(int32_t fd, const char **devname) {
#if defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	CHARFSTATFS(fd,devname,f_mntfromname)
#else
	*devname=NULL;
	return true;
#endif
}

const char *filesystem::getDeviceName() const {
#if defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_mntfromname;
#else
	return NULL;
#endif
}

bool filesystem::getFilesystemSpecificString(const char *path,
						const char **str) {
#ifdef RUDIMENTS_HAVE_STATVFS
	CHARSTATFS(path,str,f_fstr)
#else
	*str=NULL;
	return true;
#endif
}

bool filesystem::getFilesystemSpecificString(int32_t fd, const char **str) {
#ifdef RUDIMENTS_HAVE_STATVFS
	CHARFSTATFS(fd,str,f_fstr)
#else
	*str=NULL;
	return true;
#endif
}

const char *filesystem::getFilesystemSpecificString() const {
#ifdef RUDIMENTS_HAVE_STATVFS
	return (const char *)pvt->_st.f_fstr;
#else
	return NULL;
#endif
}

void *filesystem::getInternalFilesystemStatisticsStructure() {
	return (void *)&pvt->_st;
}

bool filesystem::getTypeName(const char *path, const char **name) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	CHARSTATFS(path,name,f_fstypename)
#else
	#ifdef RUDIMENTS_HAVE_STATVFS
		CHARSTATFS(path,name,f_basetype)
	#else
		#if defined(RUDIMENTS_HAVE_LINUX_STATFS)
			struct statfs st;
			int32_t	result;
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

bool filesystem::getTypeName(int32_t fd, const char **name) {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	CHARFSTATFS(fd,name,f_fstypename)
#else
	#ifdef RUDIMENTS_HAVE_STATVFS
		CHARFSTATFS(fd,name,f_basetype)
	#else
		#if defined(RUDIMENTS_HAVE_LINUX_STATFS)
			struct statfs st;
			int32_t	result;
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
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_fstypename;
#else
	#ifdef RUDIMENTS_HAVE_STATVFS
		return (const char *)pvt->_st.f_basetype;
	#else
		#if defined(RUDIMENTS_HAVE_LINUX_STATFS)
			return filesystem::getFsTypeName(pvt->_st.f_type);
		#else
			return NULL;
		#endif
	#endif
#endif
}

const char *filesystem::getFsTypeName(int64_t fstype) {
	if (fstype==0xADF5) {
		return "adfs";
	} else if (fstype==0xADFF) {
		return "affs";
	} else if (fstype==0x42465331) {
		return "befs";
	} else if (fstype==0x1BADFACE) {
		return "bfs";
	} else if (fstype==0xFF534D42) {
		return "cifs";
	} else if (fstype==0x73757245) {
		return "codafs";
	} else if (fstype==0x012FF7B7) {
		return "coherent";
	} else if (fstype==0x28cd3d45) {
		return "cramfs";
	} else if (fstype==0x1373) {
		return "devfs";
	} else if (fstype==0x00414A53) {
		return "efs";
	} else if (fstype==0x137D) {
		return "ext";
	} else if (fstype==0xEF51) {
		return "ext2 (old)";
	} else if (fstype==0xEF53) {
		return "ext2";
	} else if (fstype==0x4244) {
		return "hfs";
	} else if (fstype==0xF995E849) {
		return "hpfs";
	} else if (fstype==0x958458f6) {
		return "hugetlbfs";
	} else if (fstype==0x9660) {
		return "isofs";
	} else if (fstype==0x72b6) {
		return "jffs2";
	} else if (fstype==0x3153464a) {
		return "jfs";
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
	} else if (fstype==0x72b6) {
		return "ntfs";
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
	} else if (fstype==0x012FF7B6) {
		return "sysv2";
	} else if (fstype==0x012FF7B5) {
		return "sysv4";
	} else if (fstype==0x01021994) {
		return "tmpfs";
	} else if (fstype==0x15013346) {
		return "udf";
	} else if (fstype==0x00011954) {
		return "ufs";
	} else if (fstype==0x9fa2) {
		return "usbdevfs";
	} else if (fstype==0xa501FCF5) {
		return "vxfs";
	} else if (fstype==0x012FF7B4) {
		return "xenix";
	} else if (fstype==0x58465342) {
		return "xfs";
	} else if (fstype==0x012FD16D) {
		return "xiafs";
	}
	return NULL;
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
