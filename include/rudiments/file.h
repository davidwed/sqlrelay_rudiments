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


		// The following methods allow you to lock files or regions
		// of files.
		//
		// The tryLock() methods attempt to lock and return success if
		// the lock can be established immediately or false otherwise.
		//
		// The lock() methods block until a lock is established and
		// return true on success or false if an error occurs.
		//
		// The checkLock() methods check to see if a lock can be
		// established but do not actually lock anything.  If a lock
		// can be established, they return true. Otherwise they return
		// false and populate the fields in the supplied flock struct
		// with information about one of the conflicting locks.
		//
		// The unlock() methods attempt to release a previosly
		// established lock and return true on success or false on
		// error.

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
		bool		truncate();
				// Truncates all data in the file, resulting in
				// a file of zero length.  Returns true on
				// success and false on failure.

		static	bool	truncate(const char *filename, off_t length);
		bool		truncate(off_t length);
				// Truncates all data beyond the first "length"
				// bytes, resulting in a file of "length" bytes.
				// Returns true on success and false on failure.


		bool	sync();
			// Changes to files are often cached in system ram,
			// this method makes sure that those changes are copied
			// to the storage medium that the file resides on.
			//
			// Returns true on success and false on failure.
			//
			// Note that if a hard disk has on-board write cache
			// enabled, this method only assures that the changes
			// has been copied into the disk's write cache, not
			// necessarily to the disk itself.
		bool	dataSync();
			// Similar to sync() but does not wait for the file's
			// last-access or last-modification times to be copied.

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
		static bool	readable(const char *filename);
			// Returns true if "filename" is readable by the user
			// or false otherwise.
		static bool	writeable(const char *filename);
			// Returns true if "filename" is writeable by the user
			// or false otherwise.
		static bool	executable(const char *filename);
			// Returns true if "filename" is executable by the user
			// or false otherwise.
		static bool	accessible(const char *filename, int mode);
			// Checks to see if "filename" exists, is readable,
			// is writeable and/or is executable by the user, based
			// on the value of "mode".  Mode should be an or'ed
			// combination of F_OK (exists), R_OK (readable),
			// W_OK (writable) and X_OK (executable) flags.
			//
			// Returns true if the file meets the conditions set
			// by the mode and false otherwise.

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


		// These methods change the user and group that owns the file.
		//
		// They return true on success and false on failure.
		bool	changeOwner(const char *newuser, const char *newgroup);
		bool	changeOwner(uid_t uid, gid_t gid);
		static bool	changeOwner(const char *filename,
						const char *newuser,
						const char *newgroup);
		static bool	changeOwner(const char *filename,
						uid_t uid, gid_t gid);

		// These methods change the user that owns the file.
		//
		// They return true on success and false on failure.
		bool	changeOwnerUserId(const char *newuser);
		bool	changeOwnerUserId(uid_t uid);
		static bool	changeOwnerUserId(const char *filename,
							const char *newuser);
		static bool	changeOwnerUserId(const char *filename,
							uid_t uid);

		// These methods change the group that owns the file.
		//
		// They return true on success and false on failure.
		bool	changeOwnerGroupId(const char *newgroup);
		bool	changeOwnerGroupId(gid_t gid);
		static bool	changeOwnerGroupId(const char *filename,
							const char *newgroup);
		static bool	changeOwnerGroupId(const char *filename,
							gid_t gid);


		static bool	rename(const char *oldpath,
					const char *newpath);
			// Renames "oldpath" to "newpath".  Returns true on
			// success and false on failure.
		static bool	remove(const char *filename);
			// Removes file "filename".  Returns true on success
			// and false on failure.  (Will not remove a directory).


		static bool	symlink(const char *oldpath,
					const char *newpath);
			// Creates a symbolic link between "oldpath" and
			// "newpath". Returns true on success and false on
			// failure.
		static bool	link(const char *oldpath,
					const char *newpath);
			// Creates a hard link between "oldpath" and "newpath".
			// Returns true on success and false on failure.
		static char	*readlink(const char *filename);
			// Returns the pathname of the file that the symbolic
			// link "filename" points to.  Returns NULL on failure.
			//
			// Note that this method allocates a buffer for the
			// path internally and returns it.  The calling program
			// must deallocate this buffer.

		static bool	setLastAccessTime(const char *filename,
							time_t lastaccesstime);
			// Sets the last access time of "filename" to
			// "lastaccesstime".
			//
			// Returns true on success and false on failure.
		static bool	setLastModificationTime(const char *filename,
							time_t lastmodtime);
			// Sets the last modification time of "filename" to
			// "lastmodtime".
			//
			// Returns true on success and false on failure.
		static bool	setLastAccessAndModificationTimes(
							const char *filename,
							time_t lastaccesstime,
							time_t lastmodtime);
			// Sets the last access time of "filename" to
			// "lastaccesstime" and the last modification time
			// to "lastmodtime".
			//
			// Returns true on success and false on failure.
		static bool	setLastAccessAndModificationTimes(
							const char *filename);
			// Sets the last access and modification times of
			// "filename" to now.
			//
			// Returns true on success and false on failure.

		static bool	createDeviceNode(const char *filename,
						bool blockdevice,
						unsigned short major,
						unsigned short minor,
						mode_t perms);
			// Creates device node "filename" with major number
			// "major" and minor number "minor".  The device node
			// will be created as a block device if "blockdevice"
			// is true, otherwise it will be created as a character
			// device.  The device node will be assigned
			// permissions "perms".
			//
			// Returns true on success and false on failure.

		static bool	createFifo(const char *filename, mode_t perms);
			// Creates a fifo called "filename" with permissions
			// "perms".
			//
			// Returns true on success and false on failure.

		static int	createTemporaryFile(char *templatefilename);
			// Creates a temporary file using "templatefilename"
			// as a template.  The last 6 characters of
			// "templatefilename" must be XXXXXX and
			// "templatefilename" will be modified to contain the
			// name of the file that was actually created.
			//
			// Opens and returns the file descriptor handle of the
			// file on success and -1 on failure.


		#ifdef HAVE_XATTRS
		char	**listAttributes();
			// Returns a NULL terminated array of attributes of the
			// file or NULL if an error occurred.


		// Get attribute methods - gets the value of the named
		// attribute.
		// Returns true on success and false on failure.
		bool	getAttribute(const char *name,
						unsigned short *number);
			// reads the value of "name" into buffer "number"
		bool	getAttribute(const char *name,
						unsigned long *number);
			// reads the value of "name" into buffer "number"
		bool	getAttribute(const char *name, short *number);
			// reads the value of "name" into buffer "number"
		bool	getAttribute(const char *name, long *number);
			// reads the value of "name" into buffer "number"
		bool	getAttribute(const char *name, float *number);
			// reads the value of "name" into buffer "number"
		bool	getAttribute(const char *name, double *number);
			// reads the value of "name" into buffer "number"
		bool	getAttribute(const char *name,
						unsigned char *character);
			// reads the value of "name" into buffer "character"
		bool	getAttribute(const char *name, char *character);
			// reads the value of "name" into buffer "character"
		bool	getAttribute(const char *name, bool *value);
			// reads the value of "name" into buffer "value"
		bool	getAttribute(const char *name, unsigned char **string);
			// allocates "string" and reads the value of
			// "name" into it
		bool	getAttribute(const char *name, char **string);
			// allocates "string" and reads the value of
			// "name" into it
		bool	getAttribute(const char *name,
					unsigned char **string, size_t *size);
			// allocates "string", reads the value of "name" into it
		bool	getAttribute(const char *name,
					char **string, size_t *size);
			// allocates "string", reads the value of "name" into it
		bool	getAttribute(const char *name,
					void **buffer, size_t *size);
			// allocates "buffer", reads the value of "name" into it


		// Create attribute methods - creates a new attribute.
		// Returns true on success and false on failure.  Will
		// fail if the attribute already exists.
		bool	createAttribute(const char *name,
						unsigned short number);
			// creates attribute "name" with value "number"
		bool	createAttribute(const char *name,
						unsigned long number);
			// creates attribute "name" with value "number"
		bool	createAttribute(const char *name, short number);
			// creates attribute "name" with value "number"
		bool	createAttribute(const char *name, long number);
			// creates attribute "name" with value "number"
		bool	createAttribute(const char *name, float number);
			// creates attribute "name" with value "number"
		bool	createAttribute(const char *name, double number);
			// creates attribute "name" with value "number"
		bool	createAttribute(const char *name,
						unsigned char character);
			// creates attribute "name" with value "character"
		bool	createAttribute(const char *name, char character);
			// creates attribute "name" with value "character"
		bool	createAttribute(const char *name, bool value);
			// creates attribute "name" with value "value"
		bool	createAttribute(const char *name,
						const unsigned char *string);
			// creates attribute "name" with value "string"
		bool	createAttribute(const char *name, const char *string);
			// creates attribute "name" with value "string"
		bool	createAttribute(const char *name,
						const unsigned char *string,
						size_t size);
			// creates attribute "name" with value "string"
		bool	createAttribute(const char *name,
					const char *string, size_t size);
			// creates attribute "name" with value "string"
		bool	createAttribute(const char *name,
					const void *buffer, size_t size);
			// creates attribute "name" with value "buffer"


		// Replace attribute methods - replaces and existing attribute.
		// Returns true on success and false on failure.  Will
		// fail if the attribute doesn't already exist.
		bool	replaceAttribute(const char *name,
						unsigned short number);
			// replaces attribute "name" with value "number"
		bool	replaceAttribute(const char *name,
						unsigned long number);
			// replaces attribute "name" with value "number"
		bool	replaceAttribute(const char *name, short number);
			// replaces attribute "name" with value "number"
		bool	replaceAttribute(const char *name, long number);
			// replaces attribute "name" with value "number"
		bool	replaceAttribute(const char *name, float number);
			// replaces attribute "name" with value "number"
		bool	replaceAttribute(const char *name, double number);
			// replaces attribute "name" with value "number"
		bool	replaceAttribute(const char *name,
						unsigned char character);
			// replaces attribute "name" with value "character"
		bool	replaceAttribute(const char *name, char character);
			// replaces attribute "name" with value "character"
		bool	replaceAttribute(const char *name, bool value);
			// replaces attribute "name" with value "value"
		bool	replaceAttribute(const char *name,
						const unsigned char *string);
			// replaces attribute "name" with value "string"
		bool	replaceAttribute(const char *name, const char *string);
			// replaces attribute "name" with value "string"
		bool	replaceAttribute(const char *name,
						const unsigned char *string,
						size_t size);
			// replaces attribute "name" with value "string"
		bool	replaceAttribute(const char *name,
						const char *string,
						size_t size);
			// replaces attribute "name" with value "string"
		bool	replaceAttribute(const char *name,
						const void *buffer,
						size_t size);
			// replaces attribute "name" with value "buffer"


		// Set attribute methods - creates new attribute or replaces
		// existing attribute.
		// Returns true on success and false on failure.  Will
		// fail if the attribute doesn't already exist.
		bool	setAttribute(const char *name,
						unsigned short number);
			// sets attribute "name" to value "number"
		bool	setAttribute(const char *name,
						unsigned long number);
			// sets attribute "name" to value "number"
		bool	setAttribute(const char *name, short number);
			// sets attribute "name" to value "number"
		bool	setAttribute(const char *name, long number);
			// sets attribute "name" to value "number"
		bool	setAttribute(const char *name, float number);
			// sets attribute "name" to value "number"
		bool	setAttribute(const char *name, double number);
			// sets attribute "name" to value "number"
		bool	setAttribute(const char *name,
						unsigned char character);
			// sets attribute "name" to value "character"
		bool	setAttribute(const char *name, char character);
			// sets attribute "name" to value "character"
		bool	setAttribute(const char *name, bool value);
			// sets attribute "name" to value "value"
		bool	setAttribute(const char *name,
						const unsigned char *string);
			// sets attribute "name" to value "string"
		bool	setAttribute(const char *name, const char *string);
			// sets attribute "name" to value "string"
		bool	setAttribute(const char *name,
						const unsigned char *string,
						size_t size);
			// sets attribute "name" to value "string"
		bool	setAttribute(const char *name,
						const char *string,
						size_t size);
			// sets attribute "name" to value "string"
		bool	setAttribute(const char *name,
						const void *buffer,
						size_t size);
			// sets attribute "name" to value "buffer"


		bool	removeAttribute(const char *name);
			// Removes attribute "name".
			// Returns true on success and false on failure.
		#endif

	#include <rudiments/private/file.h>
};

#endif
