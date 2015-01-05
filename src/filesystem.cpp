// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/filesystem.h>
#include <rudiments/bytestring.h>
#include <rudiments/directory.h>
#include <rudiments/error.h>
#ifdef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
	#include <rudiments/filedescriptor.h>
	#include <rudiments/charstring.h>
#endif
#include <rudiments/stdio.h>

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

#ifndef PATH_MAX
	#define PATH_MAX 1024
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
		int64_t	f_fsid;
		DWORD	f_namelen;
		CHAR	f_typename[PATH_MAX];
		CHAR	f_volumename[PATH_MAX];
	};
#endif

class filesystemprivate {
	friend class filesystem;
	private:
		#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS)
			struct		statvfs	_st;
		#elif defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS) || \
				defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
			struct		statfs	_st;
		#else
			#error no statvfs, statfs or anything like it
		#endif
		int32_t		_fd;
		bool		_closeflag;
		#ifdef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
			char		*_volume;
		#endif
};

filesystem::filesystem() {
	pvt=new filesystemprivate;
	pvt->_fd=-1;
	pvt->_closeflag=false;
	#ifdef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
		pvt->_volume=NULL;
	#endif
	bytestring::zero(&pvt->_st,sizeof(pvt->_st));
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
	#ifdef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
		delete[] pvt->_volume;
		pvt->_volume=charstring::duplicate(f.pvt->_volume);
	#endif
	pvt->_st=f.pvt->_st;
}

filesystem::~filesystem() {
	close();
	delete pvt;
}

bool filesystem::initialize(const char *path) {
	close();
	if (!path || !path[0]) {
		path=directory::getCurrentWorkingDirectory();
	}
	#ifndef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
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
	#else 
		// extract the volume name from the path
		delete[] pvt->_volume;
		pvt->_volume=new CHAR[MAX_PATH];
		CHAR	*ptr;
		DWORD	len=GetFullPathName(path,MAX_PATH,pvt->_volume,&ptr);
		if (len>=3 && pvt->_volume[1]==':' && pvt->_volume[2]=='\\') {
			pvt->_volume[3]='\0';
		}
		return getCurrentProperties();
	#endif
}

bool filesystem::initialize(int32_t fd) {
	close();
	#ifndef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
		pvt->_fd=fd;
		return getCurrentProperties();
	#else
		// This is complex on Windows.
		// We have to determine the file name that the file descriptor
		// refers to and call the other version of initialize().  On
		// Vista and newer we can use GetFinalPathNameByHandle(), but
		// on older versions we have to memory-map the file, get the
		// file name from the map and convert the volume name to a
		// dos-style drive letter.  We'll just go ahead and do that.

		// get the file handle
		HANDLE	fh=(HANDLE)filedescriptor::
					getHandleFromFileDescriptor(fd);
		if (fh==INVALID_HANDLE_VALUE) {
			return false;
		}

		// get the file size, can't map a zero-byte file
		DWORD sizehi=0;
		DWORD sizelo=GetFileSize(fh,&sizehi);
		if (!sizehi && !sizelo) {
			return false;
		}

		// map the file
		HANDLE	fm=CreateFileMapping(fh,NULL,PAGE_READONLY,0,1,NULL);
		if (!fm) {
			return false;
		}

		// map view of file
		void	*fv=MapViewOfFile(fm,FILE_MAP_READ,0,0,1);
		if (!fv) {
			CloseHandle(fm);
			return false;
		}

		// get GetMappedFileName function
		HMODULE	kernel32=GetModuleHandle("Kernel32");
		if (!kernel32) {
			return false;
		}
		DWORD (*getmappedfilename)(HANDLE, LPVOID, LPTSTR, DWORD)=
			(DWORD (*)(HANDLE, LPVOID, LPTSTR, DWORD))
			GetProcAddress(kernel32,"K32GetMappedFileNameA");
		if (!getmappedfilename) {
			return false;
		}

		// get file name
		char	filename[MAX_PATH+1];
		if (!getmappedfilename(GetCurrentProcess(),
					fv,filename,MAX_PATH)) {
			UnmapViewOfFile(fv);
			CloseHandle(fm);
			return false;
		}

		// This will start with something like:
		// 	\Device\HarddiskVolume3\...
		// instead of a drive letter.
		// We need to translate it to a drive letter...

		// get the volume bitmap
		DWORD	volumes=GetLogicalDrives();

		// set up a template for the volume name
		char	volume[3];
		charstring::copy(volume," :");
		volume[0]='\0';

		// set up a buffer for the drive mapping
		char	mapping[MAX_PATH];

		// for each volume...
		for (char driveletter='A'; driveletter<='Z'; driveletter++) {

			// ignore volumes that don't exist
			DWORD	exists=volumes&0x0001;
			volumes=volumes>>1;
			if (!exists) {
				continue;
			}

			// create the volume name, eg: C:
			volume[0]=driveletter;

			// get the drive mapping
			if (!QueryDosDevice(volume,mapping,sizeof(mapping))) {
				UnmapViewOfFile(fv);
				CloseHandle(fm);
				return false;
			}

			// if the drive mapping for this volume matches
			// then we're done
			if (!charstring::compare(filename,mapping,
						charstring::length(mapping))) {
				break;
			}

			// reset the volume name
			volume[0]='\0';
		}

		// clean up
		UnmapViewOfFile(fv);
		CloseHandle(fm);

		// replace drive mapping with volume name in filename
		size_t	mappinglength=charstring::length(mapping);
		char	*ptr=filename;
		if (volume[0] && mappinglength>2) {
			ptr=filename+charstring::length(mapping)-2;
			*ptr=volume[0];
			*(ptr+1)=':';
		}
		
		// initialize using file name
		return (volume[0])?initialize(ptr):false;
	#endif
}

