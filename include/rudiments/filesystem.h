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
		long		getType() const;
		static int	getType(const char *path, long *type);
		static int	getType(int fd, long *type);

		// These methods return the optimum transfer block size.
		long		getOptimumTransferBlockSize() const;
		static int	getOptimumTransferBlockSize(const char *path,
								long *size);
		static int	getOptimumTransferBlockSize(int fd, long *size);

		// These methods return the total number of blocks.
		long		getTotalBlocks() const;
		static int	getTotalBlocks(const char *path, long *blocks);
		static int	getTotalBlocks(int fd, long *blocks);

		// These methods return the number of free blocks.
		long		getFreeBlocks() const;
		static int	getFreeBlocks(const char *path, long *blocks);
		static int	getFreeBlocks(int fd, long *blocks);

		// These methods return the number of blocks available to 
		// non-superuser accounts.
		long		getAvailableBlocks() const;
		static int	getAvailableBlocks(const char *path,
							long *blocks);
		static int	getAvailableBlocks(int fd, long *blocks);

		// These methods return the number of file nodes.
		long		getTotalFileNodes() const;
		static int	getTotalFileNodes(const char *path,
							long *nodes);
		static int	getTotalFileNodes(int fd, long *nodes);

		// These methods return the number of free file nodes.
		long		getFreeFileNodes() const;
		static int	getFreeFileNodes(const char *path,
							long *nodes);
		static int	getFreeFileNodes(int fd, long *nodes);

		// These methods return the file system id.
		fsid_t		getFileSystemId() const;
		static int	getFileSystemId(const char *path, fsid_t *id);
		static int	getFileSystemId(int fd, fsid_t *id);

		// These methods return the maximum length of filenames.
		long		getMaximumFileNameLength() const;
		static int	getMaximumFileNameLength(const char *path,
								long *length);
		static int	getMaximumFileNameLength(int fd, long *length);

		int	getCurrentProperties();
			// Gets the current filesystem properties.

		// This method returns a pointer to the statfs structure
		// used internally.
		struct statfs	*getStatfs();

	#include <rudiments/private/filesystem.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/filesysteminlines.h>
#endif

#endif
