// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILESYSTEM_H
#define RUDIMENTS_FILESYSTEM_H

#include <rudiments/private/filesystemincludes.h>

// TODO: 
// NetBSD: u_short f_oflags - deprecated copy of mount flags
// NetBSD: u_short f_flags - mount flags
// OpenBSD: union mount_info mount_info - per fs mount options
// FreeBSD: int32_t f_flags - mount flags
// SCO OSR5: unsigned long f_flag - mount flags
// solaris: u_long f_flag - mount flags

class filesystemprivate;

/** The filesystem class provides methods for discovering the properties
 *  associated with a filesystem such as type, total and free blocks, total and
 *  free inodes, etc.
 *
 *  The native methods for accessing filesystem data vary greatly between
 *  platforms and many of these methods only work at all on certain platforms.
 *  For those methods, the platforms that they are known to work on are listed.
 * 
 *  Use the static methods in this class if you need to get a a single property.
 * 
 *  If you need to get more that one property, create an instance of the class,
 *  call initialize(), then use the non-static methods to access the properties
 *  that you need. */
class RUDIMENTS_DLLSPEC filesystem {
	public:
		/** Creates an instance of the filesystem class. */
		filesystem();

		/** Creates an instance of the filesystem class
 		 *  that is a copy of "f". */
		filesystem(const filesystem &f);

		/** Makes this instance of the filesystem class
		 *  identical to "f". */
		filesystem	&operator=(const filesystem &f);

		/** Deletes this instance of the filesystem class. */
		virtual		~filesystem();

		/** Initializes the instance to use the filesystem containing
		 *  "path".
		 *
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *path);

		/** Initializes the instance to use already open file
		 *  descriptor "fd".
		 *
		 *  Returns true on success and false on failure. */
		bool	initialize(int32_t fd);

		/** Returns a number representing the filesystem type.
		 *  (works on Linux and  Net/Open/FreeBSD). */
		int64_t		getType() const;

		/** Sets "type" to a number representing the type of the
		 *  filesystem containing "path".
		 *  (works on Linux and  Net/Open/FreeBSD).
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getType(const char *path, int64_t *type);

		/** Sets "type" to a number representing the type of the
		 *  filesystem containing the file represented by file
		 *  descriptor "fd".
		 *  (works on Linux and  Net/Open/FreeBSD).
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getType(int32_t fd, int64_t *type);

		/** Returns the fundamental block size of the filesystem.
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris) */
		int64_t		getBlockSize() const;

		/** Sets "size" to the fundamental block size of the filesystem
		 *  containing "path".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getBlockSize(const char *path, int64_t *size);

		/** Sets "size" to the fundamental block size of the filesystem
		 *  containing the file represented by the file descriptor "fd".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getBlockSize(int32_t fd, int64_t *size);

		/** Returns the optimum transfer block size.
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris) */
		int64_t		getOptimumTransferBlockSize() const;

		/** Sets "path" to the optimum transfer block size for the
		 *  filesystem contianing "path".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getOptimumTransferBlockSize(const char *path,
								int64_t *size);

		/** Sets "path" to the optimum transfer block size for the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getOptimumTransferBlockSize(int32_t fd,
								int64_t *size);

		/** Returns the total number of blocks allocated for the
		 *  filesystem.
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris) */
		int64_t		getTotalBlocks() const;

		/** Sets "blocks" to the total number of blocks allocated for
		 *  the filesystem containing "path".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getTotalBlocks(const char *path,
							int64_t *blocks);

		/** Sets "blocks" to the total number of blocks allocated for
		 *  the filesystem containing the file represented by the
		 *  file descriptor "fd".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getTotalBlocks(int32_t fd, int64_t *blocks);

		/** Returns the number of free blocks in the filesystem.
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris) */
		int64_t		getFreeBlocks() const;

		/** Sets "blocks" to the number of free blocks in the
		 *  filesystem containing "path".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFreeBlocks(const char *path,
							int64_t *blocks);

		/** Sets "blocks" to the number of free blocks in the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFreeBlocks(int32_t fd, int64_t *blocks);

		/** Returns the number of blocks available to non-superuser
		 *  accounts on the filesystem.
		 * (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris) */
		int64_t		getAvailableBlocks() const;

