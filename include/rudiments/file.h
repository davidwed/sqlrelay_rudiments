// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILE_H
#define RUDIMENTS_FILE_H

#include <rudiments/private/fileincludes.h>

// The file class provides methods for interacting with files and for 
// discovering the properties associated with a file such as permissions,
// ownership, size, etc.
//
// Many of the static methods are just provided for convenience.  Use them
// if you need to get a a single property or perform a single operation.
//
// If you need to get multiple properties or perform multiple operations,
// create an instance of the class, call open(), then use the non-static
// methods.

class file : public filedescriptor {
	public:

			file();
			// Creates an uninitialized instance of the file class.
			file(int fd);
			// Creates an uninitialized instance of the file class
			// and associates already open file descriptor "fd"
			// with it.
		virtual	~file();


		// These methods open the file "name" using "flags".
		// They return true on success and false on failure.
		bool	open(const char *name, int flags);
		bool	open(const char *name, int flags, mode_t perms);
			// If flags contains O_CREAT and the file doesn't
			// already exist, it will be created with permissions
			// "perms".


		// These methods creates the file "name" with permissions
		// "perms".  If the file already exists, it will be truncated.
		bool	create(const char *name, mode_t perms);
			// Returns true on success and false on failure.
		ssize_t	create(const char *name, mode_t perms,
						unsigned short number);
			// Sets the initial contents to "number".
			// Returns the number of bytes written or -1 on error.
		ssize_t	create(const char *name, mode_t perms,
						unsigned long number);
			// Sets the initial contents to "number".
			// Returns the number of bytes written or -1 on error.
		ssize_t	create(const char *name, mode_t perms, float number);
			// Sets the initial contents to "number".
			// Returns the number of bytes written or -1 on error.
		ssize_t	create(const char *name, mode_t perms, double number);
			// Sets the initial contents to "number".
			// Returns the number of bytes written or -1 on error.
		ssize_t	create(const char *name, mode_t perms, char number);
			// Sets the initial contents to "number".
			// Returns the number of bytes written or -1 on error.
		ssize_t	create(const char *name, mode_t perms,
					const char *string);
			// Sets the initial contents to "string".
			// Returns the number of bytes written or -1 on error.
		ssize_t	create(const char *name, mode_t perms,
					const char *string, size_t size);
			// Sets the initial contents to "string" of size "size".
			// Returns the number of bytes written or -1 on error.
		ssize_t	create(const char *name, mode_t perms,
					const void *data, size_t size);
			// Sets the initial contents to "data" of size "size".
			// Returns the number of bytes written or -1 on error.


		// These methods read the contents of the file into a buffer.
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
		ssize_t	getContents(unsigned char *buffer, size_t buffersize);
			// Reads the contents of the currently open file into
			// "buffer" of length "buffersize".  If the file is
			// larger than "buffersize" then only "buffersize" bytes
			// of the file will be read into "buffer".
			//
			// Returns the number of bytes copied into "buffer" or
			// -1 on error.


		// These methods truncate the file.
		// They return true on success and false on failure.
		bool	truncate();
			// Truncates all data in the file, resulting in
			// a file of zero length.
		bool	truncate(off_t length);
			// Truncates all data beyond the first "length"
			// bytes, resulting in a file of "length" bytes.


		// These methods allow you to get or set the position at which
		// the next read or write will occur.  Each returns the position
		// relative to the beginning of the file on success or -1 on
		// failure.
		off_t	getCurrentPosition();
			// Returns the position at which the next read or
			// write will occur.
		off_t	setPositionRelativeToBeginning(off_t offset);
			// Sets the position to the beginning of the
			// file + "offset" bytes.
		off_t	setPositionRelativeToCurrent(off_t offset);
			// Sets the position to the current
			// position + "offset" bytes.
		off_t	setPositionRelativeToEnd(off_t offset);
			// Sets the position to the end of the
			// file + "offset" bytes.


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

		// These methods allow you to lock an arbitrary
		// region of the file.
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


		// These methods allow you to advise the kernel that you are
		// going to access a region of a file in a particular manner.
		// The kernel can then perform some optimisations.
		//
		// In these methods, the region of the file begins at "start"
		// and continues for "len" bytes.
		//
		// These methods return true on success and false on failure.
		//
		// On operating systems don't support these methods, they
		// return true but don't actually do anything.
		bool	sequentialAccess(off_t start, size_t len);
			// The region will be accessed in sequential order.
		bool	randomAccess(off_t start, size_t len);
			// The region will be accessed in random order.
		bool	onlyOnce(off_t start, size_t len);
			// The region will be only be accessed once.
		bool	willNeed(off_t start, size_t len);
			// The region will be accessed in the near future.
		bool	wontNeed(off_t start, size_t len);
			// The region will not be accessed in the near future.
		bool	normalAccess(off_t start, size_t len);
			// Removes any advice that has previously been applied
			// to the region.


