// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef FILE_H
#define FILE_H

#include <rudiments/private/config.h>

#include <rudiments/filedescriptor.h>
#include <sys/types.h>
#include <sys/stat.h>

// The file class provides methods for interacting with files and for 
// discovering the properties associated with a file such as permissions,
// ownership, size, etc.
//
// Use the static methods in this class if you need to get a a single property.
//
// If you need to get more that one property, create an instance of the class,
// call open(), then use the non-static methods to access the properties that
// you need.

class file : public filedescriptor {
	public:
			file();
			file(int fd);

		int	open(const char *name, int flags);
			// Opens file "name" using "flags".  Once the file is
			// open, it also gets it's properties.
			//
			// Returns 1 on success and 0 on failure.

		int	open(const char *name, int flags, mode_t perms);
			// Opens file "name" using "flags".  If the file
			// doesn't already exist and flags contains O_CREAT,
			// the file will be created with permissions "perms".
			// Once the file is open, it also gets it's properties.
			//
			// Returns 1 on success and 0 on failure.

		int	create(const char *name, mode_t perms);
			// Creates the file "name" with permissions "perms"
			// If the file already exists, it will be truncated.
			// Once the file is created, it also gets it's
			// properties.
			//
			// Returns 1 on success and 0 on failure.

		// These methods create the file "name" with permissions
		// "perms".  If the the file already exists, it will be
		// truncated.  The initial contents of the file are set to
		// "data".
		//
		// These methods return the number of bytes written to the file
		// or -1 if an error occurs.
		size_t	create(const char *name, mode_t perms,
						unsigned short number);
			// Sets the initial contents "number".
		size_t	create(const char *name, mode_t perms,
						unsigned long number);
			// Sets the initial contents "number".
		size_t	create(const char *name, mode_t perms, float number);
			// Sets the initial contents "number".
		size_t	create(const char *name, mode_t perms, double number);
			// Sets the initial contents "number".
		size_t	create(const char *name, mode_t perms, char number);
			// Sets the initial contents "number".
		size_t	create(const char *name, mode_t perms,
					const char *string);
			// Sets the initial contents to "string".
		size_t	create(const char *name, mode_t perms,
					const char *string, size_t size);
			// Sets the initial contents to "string" of size "size".
		size_t	create(const char *name, mode_t perms,
					const void *data, size_t size);
			// Sets the initial contents to "data" of size "size".


		// These methods allow you to set the position at which the
		// next read or write will occur.  Each returns the position
		// relative to the beginning of the file on success or -1 on
		// failure.
		off_t	setPositionRelativeToBeginning(off_t offset);
			// Sets the position to the beginning of the
			// file + "offset" bytes.
		off_t	setPositionRelativeToCurrent(off_t offset);
			// Sets the position to the current
			// position + "offset" bytes.
		off_t	setPositionRelativeToEnd(off_t offset);
			// Sets the position to the end of the
			// file + "offset" bytes.


		char	*getContents();
			// Allocates a string large enough to accommodate the
			// contents of the currently opened file, reads the
			// contents of the file into the string and returns the
			// string.  The string must be freed by the calling
			// program.
			//
			// If the file contains no data, then a string of length
			// zero is returned.  If an error occurs then a NULL
			// is returned.

		static	char	*getContents(const char *name);
			// Allocates a string large enough to accommodate the
			// contents of the file "name" , reads the contents of
			// the file into the string and returns the string.
			// The string must be freed by the calling program.
			//
			// If the file contains no data, then a string of length
			// zero is returned.  If an error occurs then a NULL
			// is returned.

		int	getCurrentProperties();
			// Gets the current file properties.

		// These methods return the permissions of the file.
		mode_t		getPermissions() const;
		static int	getPermissions(const char *filename,
						mode_t *perms);
		static int	getPermissions(int fd, mode_t *perms);

		// These methods return the user id of the file's owner.
		uid_t		getOwnerUserId() const;
		static int	getOwnerUserId(const char *filename,
						uid_t *uid);
		static int	getOwnerUserId(int fd, uid_t *uid);