		/** Sets "blocks" to the number of blocks available to
		 *  non-superuser accounts on the filesystem containing "path".
		 * (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getAvailableBlocks(const char *path,
							int64_t *blocks);

		/** Sets "blocks" to the number of blocks available to
		 *  non-superuser accounts on the filesystem containing the file
		 *  represented by the file descriptor "fd".
		 * (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getAvailableBlocks(int32_t fd, int64_t *blocks);

		/** Returns the number of file nodes on the filesystem.
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris) */
		int64_t		getTotalFileNodes() const;

		/** Sets "nodes" to the number of file nodes on the filesystem
 		 *  containing "path".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getTotalFileNodes(const char *path,
							int64_t *nodes);

		/** Sets "nodes" to the number of file nodes on the filesystem
		 *  containing the file represented by the file descriptor "fd".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getTotalFileNodes(int32_t fd, int64_t *nodes);

		/** Returns the number of free file nodes.
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris) */
		int64_t		getFreeFileNodes() const;

		/** Sets "nodes" to the number of free file nodes on the
		 *  filesystem containing "path".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFreeFileNodes(const char *path,
							int64_t *nodes);

		/** Sets "nodes" to the number of free file nodes on the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on Linux, Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFreeFileNodes(int32_t fd, int64_t *nodes);

		/** Returns the number of file nodes available to 
		 *  non-superuser accounts.
		 *  (works on SCO OSR5, Solaris) */
		int64_t		getAvailableFileNodes() const;

		/** Sets "nodes" to the number of file nodes available to 
		 *  non-superuser accounts on the filesystem containing "path".
		 *  (works on SCO OSR5, Solaris) */
		static bool	getAvailableFileNodes(const char *path,
							int64_t *nodes);
		static bool	getAvailableFileNodes(int32_t fd,
							int64_t *nodes);

		/** Returns the file system id.
		 *  (works on SCO OSR5, Solaris) */
		int64_t		getFileSystemId() const;

		/** Sets "id" to the file system id of the filesystem
		 *  containing "path".
		 *  (works on SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFileSystemId(const char *path, int64_t *id);

		/** Sets "id" to the file system id of the filesystem
		 *  containing the file represented by the file descriptor "fd".
		 *  (works on SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFileSystemId(int32_t fd, int64_t *id);

		/** Returns the maximum length of filenames on the filesystem.
		 *  (works on Linux, SCO OSR5, Solaris) */
		int64_t		getMaximumFileNameLength() const;

		/** Sets "length" to the maximum length of filenames on the
		 *  filesystem containing "path".
		 *  (works on Linux, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getMaximumFileNameLength(const char *path,
							int64_t *length);

		/** Sets "length" to the maximum length of filenames on the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on Linux, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getMaximumFileNameLength(int32_t fd,
							int64_t *length);

		/** Returns the id of the user that mounted the filesystem.
		 *  (works on Net/Open/FreeBSD) */
		uid_t		getOwner() const;

		/** Sets "owner" to the id of the user that mounted the
		 *  filesystem containing "path".
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getOwner(const char *path, uid_t *owner);

		/** Sets "owner" to the id of the user that mounted the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getOwner(int32_t fd, uid_t *owner);

		/** Returns the number of synchronous writes that
		 *  have occurred since the filesytem was mounted.
		 *  (works on Net/Open/FreeBSD) */
		int64_t		getSyncWrites() const;

		/** Sets "swrites" to the number of synchronous writes that
		 *  have occurred since the filesytem containing "path" was
		 *  mounted.
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getSyncWrites(const char *path,
							int64_t *swrites);

		/** Sets "swrites" to the number of synchronous writes that
		 *  have occurred since the filesytem containing the file
		 *  represented by the file descriptor "fd" was mounted.
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getSyncWrites(int32_t fd, int64_t *swrites);

		/** Returns the number of asynchronous writes that
		 *  have occurred since the filesytem was mounted.
		 *  (works on Net/Open/FreeBSD) */
		int64_t		getAsyncWrites() const;