		#ifdef HAVE_POSIX_FALLOCATE
		bool	reserve(off_t start, size_t len);
			// Reserves space on the storage medium such that a
			// write to the region starting at "start" and
			// continuing for "len" bytes will not fail due to
			// lack of storage space.
			//
			// Returns true if the region can be reserved and
			// false otherwise.
		#endif


		// Changes to files are often cached in system ram, these
		// methods makes sure that those changes are copied
		// to the storage medium that the file resides on.
		//
		// Returns true on success and false on failure.
		//
		// Note that if a hard disk has on-board write cache
		// enabled, this method only assures that the changes
		// has been copied into the disk's write cache, not
		// necessarily to the disk itself.
		bool	sync();
		#ifdef HAVE_FDATASYNC
		bool	dataSync();
			// Similar to sync() but does not wait for the file's
			// last-access or last-modification times to be copied.
		#endif


		// By default, the open() and create() methods call 
		// getCurrentProperties() so the other methods of this class
		// will return valid data about the file immediately.  These
		// methods alter that behaviour.
		void	getCurrentPropertiesOnOpen();
		void	dontGetCurrentPropertiesOnOpen();

		bool	getCurrentProperties();
			// Get the current file properties.
			// 
			// Returns true on success and false on failure.

		// These methods return various file properties.
		mode_t		getPermissions() const;
				// returns the permissions
		uid_t		getOwnerUserId() const;
				// returns the user id of the file's owner
		gid_t		getOwnerGroupId() const;
				// returns the group id of the file's owner
		off_t		getSize() const;
				// returns the number of bytes in the file
		blksize_t	getBlockSize() const;
				// returns the blocksize to use for most
				// efficient I/O with the file or -1 for
				// systems don't support this
		blkcnt_t	getBlockCount() const;
				// returns the number of blocks allocated for
				// the file or -1 for systems don't support this
		int		isSocket() const;
				// returns 1 if the file is a socket,
				// 0 if it's not or -1 on error.
		int		isSymbolicLink() const;
				// returns 1 if the file is a symbolic link,
				// 0 if it's not or -1 on error.
		int		isRegularFile() const;
				// returns 1 if the file is a regular file,
				// 0 if it's not or -1 on error.
		int		isBlockDevice() const;
				// returns 1 if the file is a block device,
				// 0 if it's not or -1 on error.
		int		isDirectory() const;
				// returns 1 if the file is a directory,
				// 0 if it's not or -1 on error.
		int		isCharacterDevice() const;
				// returns 1 if the file is a character device,
				// 0 if it's not or -1 on error.
		int		isFifo() const;
				// returns 1 if the file is a fifo,
				// 0 if it's not or -1 on error.
		time_t		getLastAccessTime() const;
				// returns the time of last access
		time_t		getLastModificationTime() const;
				// returns the time of last modification
				//
				// Modification time IS NOT affected by chmod,
				// chown, or creating links.
		time_t		getLastChangeTime() const;
				// returns the time of last change
				//
				// Change time IS affected by chmod, chown and
				// creating links.
		dev_t		getDevice() const;
				// return the device of the file
		dev_t		getDeviceType() const;
				// return the device type of the file
		ino_t		getInode() const;
				// return the inode of the file
		nlink_t		getNumberOfHardLinks() const;
				// return the number of hard links to the file
		struct stat	*getStat();
				// returns a pointer to the stat structure
				// used internally

		// These methods change the user and/or group
		// that owns the file.
		//
		// They return true on success and false on failure.
		bool	changeOwner(const char *newuser, const char *newgroup);
		bool	changeOwner(uid_t uid, gid_t gid);
		bool	changeOwnerUserId(const char *newuser);
		bool	changeOwnerUserId(uid_t uid);
		bool	changeOwnerGroupId(const char *newgroup);
		bool	changeOwnerGroupId(gid_t gid);

		bool	canChangeOwner();
			// Returns true if any of the changeOwner()
			// methods are allowed on "filename" and
			// false otherwise.

		long	maxLinks();
			// Returns the maximum number of links that can be
			// created to "filename".


		#ifdef HAVE_XATTRS

		// Some filesystems support extended file attributes.  These
		// methods provide an interface for getting and setting those
		// attributes.