		// These methods return the group id of the file's owner.
		gid_t		getOwnerGroupId() const;
		static int	getOwnerGroupId(const char *filename,
						gid_t *gid);
		static int	getOwnerGroupId(int fd, gid_t *gid);


		// These methods return the number of bytes in the file.
		off_t		getSize() const;
		static int	getSize(const char *filename,
						off_t *size);
		static int	getSize(int fd, off_t *size);


		// These methods return the number of
		// blocks allocated for the file.
		blkcnt_t	getBlockCount() const;
		static int	getBlockCount(const char *filename,
						blkcnt_t *blocks);
		static int	getBlockCount(int fd, blkcnt_t *blocks);


		// These methods return 1 of the file is a
		// socket or 0 if it's not or -1 on error.
		int		isSocket() const;
		static int	isSocket(const char *filename);
		static int	isSocket(int fd);

		// These methods return 1 of the file is a
		// symbolic link, 0 if it's not or -1 on error.
		int		isSymbolicLink() const;
		static int	isSymbolicLink(const char *filename);
		static int	isSymbolicLink(int fd);

		// These methods return 1 of the file is a
		// regular file, 0 if it's not or -1 on error.
		int		isRegularFile() const;
		static int	isRegularFile(const char *filename);
		static int	isRegularFile(int fd);

		// These methods return 1 of the file is a
		// block device, 0 if it's not or -1 on error.
		int		isBlockDevice() const;
		static int	isBlockDevice(const char *filename);
		static int	isBlockDevice(int fd);

		// These methods return 1 of the file is a
		// directory, 0 if it's not or -1 on error.
		int		isDirectory() const;
		static int	isDirectory(const char *filename);
		static int	isDirectory(int fd);

		// These methods return 1 of the file is a
		// character device, 0 if it's not or -1 on error.
		int		isCharacterDevice() const;
		static int	isCharacterDevice(const char *filename);
		static int	isCharacterDevice(int fd);

		// These methods return 1 of the file is a
		// fifo, 0 if it's not or -1 on error.
		int		isFifo() const;
		static int	isFifo(const char *filename);
		static int	isFifo(int fd);


		// These methods return the time of last access.
		time_t		getLastAccessTime() const;
		static int	getLastAccessTime(const char *filename,
							time_t *atime);
		static int	getLastAccessTime(int fd, time_t *atime);


		// These methods return the time of last modification.
		//
		// Modification time IS NOT affected by chmod, chown,
		// or creating links.
		time_t		getLastModificationTime() const;
		static int	getLastModificationTime(const char *filename,
							time_t *mtime);
		static int	getLastModificationTime(int fd, time_t *mtime);


		// These methods return the time of last change.
		//
		// Change time IS affected by chmod, chown and creating links.
		time_t		getLastChangeTime() const;
		static int	getLastChangeTime(const char *filename,
							time_t *ctime);
		static int	getLastChangeTime(int fd, time_t *ctime);


		// These methods return the device of the file.
		dev_t		getDevice() const;
		static int	getDevice(const char *filename, dev_t *dev);
		static int	getDevice(int fd, dev_t *dev);


		// These methods return the device type of the file.
		dev_t		getDeviceType() const;
		static int	getDeviceType(const char *filename,
							dev_t *devtype);
		static int	getDeviceType(int fd, dev_t *devtype);


		// These methods return the inode of the file.
		ino_t		getInode() const;
		static int	getInode(const char *filename, ino_t *inode);
		static int  	getInode(int fd, ino_t *inode);


		// These methods return the number of hard links to the file.
		nlink_t		getNumberOfHardLinks() const;
		static int	getNumberOfHardLinks(const char *filename,
							nlink_t *nlink);
		static int	getNumberOfHardLinks(int fd, nlink_t *nlink);

		// This method returns a pointer to the stat structure
		// used internally.
		struct stat	*getStat();

	#include <rudiments/private/file.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/file.h>
#endif

#endif
