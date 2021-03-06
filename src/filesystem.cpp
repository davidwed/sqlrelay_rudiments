// Copyright (c) 2003 David Muse
// See the COPYING file for more information

#include <rudiments/filesystem.h>
#include <rudiments/bytestring.h>
#include <rudiments/directory.h>
#include <rudiments/error.h>
#include <rudiments/charstring.h>
#ifdef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
	#include <rudiments/filedescriptor.h>
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
	#ifdef RUDIMENTS_HAVE_PSAPI_H
		#define PSAPI_VERSION 1
		#include <psapi.h>
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

bool filesystem::open(const char *path) {
	close();
	if (charstring::isNullOrEmpty(path)) {
		path=directory::getCurrentWorkingDirectory();
	}
	#ifndef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
		pvt->_closeflag=true;
		error::clearError();
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

bool filesystem::open(int32_t fd) {
	close();
	#ifndef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
		pvt->_fd=fd;
		return getCurrentProperties();
	#else
		// This is complex on Windows.

		// We have to determine the file name that the file descriptor
		// refers to and call the other version of open().

		// On Vista and newer we can use GetFinalPathNameByHandle(),
		// but on older versions we have to memory-map the file, get
		// the file name from the map and convert the volume name to a
		// dos-style drive letter, so we'll just do that.

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

		// get file name
		// NOTE: If GetMappedFileName is dynamically loaded on
		// Windows 8 or higher then it corrupts the stack when it is
		// used.  It works fine on Windows 7 and lower though.
		char	filename[MAX_PATH+1];
		if (!GetMappedFileName(GetCurrentProcess(),
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
		char	mapping[MAX_PATH+1];

		// for each volume...
		for (char driveletter='c'; driveletter<='z'; driveletter++) {

			// ignore volumes that don't exist
			DWORD	exists=volumes&0x0001;
			volumes=volumes>>1;
			if (!exists) {
				continue;
			}

			// create the volume name, eg: C:
			volume[0]=driveletter;

			// get the drive mapping
			// FIXME: If the volume is mapped to more than 1
			// location then this could return 0 with
			// ERROR_INSUFFICIENT_BUFFER if mapping[] is too small
			// to store the list of locations.  In that case we
			// should loop back, resize mapping[] and try again.
			DWORD	result=QueryDosDevice(volume,mapping,
							MAX_PATH/sizeof(TCHAR));
			if (!result) {
				UnmapViewOfFile(fv);
				CloseHandle(fm);
				return false;
			}

			// if the drive mapping for this volume matches
			// then we're done
			// FIXME: If the volume is mapped to more than 1
			// location then we need to test the filename against
			// each of the mappings.
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
		
		// open using file name
		return (volume[0])?open(ptr):false;
	#endif
}

bool filesystem::close() {
	int32_t	result=0;
	#ifndef RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE
		if (pvt->_fd>-1 && pvt->_closeflag) {
			error::clearError();
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
	#if (defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATVFS) || \
			defined(RUDIMENTS_HAVE_SOME_KIND_OF_STATFS)) && \
			!defined(__VMS)

		int32_t	result;
		error::clearError();
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
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

int64_t filesystem::getType() const {
	int64_t	result=0;
#if defined(RUDIMENTS_HAVE_LINUX_STATFS) || \
	defined(RUDIMENTS_HAVE_LINUX_LIBC4_STATFS) || \
	defined(RUDIMENTS_HAVE_FREEBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_NETBSD_STATFS) || \
	defined(RUDIMENTS_HAVE_DARWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_CYGWIN_STATFS) || \
	defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	result=pvt->_st.f_type;
#endif
	if (result) {
		return result;
	}
	const char	*fstypename=NULL;
#if defined(RUDIMENTS_HAVE_STATVFS)
	fstypename=(const char *)pvt->_st.f_basetype;
#elif defined(RUDIMENTS_HAVE_NETBSD_STATVFS) || \
	defined(RUDIMENTS_HAVE_OPENBSD_STATFS)
	fstypename=(const char *)pvt->_st.f_fstypename;
#endif
	if (!charstring::compareIgnoringCase(
				fstypename,"adfs")) {
		return 0xADF5;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"affs")) {
		return 0xADFF;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"befs")) {
		return 0x42465331;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"bfs")) {
		return 0x1BADFACE;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"cifs")) {
		return 0xFF534D42;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"codafs")) {
		return 0x73757245;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"coherent")) {
		return 0x012FF7B7;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"cramfs")) {
		return 0x28cd3d45;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"devfs")) {
		return 0x1373;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"efs")) {
		return 0x00414A53;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"ext")) {
		return 0x137D;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"ext2 (old)")) {
		return 0xEF51;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"ext2")) {
		return 0xEF53;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"hfs")) {
		return 0x4244;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"hpfs")) {
		return 0xF995E849;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"htfs")) {
		return 0xA060492A;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"hugetlbfs")) {
		return 0x958458f6;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"isofs")) {
		return 0x9660;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"jffs2")) {
		return 0x72b6;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"jfs")) {
		return 0x3153464a;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"minix (original)")) {
		return 0x137F;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"minix (30 char)")) {
		return 0x138F;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"minix2 (original)")) {
		return 0x2468;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"minix2 (30 char)")) {
		return 0x2478;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"msdos")) {
		return 0x4d44;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"ncp")) {
		return 0x564c;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"nfs")) {
		return 0x6969;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"ntfs")) {
		return 0x5346544e;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"openprom")) {
		return 0x9fa1;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"proc")) {
		return 0x9fa0;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"qnx4")) {
		return 0x002f;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"reiserfs")) {
		return 0x52654973;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"romfs")) {
		return 0x7275;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"smb")) {
		return 0x517B;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"sysv2")) {
		return 0x012FF7B6;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"sysv4")) {
		return 0x012FF7B5;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"tmpfs")) {
		return 0x01021994;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"udf")) {
		return 0x15013346;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"ufs") ||
			!charstring::compareIgnoringCase(
				fstypename,"ffs")) {
		return 0x00011954;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"usbdevfs")) {
		return 0x9fa2;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"vxfs")) {
		return 0xa501FCF5;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"xenix")) {
		return 0x012FF7B4;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"xfs")) {
		return 0x58465342;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"xiafs")) {
		return 0x012FD16D;
	} else if (!charstring::compareIgnoringCase(
				fstypename,"zfs")) {
		return 0x00BAB10C;
	}
	return 0;
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