		char	**listAttributes();
			// Returns a NULL terminated array of attributes or
			// NULL if an error occurred.


		// These methods reads the value of the named attribute into
		// the supplied (preallocated) buffer.
		// They return true on success and false on failure.
		bool	getAttribute(const char *name,
						unsigned short *number);
		bool	getAttribute(const char *name,
						unsigned long *number);
		bool	getAttribute(const char *name, short *number);
		bool	getAttribute(const char *name, long *number);
		bool	getAttribute(const char *name, float *number);
		bool	getAttribute(const char *name, double *number);
		bool	getAttribute(const char *name,
						unsigned char *character);
		bool	getAttribute(const char *name, char *character);
		bool	getAttribute(const char *name, bool *value);

		// These methods allocate a buffer, read the value into it
		// and return the allocated buffer.  Some of these methods also
		// return the size of the buffer.
		// They return true on success and false on failure.
		bool	getAttribute(const char *name, unsigned char **string);
		bool	getAttribute(const char *name, char **string);
		bool	getAttribute(const char *name,
					unsigned char **string, size_t *size);
		bool	getAttribute(const char *name,
					char **string, size_t *size);
		bool	getAttribute(const char *name,
					void **buffer, size_t *size);


		// These methods create a new attribute.
		// They return true on success and false on failure and will
		// fail if the attribute already exists.
		bool	createAttribute(const char *name,
						unsigned short number);
		bool	createAttribute(const char *name,
						unsigned long number);
		bool	createAttribute(const char *name, short number);
		bool	createAttribute(const char *name, long number);
		bool	createAttribute(const char *name, float number);
		bool	createAttribute(const char *name, double number);
		bool	createAttribute(const char *name,
						unsigned char character);
		bool	createAttribute(const char *name, char character);
		bool	createAttribute(const char *name, bool value);
		bool	createAttribute(const char *name,
						const unsigned char *string);
		bool	createAttribute(const char *name, const char *string);
		bool	createAttribute(const char *name,
						const unsigned char *string,
						size_t size);
		bool	createAttribute(const char *name,
					const char *string, size_t size);
		bool	createAttribute(const char *name,
					const void *buffer, size_t size);


		// These method replace an existing attribute.
		// They return true on success and false on failure and will
		// fail if the attribute doesn't already exist.
		bool	replaceAttribute(const char *name,
						unsigned short number);
		bool	replaceAttribute(const char *name,
						unsigned long number);
		bool	replaceAttribute(const char *name, short number);
		bool	replaceAttribute(const char *name, long number);
		bool	replaceAttribute(const char *name, float number);
		bool	replaceAttribute(const char *name, double number);
		bool	replaceAttribute(const char *name,
						unsigned char character);
		bool	replaceAttribute(const char *name, char character);
		bool	replaceAttribute(const char *name, bool value);
		bool	replaceAttribute(const char *name,
						const unsigned char *string);
		bool	replaceAttribute(const char *name, const char *string);
		bool	replaceAttribute(const char *name,
						const unsigned char *string,
						size_t size);
		bool	replaceAttribute(const char *name,
						const char *string,
						size_t size);
		bool	replaceAttribute(const char *name,
						const void *buffer,
						size_t size);


		// These methods create new attributes or replace existing
		// attributes.  They returns true on success and false on
		// failure.
		bool	setAttribute(const char *name,
						unsigned short number);
		bool	setAttribute(const char *name,
						unsigned long number);
		bool	setAttribute(const char *name, short number);
		bool	setAttribute(const char *name, long number);
		bool	setAttribute(const char *name, float number);
		bool	setAttribute(const char *name, double number);
		bool	setAttribute(const char *name,
						unsigned char character);
		bool	setAttribute(const char *name, char character);
		bool	setAttribute(const char *name, bool value);
		bool	setAttribute(const char *name,
						const unsigned char *string);
		bool	setAttribute(const char *name, const char *string);
		bool	setAttribute(const char *name,
						const unsigned char *string,
						size_t size);
		bool	setAttribute(const char *name,
						const char *string,
						size_t size);
		bool	setAttribute(const char *name,
						const void *buffer,
						size_t size);


		bool	removeAttribute(const char *name);
			// Removes attribute "name".
			// Returns true on success and false on failure.
		#endif



