// Copyright (c) 2003 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILESYSTEM_H
#define RUDIMENTS_FILESYSTEM_H

#include <rudiments/private/filesystemincludes.h>

class filesystemprivate;

/** The filesystem class provides methods for discovering the properties
 *  associated with a filesystem such as type, total and free blocks, total and
 *  free inodes, etc.
 *
 *  The native methods for accessing filesystem data vary greatly between
 *  platforms and many of these methods only work at all on certain platforms.
 *  */
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
		 *  "path".  If "path" is NULL or an empty string then it the
		 *  instance is initialized using the current working directoy.
		 *
		 *  Returns true on success and false on failure. */
		bool	initialize(const char *path);

		/** Initializes the instance to use already open file
		 *  descriptor "fd".
		 *
		 *  Returns true on success and false on failure. */
		bool	initialize(int32_t fd);

		/** Returns a number representing the filesystem type. */
		int64_t		getType() const;

		/** Returns the fundamental block size of the filesystem. */
		int64_t		getBlockSize() const;

		/** Returns the optimum transfer block size. */
		int64_t		getOptimumTransferBlockSize() const;

		/** Returns the total number of blocks allocated for the
		 *  filesystem. */
		int64_t		getTotalBlocks() const;

		/** Returns the number of free blocks in the filesystem. */
		int64_t		getFreeBlocks() const;

		/** Returns the number of blocks available to non-superuser
		 *  accounts on the filesystem. */
		int64_t		getAvailableBlocks() const;

		/** Returns the number of file nodes on the filesystem. */
		int64_t		getTotalFileNodes() const;

		/** Returns the number of free file nodes. */
		int64_t		getFreeFileNodes() const;

		/** Returns the number of file nodes available to 
		 *  non-superuser accounts. */
		int64_t		getAvailableFileNodes() const;

		/** Returns the file system id. */
		int64_t		getFileSystemId() const;

		/** Returns the maximum length of filenames on the
		 *  filesystem. */
		int64_t		getMaximumFileNameLength() const;

		/** Returns the id of the user that mounted the filesystem. */
		uid_t		getOwner() const;

		/** Returns the number of synchronous writes that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getSyncWrites() const; 

		/** Returns the number of asynchronous writes that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getAsyncWrites() const;

		/** Returns the name of the filesystem type. */
		const char	*getTypeName() const;

		/** Returns the mount point of the filesystem. */
		const char	*getMountPoint() const;

		/** Returns the number of synchronous reads that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getSyncReads() const;

		/** Returns the number of asynchronous reads that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getAsyncReads() const;

		/** Returns the name of the device file for the filesystem. */
		const char	*getDeviceName() const;

		/** Returns the filesystem-specific string. */
		const char	*getFilesystemSpecificString() const;

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