bool filesystem::close() {
	int32_t	result=0;
	#ifndef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
		if (pvt->_fd>-1 && pvt->_closeflag) {
			do {
				#if defined(RUDIMENTS_HAVE__CLOSE)
					result=_close(pvt->_fd);
				#elif defined(RUDIMENTS_HAVE_CLOSE)
					result=::close(pvt->_fd);
				#else
					#error no close or anything like it
				#endif
			} while (result==-1 && error::getErrorNumber()==EINTR);
		}
	#else
		delete[] pvt->_volume;
		pvt->_volume=NULL;
	#endif
	pvt->_fd=-1;
	pvt->_closeflag=false;
	bytestring::zero(&pvt->_st,sizeof(pvt->_st));
	return !result;
}

bool filesystem::getCurrentProperties() {
	#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS) || \
			defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS)

		int32_t	result;
		do {
			#if defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS)
				result=fstatvfs(pvt->_fd,&pvt->_st);
			#elif defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS)
				result=fstatfs(pvt->_fd,&pvt->_st);
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR);
		return !result;

	#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)

		// clear the statfs buffer
		bytestring::zero(&pvt->_st,sizeof(pvt->_st));

		// bail if no volume was specified
		if (!pvt->_volume) {
			return false;
		}

		// Get free space the old-school and well-supported way, by
		// sectors and clusters.  If it fails, fall back to some safe
		// defaults.
		DWORD	sectorspercluster;
		DWORD	bytespersector;
		DWORD	freeclusters;
		DWORD	totalclusters;
		if (GetDiskFreeSpace(pvt->_volume,&sectorspercluster,
						&bytespersector,
						&freeclusters,
						&totalclusters)==TRUE) {
			// calculate bytes, blocks and files available
			// (apparently cluster size = block size on windows)
			pvt->_st.f_bsize=sectorspercluster*bytespersector;
			pvt->_st.f_blocks=totalclusters;
			pvt->_st.f_bfree=freeclusters;
		}

		// get volume information
		DWORD	volumeserialnumber;
		if (!GetVolumeInformation(pvt->_volume,
						pvt->_st.f_volumename,
						PATH_MAX,
						&volumeserialnumber,
						&(pvt->_st.f_namelen),
						NULL,
						pvt->_st.f_typename,
						PATH_MAX)) {
			return false;
		}

		// build the volume serial number
		union fsid {
			int32_t	val32[2];
		};
		fsid	*f_fsid=(union fsid *)&(pvt->_st.f_fsid);
		f_fsid->val32[0]=HIWORD(volumeserialnumber);
		f_fsid->val32[1]=LOWORD(volumeserialnumber);

		// translate the filesystem type name to hex
		pvt->_st.f_type=0x0;
		if (!charstring::compare(pvt->_st.f_typename,"FAT") ||
			!charstring::compare(pvt->_st.f_typename,"FAT32")) {
			pvt->_st.f_type=0x4d44;
		} else if (!charstring::compare(pvt->_st.f_typename,"NTFS")) {
			pvt->_st.f_type=0x5346544e;
		} else if (!charstring::compare(pvt->_st.f_typename,"CDFS")) {
			pvt->_st.f_type=0x9660;
		}
		return true;
	#else
		#error no fstatvfs, fstatfs or anything like it
	#endif
}