		// These methods allow you to create special kinds of files.
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
		static bool	createHardLink(const char *oldpath,
							const char *newpath);
			// Creates a hard link between "oldpath" and "newpath".
			// Returns true on success and false on failure.
		static bool	createSymbolicLink(const char *oldpath,
							const char *newpath);
			// Creates a symbolic link between "oldpath" and
			// "newpath". Returns true on success and false on
			// failure.
		static char	*resolveSymbolicLink(const char *filename);
			// Returns the pathname of the file that the symbolic
			// link "filename" points to.  Returns NULL on failure.
			//
			// Note that this method allocates a buffer for the
			// path internally and returns it.  The calling program
			// must deallocate this buffer.


		static bool	rename(const char *oldpath,
					const char *newpath);
			// Renames "oldpath" to "newpath".  Returns true on
			// success and false on failure.
		static bool	remove(const char *filename);
			// Removes file "filename".  Returns true on success
			// and false on failure.  (Will not remove a directory).


		// These methods truncate the file.
		// They return true on success and false on failure.
		static	bool	truncate(const char *filename);
		static	bool	truncate(const char *filename, off_t length);


		// These methods read the contents of the file into a buffer.
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


		// These methods allow you to check various access statuses.
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


		// These methods return various file properties into the
		// supplied buffers.
		// They return true on success and false on failure.
		static bool	getPermissions(const char *filename,
						mode_t *perms);
		static bool	getOwnerUserId(const char *filename,
						uid_t *uid);
		static bool	getOwnerGroupId(const char *filename,
						gid_t *gid);
		static bool	getSize(const char *filename,
						off_t *size);
		static bool	getBlockSize(const char *filename,
						blksize_t *size);
		static bool	getBlockCount(const char *filename,
						blkcnt_t *blocks);
		static int	isSocket(const char *filename);
		static int	isSymbolicLink(const char *filename);
		static int	isRegularFile(const char *filename);
		static int	isBlockDevice(const char *filename);
		static int	isDirectory(const char *filename);
		static int	isCharacterDevice(const char *filename);
		static int	isFifo(const char *filename);
		static bool	getLastAccessTime(const char *filename,
							time_t *atime);
		static bool	getLastModificationTime(const char *filename,
							time_t *mtime);
		static bool	getLastChangeTime(const char *filename,
							time_t *ctime);
		static bool	getDevice(const char *filename, dev_t *dev);
		static bool	getDeviceType(const char *filename,
							dev_t *devtype);
		static bool	getInode(const char *filename, ino_t *inode);
		static bool	getNumberOfHardLinks(const char *filename,
							nlink_t *nlink);
		static bool	changeOwner(const char *filename,
						const char *newuser,
						const char *newgroup);
		static bool	changeOwner(const char *filename,
						uid_t uid, gid_t gid);
		static bool	changeOwnerUserId(const char *filename,
							const char *newuser);
		static bool	changeOwnerUserId(const char *filename,
							uid_t uid);
		static bool	changeOwnerGroupId(const char *filename,
							const char *newgroup);
		static bool	changeOwnerGroupId(const char *filename,
							gid_t gid);

		static bool	canChangeOwner(const char *filename);
				// Returns true if any of the changeOwner()
				// methods are allowed on "filename" and
				// false otherwise.


		// These methods allow you to override the last access and/or
		// modification time of a file.
		// They return true on success and false on failure.
		static bool	setLastAccessTime(const char *filename,
							time_t lastaccesstime);
		static bool	setLastModificationTime(const char *filename,
							time_t lastmodtime);
		static bool	setLastAccessAndModificationTimes(
							const char *filename,
							time_t lastaccesstime,
							time_t lastmodtime);
		static bool	setLastAccessAndModificationTimes(
							const char *filename);
			// Sets the last access and modification times of
			// "filename" to now.


		static char	*dirname(const char *filename);
				// Returns the directory portion of "filename".
				// This method allocates a buffer internally
				// and returns it.  The calling program must
				// deallocate the buffer.
		static char	*basename(const char *filename);
				// Returns the non-directory portion of
				// "filename".
				// This method allocates a buffer internally
				// and returns it.  The calling program must
				// deallocate the buffer.
		static char	*basename(const char *filename,
						const char *suffix);
				// Returns the non-directory portion of
				// "filename", truncating "suffix".
				// This method allocates a buffer internally
				// and returns it.  The calling program must
				// deallocate the buffer.


		static key_t	generateKey(const char *filename, int id);
				// Generates a key based on "filename" and the
				// least signifigant 8 bits of id (which must
				// be non-zero) suitable for use with
				// semaphores, shared memory segments and
				// message queues.

		static long	maxLinks(const char *filename);
				// Returns the maximum number of links that
				// can be created to "filename".

	#include <rudiments/private/file.h>
};

#endif
