// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef FILEPROPERTIES_H
#define FILEPROPERTIES_H

#include <rudiments/private/config.h>

//#define __USE_UNIX98
#include <sys/types.h>
#include <sys/stat.h>

// The fileproperties class provides methods for discovering the
// properties associated with a file such as permissions, ownership,
// size, etc.
//
// Use one of the static methods in this class if you just need to get a a
// single property.
//
// If you need to get more that one property, create an instance of the class,
// call getFileProperties(), then use the non-static methods to access
// the properties that you need.

class fileproperties {
	public:
			fileproperties();


		int	getFileProperties(const char *filename);
			// Returns 1 if the properties of "filename" could
			// be read and 0 on error.
		int	getFileProperties(int fd);
			// Returns 1 of the properties of the file associated
			// with file descriptor "fd" could be read and 0 on
			// error.


		// These methods return the permissions of the file.
		int		getPermissions(mode_t *perms) const;
		static int	getPermissions(const char *filename,
						mode_t *perms);
		static int	getPermissions(int fd, mode_t *perms);


		// These methods return the user id of the file's owner.
		int		getOwnerUserId(uid_t *uid) const;
		static int	getOwnerUserId(const char *filename,
						uid_t *uid);
		static int	getOwnerUserId(int fd, uid_t *uid);


		// These methods return the group id of the file's owner.
		int		getOwnerGroupId(gid_t *gid) const;
		static int	getOwnerGroupId(const char *filename,
						gid_t *gid);
		static int	getOwnerGroupId(int fd, gid_t *gid);


		// These methods return the number of bytes in the file.
		int		getSize(off_t *size) const;
		static int	getSize(const char *filename,
						off_t *size);
		static int	getSize(int fd, off_t *size);


		// These methods return the number of
		// blocks allocated for the file.
		int		getBlockCount(blkcnt_t *blocks) const;
		static int	getBlockCount(const char *filename,
						blkcnt_t *blocks);
		static int	getBlockCount(int fd, blkcnt_t *blocks);


		// These methods return 1 of the file is a
		// socket or 0 if it's not.
		int		isSocket() const;
		static int	isSocket(const char *filename);
		static int	isSocket(int fd);

		// These methods return 1 of the file is a
		// symbolic link or 0 if it's not.
		int		isSymbolicLink() const;
		static int	isSymbolicLink(const char *filename);
		static int	isSymbolicLink(int fd);

		// These methods return 1 of the file is a
		// regular file or 0 if it's not.
		int		isRegularFile() const;
		static int	isRegularFile(const char *filename);
		static int	isRegularFile(int fd);

		// These methods return 1 of the file is a
		// block device or 0 if it's not.
		int		isBlockDevice() const;
		static int	isBlockDevice(const char *filename);
		static int	isBlockDevice(int fd);

		// These methods return 1 of the file is a
		// directory or 0 if it's not.
		int		isDirectory() const;
		static int	isDirectory(const char *filename);
		static int	isDirectory(int fd);

		// These methods return 1 of the file is a
		// character device or 0 if it's not.
		int		isCharacterDevice() const;
		static int	isCharacterDevice(const char *filename);
		static int	isCharacterDevice(int fd);

		// These methods return 1 of the file is a
		// fifo or 0 if it's not.
		int		isFifo() const;
		static int	isFifo(const char *filename);
		static int	isFifo(int fd);


		// These methods return the time of last access.
		int		getLastAccessTime(time_t *atime) const;
		static int	getLastAccessTime(const char *filename,
							time_t *atime);
		static int	getLastAccessTime(int fd, time_t *atime);


		// These methods return the time of last modification.
		//
		// Modification time IS NOT affected by chmod, chown,
		// or creating links.
		int		getLastModificationTime(time_t *mtime) const;
		static int	getLastModificationTime(const char *filename,
							time_t *mtime);
		static int	getLastModificationTime(int fd, time_t *mtime);


		// These methods return the time of last change.
		//
		// Change time IS affected by chmod, chown and creating links.
		int		getLastChangeTime(time_t *ctime) const;
		static int	getLastChangeTime(const char *filename,
							time_t *ctime);
		static int	getLastChangeTime(int fd, time_t *ctime);


		// These methods return the device of the file.
		int		getDevice(dev_t *dev) const;
		static int	getDevice(const char *filename, dev_t *dev);
		static int	getDevice(int fd, dev_t *dev);


		// These methods return the device type of the file.
		int		getDeviceType(dev_t *devtype) const;
		static int	getDeviceType(const char *filename,
							dev_t *devtype);
		static int	getDeviceType(int fd, dev_t *devtype);


		// These methods return the inode of the file.
		int		getInode(ino_t *inode) const;
		static int	getInode(const char *filename, ino_t *inode);
		static int  	getInode(int fd, ino_t *inode);


		// These methods return the number of hard links to the file.
		int		getNumberOfHardLinks(nlink_t *nlink) const;
		static int	getNumberOfHardLinks(const char *filename,
							nlink_t *nlink);
		static int	getNumberOfHardLinks(int fd, nlink_t *nlink);


		// This method returns a pointer to the stat structure
		// used internally.
		struct stat	*getStat();

	#include <rudiments/private/fileproperties.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/filepropertiesinlines.h>
#endif

#endif