		/** Sets "aswrites" to the number of asynchronous writes that
		 *  have occurred since the filesytem containing "path"
		 *  was mounted.
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getAsyncWrites(const char *path,
							int64_t *aswrites);

		/** Sets "aswrites" to the number of asynchronous writes that
		 *  have occurred since the filesytem containing the file
		 *  represented by the file descriptor "fd" was mounted.
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getAsyncWrites(int32_t fd, int64_t *aswrites);

		/** Returns the name of the filesystem type.
		 *  (works on Net/Open/FreeBSD, SCO OSR5, Solaris) */
		const char	*getTypeName() const;

		/** Sets "name" to the name of the filesystem type of the
		 *  filesystem containing "path".
		 *  (works on Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getTypeName(const char *path,
						const char **name);

		/** Sets "name" to the name of the filesystem type of the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on Net/Open/FreeBSD, SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getTypeName(int32_t fd, const char **name);

		/** Returns the mount point of the filesystem.
		 *  (works on Net/Open/FreeBSD) */
		const char	*getMountPoint() const;

		/** Sets "mtpt" to the mount point of the filesystem containing
		 *  "path".
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getMountPoint(const char *path,
							const char **mtpt);

		/** Sets "mtpt" to the mount point of the filesystem containing
		 *  the file represented by the file descriptor "fd".
		 *  (works on Net/Open/FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getMountPoint(int32_t fd, const char **mtpt);

		/** Returns the number of synchronous reads that
		 *  have occurred since the filesytem was mounted.
		 *  (works on FreeBSD) */
		int64_t		getSyncReads() const;

		/** Sets "sreads" to the number of synchronous reads that
		 *  have occurred since the filesytem containing "path"
		 *  was mounted.
		 *  (works on FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getSyncReads(const char *path, int64_t *sreads);

		/** Sets "sreads" to the number of synchronous reads that
		 *  have occurred since the filesytem containing the file
		 *  represented by the file descriptor "fd" was mounted.
		 *  (works on FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getSyncReads(int32_t fd, int64_t *sreads);

		/** Returns the number of asynchronous reads that
		 *  have occurred since the filesytem was mounted.
		 *  (works on FreeBSD) */
		int64_t		getAsyncReads() const;

		/** Sets "asreads" to the number of asynchronous reads that
		 *  have occurred since the filesytem containing "path"
		 *  was mounted.
		 *  (works on FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getAsyncReads(const char *path,
							int64_t *asreads);

		/** Sets "asreads" to the number of asynchronous reads that
		 *  have occurred since the filesytem containing the file
		 *  represented by the file descriptor "fd" was mounted.
		 *  (works on FreeBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getAsyncReads(int32_t fd, int64_t *asreads);

		/** Returns the name of the device file for the filesystem.
		 *  (works on Net/OpenBSD) */
		const char	*getDeviceName() const;

		/** Sets "devname" to the name of the device file for the
		 *  filesystem containing "path".
		 *  (works on Net/OpenBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getDeviceName(const char *path,
							const char **devname);

		/** Sets "devname" to the name of the device file for the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on Net/OpenBSD)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getDeviceName(int32_t fd, const char **devname);

		/** Returns the filesystem-specific string.
		 *  (works on SCO OSR5, Solaris) */
		const char	*getFilesystemSpecificString() const;

		/** Sets "str" to the filesystem-specific string for the
		 *  filesystem containing "path".
		 *  (works on SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFilesystemSpecificString(const char *path,
							const char **str);

		/** Sets "str" to the filesystem-specific string for the
		 *  filesystem containing the file represented by the file
		 *  descriptor "fd".
		 *  (works on SCO OSR5, Solaris)
		 *
		 *  Returns true on success and false otherwise. */
		static bool	getFilesystemSpecificString(int32_t fd,
							const char **str);


		/** Refreshes the current filesystem properties as returned
		 *  by the various methods of this class.
		 *  Returns true on success and false on failure. */
		bool	getCurrentProperties();

		/** Returns a pointer to the filesystem stats structure
		 *  used internally.  (This is VERY system-specific and could
		 *  be struct statfs *, struct statvfs * or something else.) */
		void	*getInternalFilesystemStatisticsStructure();

	#include <rudiments/private/filesystem.h>
};

#endif
