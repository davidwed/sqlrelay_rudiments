// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILE_H
#define RUDIMENTS_FILE_H

#include <rudiments/private/fileincludes.h>

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
			// Creates an uninitialized instance of the file class.
			file(int fd);
			// Creates an uninitialized instance of the file class
			// and associates already open file descriptor "fd"
			// with it.

		bool	open(const char *name, int flags);
			// Opens file "name" using "flags".  Once the file is
			// open, it also gets it's properties.
			//
			// Returns true on success and false on failure.

		bool	open(const char *name, int flags, mode_t perms);
			// Opens file "name" using "flags".  If the file
			// doesn't already exist and flags contains O_CREAT,
			// the file will be created with permissions "perms".
			// Once the file is open, it also gets it's properties.
			//
			// Returns true on success and false on failure.

		bool	create(const char *name, mode_t perms);
			// Creates the file "name" with permissions "perms"
			// If the file already exists, it will be truncated.
			// Once the file is created, it also gets it's
			// properties.
			//
			// Returns true on success and false on failure.

		// These methods create the file "name" with permissions
		// "perms".  If the the file already exists, it will be
		// truncated.  The initial contents of the file are set to
		// "data".
		//
		// These methods return the number of bytes written to the file
		// or -1 if an error occurs.
		ssize_t	create(const char *name, mode_t perms,
						unsigned short number);
			// Sets the initial contents "number".
		ssize_t	create(const char *name, mode_t perms,
						unsigned long number);
			// Sets the initial contents "number".
		ssize_t	create(const char *name, mode_t perms, float number);
			// Sets the initial contents "number".
		ssize_t	create(const char *name, mode_t perms, double number);
			// Sets the initial contents "number".
		ssize_t	create(const char *name, mode_t perms, char number);
			// Sets the initial contents "number".
		ssize_t	create(const char *name, mode_t perms,
					const char *string);
			// Sets the initial contents to "string".
		ssize_t	create(const char *name, mode_t perms,
					const char *string, size_t size);
			// Sets the initial contents to "string" of size "size".
		ssize_t	create(const char *name, mode_t perms,
					const void *data, size_t size);
			// Sets the initial contents to "data" of size "size".


		// These methods allow you to lock the entire file.
		bool	tryLockFile(short type);
		bool	lockFile(short type);
		bool	checkLockFile(short type, struct flock *lck);
		bool	unlockFile(short type);

		// These methods allow you to lock an arbitrary region of the
		// file.
		bool	tryLockRegion(short type, off_t start, off_t len);
		bool	lockRegion(short type, off_t start, off_t len);
		bool	checkLockRegion(short type, off_t start, off_t len,
							struct flock *lck);
		bool	unlockRegion(off_t start, off_t len);

		// These methods allow you to lock an arbitrary region of the
		// file relative to the current position.
		bool	tryLockFromCurrent(short type, off_t len);
		bool	tryLockFromCurrent(short type, off_t start, off_t len);
		bool	lockFromCurrent(short type, off_t len);
		bool	lockFromCurrent(short type, off_t start, off_t len);
		bool	checkLockFromCurrent(short type, off_t len,
							struct flock *lck);
		bool	checkLockFromCurrent(short type, off_t start, off_t len,
							struct flock *lck);
		bool	unlockFromCurrent(off_t len);
		bool	unlockFromCurrent(off_t start, off_t len);

		// These methods allow you to lock an arbitrary region of the
		// file relative to the end of the file.
		bool	tryLockFromEnd(short type, off_t len);
		bool	tryLockFromEnd(short type, off_t start, off_t len);
		bool	lockFromEnd(short type, off_t len);
		bool	lockFromEnd(short type, off_t start, off_t len);
		bool	checkLockFromEnd(short type, off_t len,
							struct flock *lck);
		bool	checkLockFromEnd(short type, off_t start, off_t len,
							struct flock *lck);
		bool	unlockFromEnd(off_t len);
		bool	unlockFromEnd(off_t start, off_t len);

		// These methods allow you to lock "the remainder" of a file
		// starting at a given offset.
		bool	tryLockRemainder(short type, off_t start);
		bool	lockRemainder(short type, off_t start);
		bool	checkLockRemainder(short type, off_t start,
							struct flock *lck);
		bool	unlockRemainder(off_t start);

		// These methods allow you to lock "the remainder" of a file
		// relative to the current position.
		bool	tryLockRemainderFromCurrent(short type);
		bool	tryLockRemainderFromCurrent(short type, off_t start);
		bool	lockRemainderFromCurrent(short type);
		bool	lockRemainderFromCurrent(short type, off_t start);
		bool	checkLockRemainderFromCurrent(short type,
							struct flock *lck);
		bool	checkLockRemainderFromCurrent(short type, off_t start,
							struct flock *lck);
		bool	unlockRemainderFromCurrent();
		bool	unlockRemainderFromCurrent(off_t start);

		// These methods allow you to lock "the remainder" of a file
		// relative to the end of the file.
		bool	tryLockRemainderFromEnd(short type);
		bool	tryLockRemainderFromEnd(short type, off_t start);
		bool	lockRemainderFromEnd(short type);
		bool	lockRemainderFromEnd(short type, off_t start);
		bool	checkLockRemainderFromEnd(short type,
							struct flock *lck);
		bool	checkLockRemainderFromEnd(short type, off_t start,
							struct flock *lck);
		bool	unlockRemainderFromEnd();
		bool	unlockRemainderFromEnd(off_t start);


		// These methods allow you to truncate a file
		static	bool	truncate(const char *filename);
		static	bool	truncate(const char *filename, off_t length);
		bool	truncate();
		bool	truncate(off_t length);


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

		off_t	getCurrentPosition();
			// Returns the position at which the next read or
			// write will occur or -1 on error.

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
			// the file into the string, terminates it with a NULL
			// and returns the string.
			// The string must be freed by the calling program.
			//
			// If the file contains no data, then a string of length
			// zero is returned.  If an error occurs then a NULL
			// is returned.

		ssize_t	getContents(unsigned char *buffer, size_t buffersize);
			// Reads the contents of the currently open file into
			// "buffer" of length "buffersize".  If the file is
			// larger than "buffersize" then only "buffersize" bytes
			// of the file will be read into "buffer".
			//
			// Returns the number of bytes copied into "buffer" or
			// -1 on error.

		static	ssize_t	getContents(const char *name,
						unsigned char *buffer,
						size_t buffersize);
			// Reads the contents of the file "name" into
			// "buffer" of length "buffersize".  If the file is
			// larger than "buffersize" then only "buffersize" bytes
			// of the file will be read into "buffer".
			//
			// Returns the number of bytes copied into "buffer" or
			// -1 on error.

		bool	getCurrentProperties();
			// Gets the current file properties.
			// 
			// Returns true on success and false on failure.

		static bool	exists(const char *filename);
			// Returns true if the file exists and false otherwise.

		// These methods return the permissions of the file.
		//
		// The static methods return true on success and false on
		// failure.
		mode_t		getPermissions() const;
		static bool	getPermissions(const char *filename,
						mode_t *perms);
		static bool	getPermissions(int fd, mode_t *perms);

		// These methods return the user id of the file's owner.
		//
		// The static methods return true on success and false on
		// failure.
		uid_t		getOwnerUserId() const;
		static bool	getOwnerUserId(const char *filename,
						uid_t *uid);
		static bool	getOwnerUserId(int fd, uid_t *uid);


		// These methods return the group id of the file's owner.
		//
		// The static methods return true on success and false on
		// failure.
		gid_t		getOwnerGroupId() const;
		static bool	getOwnerGroupId(const char *filename,
						gid_t *gid);
		static bool	getOwnerGroupId(int fd, gid_t *gid);


		// These methods return the number of bytes in the file.
		//
		// The static methods return true on success and false on
		// failure.
		off_t		getSize() const;
		static bool	getSize(const char *filename,
						off_t *size);
		static bool	getSize(int fd, off_t *size);


		// These methods return the blocksize to use
		// for most efficient I/O with the file.
		// Note that some systems don't support this.
		// On those systems, a block size of -1 is returned.
		//
		// The static methods return true on success and false on
		// failure.
		blksize_t	getBlockSize() const;
		static bool	getBlockSize(const char *filename,
						blksize_t *size);
		static bool	getBlockSize(int fd, blksize_t *size);


		// These methods return the number of
		// blocks allocated for the file.
		// Note that some systems don't support this.
		// On those systems, a block count of -1 is returned.
		//
		// The static methods return true on success and false on
		// failure.
		blkcnt_t	getBlockCount() const;
		static bool	getBlockCount(const char *filename,
						blkcnt_t *blocks);
		static bool	getBlockCount(int fd, blkcnt_t *blocks);


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
		//
		// The static methods return true on success and false on
		// failure.
		time_t		getLastAccessTime() const;
		static bool	getLastAccessTime(const char *filename,
							time_t *atime);
		static bool	getLastAccessTime(int fd, time_t *atime);


		// These methods return the time of last modification.
		//
		// Modification time IS NOT affected by chmod, chown,
		// or creating links.
		//
		// The static methods return true on success and false on
		// failure.
		time_t		getLastModificationTime() const;
		static bool	getLastModificationTime(const char *filename,
							time_t *mtime);
		static bool	getLastModificationTime(int fd, time_t *mtime);


		// These methods return the time of last change.
		//
		// Change time IS affected by chmod, chown and creating links.
		//
		// The static methods return true on success and false on
		// failure.
		time_t		getLastChangeTime() const;
		static bool	getLastChangeTime(const char *filename,
							time_t *ctime);
		static bool	getLastChangeTime(int fd, time_t *ctime);


		// These methods return the device of the file.
		//
		// The static methods return true on success and false on
		// failure.
		dev_t		getDevice() const;
		static bool	getDevice(const char *filename, dev_t *dev);
		static bool	getDevice(int fd, dev_t *dev);


		// These methods return the device type of the file.
		//
		// The static methods return true on success and false on
		// failure.
		dev_t		getDeviceType() const;
		static bool	getDeviceType(const char *filename,
							dev_t *devtype);
		static bool	getDeviceType(int fd, dev_t *devtype);


		// These methods return the inode of the file.
		//
		// The static methods return true on success and false on
		// failure.
		ino_t		getInode() const;
		static bool	getInode(const char *filename, ino_t *inode);
		static bool  	getInode(int fd, ino_t *inode);


		// These methods return the number of hard links to the file.
		//
		// The static methods return true on success and false on
		// failure.
		nlink_t		getNumberOfHardLinks() const;
		static bool	getNumberOfHardLinks(const char *filename,
							nlink_t *nlink);
		static bool	getNumberOfHardLinks(int fd, nlink_t *nlink);

		// This method returns a pointer to the stat structure
		// used internally.
		struct stat	*getStat();

		// By default, the open() and create() methods call 
		// getCurrentProperties() so the other methods of this class
		// will return valid data about the file immediately.  These
		// methods alter that behaviour.
		void	getCurrentPropertiesOnOpen();
		void	dontGetCurrentPropertiesOnOpen();

	#include <rudiments/private/file.h>
};

#ifdef ENABLE_RUDIMENTS_INLINES
	#include <rudiments/private/fileinlines.h>
#endif

#endif