int64_t filesystem::getType() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
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

int64_t filesystem::getBlockSize() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_bsize;
#elif defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	return pvt->_st.f_frsize;
#else
	return 0;
#endif
}

int64_t filesystem::getOptimumTransferBlockSize() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_bsize;
#elif defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	return pvt->_st.f_iosize;
#else
	return 0;
#endif
}

int64_t filesystem::getTotalBlocks() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
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

int64_t filesystem::getFreeBlocks() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
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

int64_t filesystem::getAvailableBlocks() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS)
	return pvt->_st.f_bavail;
#else
	return 0;
#endif
}

int64_t filesystem::getTotalFileNodes() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS)
	return pvt->_st.f_files;
#else
	return 0;
#endif
}

int64_t filesystem::getFreeFileNodes() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS)
	return pvt->_st.f_ffree;
#else
	return 0;
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

int64_t filesystem::getMaximumFileNameLength() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return pvt->_st.f_namelen;
#elif defined(RUDIMENTS_HAVE_STATVFS) || \
	defined(RUDIMENTS_HAVE_MINIX_HAIKU_STATVFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	return pvt->_st.f_namemax;
#else
	return 0;
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

const char *filesystem::getMountPoint() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_mntonname;
#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return (const char *)pvt->_volume;
#else
	return NULL;
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

int64_t filesystem::getAsyncReads() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS)
	return pvt->_st.f_asyncreads;
#else
	return 0;
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

const char *filesystem::getFilesystemSpecificString() const {
#ifdef RUDIMENTS_HAVE_STATVFS
	return (const char *)pvt->_st.f_fstr;
#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return (const char *)pvt->_st.f_volumename;
#else
	return NULL;
#endif
}

void *filesystem::getInternalFilesystemStatisticsStructure() {
	return (void *)&pvt->_st;
}

const char *filesystem::getTypeName() const {
#if defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS)
	return (const char *)pvt->_st.f_fstypename;
#elif defined(RUDIMENTS_HAVE_STATVFS)
	return (const char *)pvt->_st.f_basetype;
#elif defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS)
	switch (pvt->_st.f_type) {
		case 0xADF5:
			return "adfs";
		case 0xADFF:
			return "affs";
		case 0x42465331:
			return "befs";
		case 0x1BADFACE:
			return "bfs";
		case 0xFF534D42:
			return "cifs";
		case 0x73757245:
			return "codafs";
		case 0x012FF7B7:
			return "coherent";
		case 0x28cd3d45:
			return "cramfs";
		case 0x1373:
			return "devfs";
		case 0x00414A53:
			return "efs";
		case 0x137D:
			return "ext";
		case 0xEF51:
			return "ext2 (old)";
		case 0xEF53:
			return "ext2";
		case 0x4244:
			return "hfs";
		case 0xF995E849:
			return "hpfs";
		case 0x958458f6:
			return "hugetlbfs";
		case 0x9660:
			return "isofs";
		case 0x72b6:
			return "jffs2";
		case 0x3153464a:
			return "jfs";
		case 0x137F:
			return "minix (original)";
		case 0x138F:
			return "minix (30 char)";
		case 0x2468:
			return "minix2 (original)";
		case 0x2478:
			return "minix2 (30 char)";
		case 0x4d44:
			return "msdos";
		case 0x564c:
			return "ncp";
		case 0x6969:
			return "nfs";
		case 0x5346544e:
			return "ntfs";
		case 0x9fa1:
			return "openprom";
		case 0x9fa0:
			return "proc";
		case 0x002f:
			return "qnx4";
		case 0x52654973:
			return "reiserfs";
		case 0x7275:
			return "romfs";
		case 0x517B:
			return "smb";
		case 0x012FF7B6:
			return "sysv2";
		case 0x012FF7B5:
			return "sysv4";
		case 0x01021994:
			return "tmpfs";
		case 0x15013346:
			return "udf";
		case 0x00011954:
			return "ufs";
		case 0x9fa2:
			return "usbdevfs";
		case 0xa501FCF5:
			return "vxfs";
		case 0x012FF7B4:
			return "xenix";
		case 0x58465342:
			return "xfs";
		case 0x012FD16D:
			return "xiafs";
	}
	return NULL;
#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return (const char *)pvt->_st.f_typename;
#else
	return NULL;
#endif
}
