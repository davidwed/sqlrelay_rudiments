// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILESYSTEM_H
#define RUDIMENTS_FILESYSTEM_H

#include <rudiments/private/filesystemincludes.h>

// The filesystem class provides methods for discovering the properties
// associated with a filesystem such as type, total and free blocks, total and
// free inodes, etc.
//
// Use the static methods in this class if you need to get a a single property.
//
// If you need to get more that one property, create an instance of the class,
// call initialize(), then use the non-static methods to access the properties
// that you need.

class filesystem {
	public:
			filesystem();
			// Creates an uninitialized instance of the filesystem
			// class.
			~filesystem();

		int	initialize(const char *path);
			// Initializes the instance to use filesystem "path".
			//
			// Returns 1 on success and 0 on failure.
		int	initialize(int fd);
			// Initializes the instance to use already open file
			// descriptor "fd".
			//
			// Returns 1 on success and 0 on failure.

		// These methods return the filesystem type.
		// Linux, Net/Open/FreeBSD
		long		getType() const;
		static int	getType(const char *path, long *type);
		static int	getType(int fd, long *type);

		// These methods return the fundamental block size of the
		// filesystem.
		// Net/Open/FreeBSD, OSR5, Solaris
		long		getBlockSize() const;
		static int	getBlockSize(const char *path, long *size);
		static int	getBlockSize(int fd, long *size);

		// These methods return the optimum transfer block size.
		// Linux, Net/Open/FreeBSD, OSR5, Solaris
		long		getOptimumTransferBlockSize() const;
		static int	getOptimumTransferBlockSize(const char *path,
								long *size);
		static int	getOptimumTransferBlockSize(int fd, long *size);

		// These methods return the total number of blocks.
		// Linux, Net/Open/FreeBSD, OSR5, Solaris
		long		getTotalBlocks() const;
		static int	getTotalBlocks(const char *path, long *blocks);
		static int	getTotalBlocks(int fd, long *blocks);

		// These methods return the number of free blocks.
		// Linux, Net/Open/FreeBSD, OSR5, Solaris
		long		getFreeBlocks() const;
		static int	getFreeBlocks(const char *path, long *blocks);
		static int	getFreeBlocks(int fd, long *blocks);

		// These methods return the number of blocks available to 
		// non-superuser accounts.
		// Linux, Net/Open/FreeBSD, OSR5, Solaris
		long		getAvailableBlocks() const;
		static int	getAvailableBlocks(const char *path,
							long *blocks);
		static int	getAvailableBlocks(int fd, long *blocks);

		// These methods return the number of file nodes.
		// Linux, Net/Open/FreeBSD, OSR5, Solaris
		long		getTotalFileNodes() const;
		static int	getTotalFileNodes(const char *path,
							long *nodes);
		static int	getTotalFileNodes(int fd, long *nodes);

		// These methods return the number of free file nodes.
		// Linux, Net/Open/FreeBSD, OSR5, Solaris
		long		getFreeFileNodes() const;
		static int	getFreeFileNodes(const char *path,
							long *nodes);
		static int	getFreeFileNodes(int fd, long *nodes);

		// These methods return the number of file nodes available to 
		// non-superuser accounts.
		// OSR5, Solaris
		long		getAvailableFileNodes() const;
		static int	getAvailableFileNodes(const char *path,
							long *nodes);
		static int	getAvailableFileNodes(int fd, long *nodes);

#ifdef HAVE_STATVFS
		// These methods return the file system id.
		// OSR5, Solaris - long 
		long		getFileSystemId() const;
		static int	getFileSystemId(const char *path, long *id);
		static int	getFileSystemId(int fd, long *id);
#else
		// These methods return the file system id.
		// Linux, Net/Open/FreeBSD
		fsid_t		getFileSystemId() const;
		static int	getFileSystemId(const char *path, fsid_t *id);
		static int	getFileSystemId(int fd, fsid_t *id);
#endif

		// These methods return the maximum length of filenames.
		// Linux, OSR5, Solaris
		long		getMaximumFileNameLength() const;
		static int	getMaximumFileNameLength(const char *path,
								long *length);
		static int	getMaximumFileNameLength(int fd, long *length);

		// These methods return the user that mounted the filesystem.
		// Net/Open/FreeBSD
		uid_t		getOwner() const;
		static int	getOwner(const char *path, uid_t *owner);
		static int	getOwner(int fd, uid_t *owner);

		// These methods return the number of synchronous writes that
		// have occurred since the filesytem was mounted.
		// Net/Open/FreeBSD
		long		getSyncWrites() const;
		static int	getSyncWrites(const char *path,
							long *swrites);
		static int	getSyncWrites(int fd, long *swrites);

		// These methods return the number of asynchronous writes that
		// have occurred since the filesytem was mounted.
		// Net/Open/FreeBSD
		long		getAsyncWrites() const;
		static int	getAsyncWrites(const char *path,
							long *aswrites);
		static int	getAsyncWrites(int fd, long *sswrites);

		// These methods return the name of the filesystem type.
		// Net/Open/FreeBSD, OSR5, Solaris
		char		*getTypeName() const;
		static int	getTypeName(const char *path, char **name);
		static int	getTypeName(int fd, char **name);

		// These methods return the mount point of the filesystem.
		// Net/Open/FreeBSD
		char		*getMountPoint() const;
		static int	getMountPoint(const char *path, char **mtpt);
		static int	getMountPoint(int fd, char **mtpt);

		// These methods return the number of synchronous reads that
		// have occurred since the filesytem was mounted.
		// FreeBSD
		long		getSyncReads() const;
		static int	getSyncReads(const char *path, long *sreads);
		static int	getSyncReads(int fd, long *sreads);

		// These methods return the number of asynchronous reads that
		// have occurred since the filesytem was mounted.
		// FreeBSD
		long		getAsyncReads() const;
		static int	getAsyncReads(const char *path, long *asreads);
		static int	getAsyncReads(int fd, long *asreads);

		// These methods return the name of the device file for the
		// filesystem.
		// Net/OpenBSD
		char		*getDeviceName() const;
		static int	getDeviceName(const char *path, char **devname);
		static int	getDeviceName(int fd, char **devname);

		// These methods return the filesystem-specific string.
		// OSR5, Solaris
		char		*getFilesystemSpecificString() const;
		static int	getFilesystemSpecificString(const char *path,
								char **str);
		static int	getFilesystemSpecificString(int fd, char **str);


		// NetBSD: u_short f_oflags - deprecated copy of mount flags
		// NetBSD: u_short f_flags - mount flags
		// OpenBSD: union mount_info mount_info - per fs mount options
		// FreeBSD: int f_flags - mount flags
		// OSR5/statvfs: unsigned long f_flag - mount flags
		// solaris/statvfs: u_long f_flag - mount flags


		int	getCurrentProperties();
			// Gets the current filesystem properties.

		// This method returns a pointer to the statfs structure
		// used internally.
#ifdef HAVE_STATVFS
		struct statvfs	*getStatfs();
#else
		struct statfs	*getStatfs();
#endif

	#include <rudiments/private/filesystem.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/filesysteminlines.h>
#endif

#endif
