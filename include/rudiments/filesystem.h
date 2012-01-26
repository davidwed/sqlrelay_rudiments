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

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class filesystemprivate;

class filesystem {
	public:
				filesystem();
				// Creates an uninitialized instance of the
				// filesystem class.
				filesystem(const filesystem &f);
		filesystem	&operator=(const filesystem &f);
		virtual		~filesystem();

		bool	initialize(const char *path);
			// Initializes the instance to use filesystem "path".
			//
			// Returns true on success and false on failure.
		bool	initialize(int fd);
			// Initializes the instance to use already open file
			// descriptor "fd".
			//
			// Returns true on success and false on failure.

		// These methods return the filesystem type.
		// (works on Linux, Net/Open/FreeBSD)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getType() const;
		static bool	getType(const char *path, int64_t *type);
		static bool	getType(int fd, int64_t *type);

		// These methods return the fundamental block size of the
		// filesystem.
		// (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getBlockSize() const;
		static bool	getBlockSize(const char *path, int64_t *size);
		static bool	getBlockSize(int fd, int64_t *size);

		// These methods return the optimum transfer block size.
		// (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getOptimumTransferBlockSize() const;
		static bool	getOptimumTransferBlockSize(const char *path,
								int64_t *size);
		static bool	getOptimumTransferBlockSize(int fd,
								int64_t *size);

		// These methods return the total number of blocks.
		// (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getTotalBlocks() const;
		static bool	getTotalBlocks(const char *path,
							int64_t *blocks);
		static bool	getTotalBlocks(int fd, int64_t *blocks);

		// These methods return the number of free blocks.
		// (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getFreeBlocks() const;
		static bool	getFreeBlocks(const char *path,
							int64_t *blocks);
		static bool	getFreeBlocks(int fd, int64_t *blocks);

		// These methods return the number of blocks available to 
		// non-superuser accounts.
		// (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getAvailableBlocks() const;
		static bool	getAvailableBlocks(const char *path,
							int64_t *blocks);
		static bool	getAvailableBlocks(int fd, int64_t *blocks);

		// These methods return the number of file nodes.
		// (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getTotalFileNodes() const;
		static bool	getTotalFileNodes(const char *path,
							int64_t *nodes);
		static bool	getTotalFileNodes(int fd, int64_t *nodes);

		// These methods return the number of free file nodes.
		// (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getFreeFileNodes() const;
		static bool	getFreeFileNodes(const char *path,
							int64_t *nodes);
		static bool	getFreeFileNodes(int fd, int64_t *nodes);

		// These methods return the number of file nodes available to 
		// non-superuser accounts.
		// (works on SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getAvailableFileNodes() const;
		static bool	getAvailableFileNodes(const char *path,
								int64_t *nodes);
		static bool	getAvailableFileNodes(int fd, int64_t *nodes);

		// These methods return the file system id.
		// (works on SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getFileSystemId() const;
		static bool	getFileSystemId(const char *path, int64_t *id);
		static bool	getFileSystemId(int fd, int64_t *id);

		// These methods return the maximum length of filenames.
		// (works on Linux, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getMaximumFileNameLength() const;
		static bool	getMaximumFileNameLength(const char *path,
							int64_t *length);
		static bool	getMaximumFileNameLength(int fd,
							int64_t *length);

		// These methods return the user that mounted the filesystem.
		// (works on Net/Open/FreeBSD)
		//
		// The static methods return true on success and false on
		// failure.
		uid_t		getOwner() const;
		static bool	getOwner(const char *path, uid_t *owner);
		static bool	getOwner(int fd, uid_t *owner);

		// These methods return the number of synchronous writes that
		// have occurred since the filesytem was mounted.
		// (works on Net/Open/FreeBSD)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getSyncWrites() const;
		static bool	getSyncWrites(const char *path,
							int64_t *swrites);
		static bool	getSyncWrites(int fd, int64_t *swrites);

		// These methods return the number of asynchronous writes that
		// have occurred since the filesytem was mounted.
		// (works on Net/Open/FreeBSD)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getAsyncWrites() const;
		static bool	getAsyncWrites(const char *path,
							int64_t *aswrites);
		static bool	getAsyncWrites(int fd, int64_t *aswrites);

		// These methods return the name of the filesystem type.
		// (works on Net/Open/FreeBSD, SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		const char	*getTypeName() const;
		static bool	getTypeName(const char *path,
						const char **name);
		static bool	getTypeName(int fd, const char **name);

		// These methods return the mount point of the filesystem.
		// (works on Net/Open/FreeBSD)
		//
		// The static methods return true on success and false on
		// failure.
		const char	*getMountPoint() const;
		static bool	getMountPoint(const char *path,
							const char **mtpt);
		static bool	getMountPoint(int fd, const char **mtpt);

		// These methods return the number of synchronous reads that
		// have occurred since the filesytem was mounted.
		// (works on FreeBSD)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getSyncReads() const;
		static bool	getSyncReads(const char *path, int64_t *sreads);
		static bool	getSyncReads(int fd, int64_t *sreads);

		// These methods return the number of asynchronous reads that
		// have occurred since the filesytem was mounted.
		// (works on FreeBSD)
		//
		// The static methods return true on success and false on
		// failure.
		int64_t		getAsyncReads() const;
		static bool	getAsyncReads(const char *path,
							int64_t *asreads);
		static bool	getAsyncReads(int fd, int64_t *asreads);

		// These methods return the name of the device file for the
		// filesystem.
		// (works on Net/OpenBSD)
		//
		// The static methods return true on success and false on
		// failure.
		const char	*getDeviceName() const;
		static bool	getDeviceName(const char *path,
							const char **devname);
		static bool	getDeviceName(int fd, const char **devname);

		// These methods return the filesystem-specific string.
		// (works on SCO OSR5, Solaris)
		//
		// The static methods return true on success and false on
		// failure.
		const char	*getFilesystemSpecificString() const;
		static bool	getFilesystemSpecificString(const char *path,
							const char **str);
		static bool	getFilesystemSpecificString(int fd,
							const char **str);


		// NetBSD: u_short f_oflags - deprecated copy of mount flags
		// NetBSD: u_short f_flags - mount flags
		// OpenBSD: union mount_info mount_info - per fs mount options
		// FreeBSD: int f_flags - mount flags
		// SCO OSR5: unsigned long f_flag - mount flags
		// solaris: u_long f_flag - mount flags


		bool	getCurrentProperties();
			// Gets the current filesystem properties.
			//
			// Returns true on success and false on failure.

		// This method returns a pointer to the statfs structure
		// used internally.
		void	*getInternalFilesystemStatisticsStructure();

	#include <rudiments/private/filesystem.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