int64_t filesystem::getReservedBlocks() const {
#if defined(RUDIMENTS_HAVE_STATVFS_RESVD)
	return pvt->_st.f_bresvd;
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
	return pvt->_st.f_favail;
#else
	return 0;
#endif
}

int64_t filesystem::getReservedFileNodes() const {
#if defined(RUDIMENTS_HAVE_STATVFS_RESVD)
	return pvt->_st.f_fresvd;
#else
	return 0;
#endif
}

int64_t filesystem::getFileSystemId() const {
#if defined(RUDIMENTS_HAVE_LINUX_STATFS)
	// fsid_t is an opaque struct, but it typically contains an array of
	// some number of bytes which are collectively set to some unique value.
	// Attempt to handle various possibilities...
	if (sizeof(fsid_t)==sizeof(uint16_t)) {
		uint16_t	temp;
		bytestring::copy(&temp,&pvt->_st.f_fsid,sizeof(uint16_t));
		return (int64_t)temp;
	} else if (sizeof(fsid_t)==sizeof(uint32_t)) {
		uint32_t	temp;
		bytestring::copy(&temp,&pvt->_st.f_fsid,sizeof(uint32_t));
		return (int64_t)temp;
	} else {
		uint64_t	temp;
		bytestring::copy(&temp,&pvt->_st.f_fsid,sizeof(uint64_t));
		return (int64_t)temp;
	}
	return 0;
#elif defined(RUDIMENTS_HAVE_STATVFS) || \
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
	return (pvt->_st.f_mntonname[0])?
			(const char *)pvt->_st.f_mntonname:NULL;
#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return (!charstring::isNullOrEmpty(pvt->_volume))?
				(const char *)pvt->_volume:NULL;
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
	return (pvt->_st.f_mntfromname[0])?
			(const char *)pvt->_st.f_mntfromname:NULL;
#else
	return NULL;
#endif
}

const char *filesystem::getFilesystemSpecificString() const {
#ifdef RUDIMENTS_HAVE_STATVFS
	return (pvt->_st.f_fstr[0])?
			(const char *)pvt->_st.f_fstr:NULL;
#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return (pvt->_st.f_volumename[0])?
			(const char *)pvt->_st.f_volumename:NULL;
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
	return (pvt->_st.f_fstypename[0])?
			(const char *)pvt->_st.f_fstypename:NULL;
#elif defined(RUDIMENTS_HAVE_STATVFS)
	return (pvt->_st.f_basetype[0])?
			(const char *)pvt->_st.f_basetype:NULL;
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
		case 0xA060492A:
			return "htfs";
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
		case 0x00BAB10C:
			return "zfs";
	}
	return NULL;
#elif defined(RUDIMENTS_HAVE_WINDOWS_GETDISKFREESPACE)
	return (pvt->_st.f_typename[0])?
			(const char *)pvt->_st.f_typename:NULL;
#else
	return NULL;
#endif
}
