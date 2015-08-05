// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILE_H
#define RUDIMENTS_FILE_H

#include <rudiments/private/fileincludes.h>

class fileprivate;

/** The file class provides methods for interacting with files and for 
 *  discovering the properties associated with a file such as permissions,
 *  ownership, size, etc.
 * 
 *  Many of the static methods are just provided for convenience.  Use them
 *  if you need to get a a single property or perform a single operation.
 * 
 *  If you need to get multiple properties or perform multiple operations,
 *  create an instance of the class, call open(), then use the non-static
 *  methods. */
class RUDIMENTS_DLLSPEC file : public filedescriptor {
	public:

		/** Creates an instance of the file class. */
		file();

		/** Creates an instance of the file class
		 *  that is a copy of "f". */
		file(const file &f);

		/** Makes this instance of the file class
		 *  identical to "f". */
		file	&operator=(const file &f);

		/** Deletes this instance of the file class. */
		virtual	~file();

		/** Opens the file "name" using "flags".
		 *
		 *  "flags" may contain one more more of the following flags,
		 *  or'ed together:
		 *
		 *  O_RDONLY - Open the file in read-only mode.
		 *
		 *  O_WRONLY - Open the file in write-only mode.
		 *
		 *  O_RDWR - Open the file in write-only mode.
		 *
		 *  O_APPEND - Set the position to the end of the file.
		 *
		 *  O_TRUNC - Truncate the file.  Requires O_WRONLY or O_RDWR.
		 *
		 *  O_CREAT - Creates the file if it doesn't exist.  See O_EXCL.
		 *
		 *  O_EXCL - Requires O_CREAT.  Causes O_CREAT to fail if the
		 *  		file already exists.  Without O_EXCL, O_CREAT
		 *  		will succeed, and just open the file, if the
		 *  		file already exists.
		 *
		 *  Many platforms support additional, platform-specific flags.
		 *
		 *  Returns true on success and false on failure. */
		bool	open(const char *name, int32_t flags);

		/** Opens the file "name" using "flags".
		 *
		 *  "flags" may contain one more more of the following flags,
		 *  or'ed together:
		 *
		 *  O_RDONLY - Open the file in read-only mode.
		 *
		 *  O_WRONLY - Open the file in write-only mode.
		 *
		 *  O_RDWR - Open the file in write-only mode.
		 *
		 *  O_APPEND - Set the position to the end of the file.
		 *
		 *  O_TRUNC - Truncate the file.  Requires O_WRONLY or O_RDWR.
		 *
		 *  O_CREAT - Creates the file if it doesn't exist.  See O_EXCL.
		 *
		 *  O_EXCL - Requires O_CREAT.  Causes O_CREAT to fail if the
		 *  		file already exists.  Without O_EXCL, O_CREAT
		 *  		will succeed, and just open the file, if the
		 *  		file already exists.
		 *
		 *  Many platforms support additional, platform-specific flags.
		 *
		 *  If "flags" contains O_CREAT and the file doesn't already
		 *  exist, then it will be created with permissions "perms".
		 *
		 *  Returns true on success and false on failure. */
		bool	open(const char *name, int32_t flags, mode_t perms);

		/** Creates the file "name" with permissions "perms".  If the
		 *  file already exists, it will be truncated.  Returns true on
		 *  success and false on failure.
		 *
		 *  Equivalent to calling:
		 *  open(name,O_CREAT|O_WRONLY|O_TRUNC,perms) */
		bool	create(const char *name, mode_t perms);


		/** Allocates a string large enough to accommodate the
		 *  contents of the currently opened file, reads the
		 *  contents of the file into the string and returns the
		 *  string.  The string must be freed by the calling
		 *  program.
		 * 
		 *  If the file contains no data, then a string of length
		 *  zero is returned.  If an error occurs then a NULL
		 *  is returned. */
		char	*getContents();

		/** Reads the contents of the currently open file into
		 *  "buffer" of length "buffersize".  If the file is
		 *  larger than "buffersize" then only "buffersize" bytes
		 *  of the file will be read into "buffer".
		 * 
		 *  Returns the number of bytes copied into "buffer" or
		 *  -1 on error. */
		ssize_t	getContents(unsigned char *buffer, size_t buffersize);


		/** Truncates all data in the file, resulting in a file of
		 * zero length.  Returns true on success and false on
		 * failure. */
		bool	truncate() const;

		/** Truncates all data beyond the first "length" bytes,
		 *  resulting in a file of "length" bytes.  Returns true on
		 *  success and false on failure. */
		bool	truncate(off64_t length) const;


		/** Returns the position (relative to the beginning of the file)
		 *  at which the next read or write will occur, or -1 on
		 *  failure. */
		off64_t	getCurrentPosition() const;

		/** Sets the position (relative to the beginning of the file)
		 *  at which the next read or write will occur to "offset".
		 *  Returns that position on success or -1 on failure. */
		off64_t	setPositionRelativeToBeginning(off64_t offset) const;

		/** Advances the position at which the next read or write will
 		 *  occur by "offset" bytes.  Returns that position on success
 		 *  or -1 on failure. */
		off64_t	setPositionRelativeToCurrent(off64_t offset) const;

		/** Sets the position at which the next read or write will
		 *  occur to the end of the file plus "offset" bytes.
		 *  Generally, "offset" will be negative though most filesystems
		 *  allow the creation of files with holes in them and that can
		 *  be accomplished by using a positive "offset" and then
		 *  writing data at that position.  Returns the position on
		 *  succes or -1 on failure. */
		off64_t	setPositionRelativeToEnd(off64_t offset) const;

		/** Attempts to lock the entire file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK).  Returns true if the lock can
		 *  be established immediately or false otherwise. */
		bool	tryLockFile(int16_t type) const;

		/** Attempts to lock the entire file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK) and blocks until the lock
		 *  can be established.  Return true on success or false
		 *  otherwise. */
		bool	lockFile(int16_t type) const;

		/** Checks to see if the entire file can be locked or not
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) but
		 *  does not actually attempt to lock the file.  Returns true
		 *  if the lock can be established immediately or false
		 *  otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockFile(int16_t type,
					int16_t *conftype,
					int16_t *confwhence,
					off64_t *confstart,
					off64_t *conflen) const;

		/** Attempts to release a previosly established lock and returns
		 *  true on success or false otherwise. */
		bool	unlockFile() const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK).
		 *  Returns true if the lock can be established immediately or
		 *  false otherwise. */
		bool	tryLockRegion(int16_t type, off64_t start,
							off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) and
		 *  blocks until the lock can be established.  Returns true on
		 *  success or false otherwise. */
		bool	lockRegion(int16_t type, off64_t start,
							off64_t len) const;

		/** Checks to see if "len" bytes of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  can be locked or not using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) but does not actually attempt to lock
		 *  the file.  Returns true if the lock can be established
		 *  immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockRegion(int16_t type,
					off64_t start,
					off64_t len,
					int16_t *conftype,
					int16_t *confwhence,
					off64_t *confstart,
					off64_t *conflen) const;

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at position "start" (relative to
		 *  the beginning of the file).  Returns true on success or
		 *  false otherwise. */
		bool	unlockRegion(off64_t start, off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK).  Returns true if the lock can
		 *  be established immediately or false otherwise. */
		bool	tryLockFromCurrent(int16_t type, off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file plus "start" bytes
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK).
		 *  Returns true if the lock can be established immediately or
		 *  false otherwise. */
		bool	tryLockFromCurrent(int16_t type, off64_t start,
							off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK) and blocks until the lock can
		 *  be established.  Returns true on success or false
		 *  otherwise. */
		bool	lockFromCurrent(int16_t type, off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file plus "start" bytes
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) and
		 *  blocks until the lock can be established.  Returns true on
		 *  success or false otherwise. */
		bool	lockFromCurrent(int16_t type, off64_t start,
							off64_t len) const;

		/** Checks to see if "len" bytes of the file starting at
		 *  the current position in the file can be locked or not using
		 *  the specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockFromCurrent(int16_t type, off64_t len,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Checks to see if "len" bytes of the file starting at
		 *  the current position in the file plus "start" bytes
		 *  can be locked or not using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) but does not actually attempt to lock
		 *  the file.  Returns true if the lock can be established
		 *  immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockFromCurrent(int16_t type, off64_t start,
						off64_t len,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the current position in the
		 *  file.  Returns true on success or false otherwise. */
		bool	unlockFromCurrent(off64_t len) const;

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the current position in the
		 *  file plus "start" bytes.  Returns true on success or
		 *  false otherwise. */
		bool	unlockFromCurrent(off64_t start, off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the end of the file using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		bool	tryLockFromEnd(int16_t type, off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the the end of the file plus "start" bytes using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK).  Returns true
		 *  if the lock can be established immediately or false
		 *  otherwise. */
		bool	tryLockFromEnd(int16_t type, off64_t start,
						off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the end of the file using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can
		 *  be established.  Returns true on success or false
		 *  otherwise. */
		bool	lockFromEnd(int16_t type, off64_t len) const;

		/** Attempts to lock "len" bytes of the file starting at
		 *  the end of the file plus "start" bytes using the specified
		 *  lock "type" (F_RDLCK or F_WRLCK) and blocks until the lock
		 *  can be established.  Returns true on success or false
		 *  otherwise. */
		bool	lockFromEnd(int16_t type, off64_t start,
						off64_t len) const;

		/** Checks to see if "len" bytes of the file starting at
		 *  the end of the file can be locked or not using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockFromEnd(int16_t type, off64_t len,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Checks to see if "len" bytes of the file starting at
		 *  the end of the file plus "start" bytes can be locked or not
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) but
		 *  does not actually attempt to lock the file.  Returns true
		 *  if the lock can be established immediately or false
		 *  otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockFromEnd(int16_t type, off64_t start,
						off64_t len,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the end of the file.
		 *  Returns true on success or false otherwise. */
		bool	unlockFromEnd(off64_t len) const;

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the end of the file plus
		 *  "start" bytes.  Returns true on success or false
		 *  otherwise. */
		bool	unlockFromEnd(off64_t start, off64_t len) const;

		/** Attempts to lock the remainder of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK).
		 *  Returns true if the lock can be established immediately or
		 *  false otherwise. */
		bool	tryLockRemainder(int16_t type, off64_t start) const;

		/** Attempts to lock the remainder of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) and
		 *  blocks until the lock can be established.  Returns true on
		 *  success or false otherwise. */
		bool	lockRemainder(int16_t type, off64_t start) const;

		/** Checks to see if the remainder of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  can be locked or not using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) but does not actually attempt to lock
		 *  the file.  Returns true if the lock can be established
		 *  immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockRemainder(int16_t type, off64_t start,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at position "start"
		 *  (relative to the begining of the file).  Returns true on
		 *  success or false otherwise. */
		bool	unlockRemainder(off64_t start) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the current position using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		bool	tryLockRemainderFromCurrent(int16_t type) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the current position plus "start" bytes using the specified
		 *  lock "type" (F_RDLCK or F_WRLCK).  Returns true if the lock
		 *  can be established immediately or false otherwise. */
		bool	tryLockRemainderFromCurrent(int16_t type,
							off64_t start) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the current position using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can be
		 *  established.  Returns true on success or false otherwise. */
		bool	lockRemainderFromCurrent(int16_t type) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the current position plus "start" bytes using the specified
		 *  lock "type" (F_RDLCK or F_WRLCK) and blocks until the lock
		 *  can be established.  Returns true on success or false
		 *  otherwise. */
		bool	lockRemainderFromCurrent(int16_t type,
							off64_t start) const;

		/** Checks to see if the remainder of the file starting at
		 *  the current position can be locked or not using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockRemainderFromCurrent(int16_t type,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Checks to see if the remainder of the file starting at
		 *  the current position plus "start" bytes can be locked or not
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) but
		 *  does not actually attempt to lock the file.  Returns true
		 *  if the lock can be established immediately or false 
		 *  otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockRemainderFromCurrent(int16_t type,
						off64_t start,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the current position.
		 *  Returns true on success or false otherwise. */
		bool	unlockRemainderFromCurrent() const;

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the current position
		 *  plus "start" bytes.  Returns true on success or false
		 *  otherwise. */
		bool	unlockRemainderFromCurrent(off64_t start) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the end using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		bool	tryLockRemainderFromEnd(int16_t type) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the end plus "start" bytes using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		bool	tryLockRemainderFromEnd(int16_t type,
						off64_t start) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the end using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can be
		 *  established.  Returns true on success or false otherwise. */
		bool	lockRemainderFromEnd(int16_t type) const;

		/** Attempts to lock the remainder of the file starting at
		 *  the end plus "start" bytes using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can be
		 *  established.  Returns true on success or false otherwise. */
		bool	lockRemainderFromEnd(int16_t type, off64_t start) const;

		/** Checks to see if the remainder of the file starting at
		 *  the end can be locked or not using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK) but does not actually attempt
		 *  to lock the file.  Returns true if the lock can be
		 *  established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockRemainderFromEnd(int16_t type,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Checks to see if the remainder of the file starting at
		 *  the end plus "start" bytes can be locked or not using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		bool	checkLockRemainderFromEnd(int16_t type,
						off64_t start,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen) const;

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the end.
		 *  Returns true on success or false otherwise. */
		bool	unlockRemainderFromEnd() const;

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the end plus "start"
		 *  bytes.  Returns true on success or false otherwise. */
		bool	unlockRemainderFromEnd(off64_t start) const;


		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, sequentially so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	sequentialAccess(off64_t start, size_t len) const;

		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, randomly so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	randomAccess(off64_t start, size_t len) const;

		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, only one time so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	onlyOnce(off64_t start, size_t len) const;

		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, in the near future so the kernel can perform
		 *  some optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	willNeed(off64_t start, size_t len) const;

		/** Advises the kernel that you are not going to access the
		 *  region of the currently open file begining at byte "start",
		 *  for "len" bytes, in the near future so the kernel can
		 *  perform some optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	wontNeed(off64_t start, size_t len) const;

		/** Advises the kernel that you are going to access the
		 *  region of the currently open file begining at byte "start",
		 *  for "len" bytes, normally so the kernel can undo any
		 *  previously applied optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	normalAccess(off64_t start, size_t len) const;


		/** Reserves space on the storage medium such that a
		 *  write to the region starting at "start" and
		 *  continuing for "len" bytes will not fail due to
		 *  lack of storage space.
		 * 
		 *  Returns true if the region can be reserved and
		 *  false otherwise. */
		bool	reserve(off64_t start, size_t len) const;


		/** On most filesystems, changes to files are often cached in
		 *  system ram, this methods causes those changes (including
		 *  changes to file metadata such as last-access and
		 *  last-modifiction times) to be immediately copied to the
		 *  storage medium that the file resides on.
		 * 
		 *  Returns true on success and false on failure.
		 * 
		 *  Note that if a hard disk has on-board write cache
		 *  enabled, this method only assures that the changes
		 *  has been copied into the disk's write cache, not
		 *  necessarily to the disk itself. */
		bool	sync() const;

		/** On most filesystems, changes to files are often cached in
		 *  system ram, this methods causes those changes to be
		 *  immediately copied to the storage medium that the file
		 *  resides on.
		 *
		 *  This method differs from sync() in that it does not
		 *  wait until file metadata such as last-access or
		 *  last-modification times to be written before it returns.
		 * 
		 *  Returns true on success and false on failure.
		 * 
		 *  Note that if a hard disk has on-board write cache
		 *  enabled, this method only assures that the changes
		 *  has been copied into the disk's write cache, not
		 *  necessarily to the disk itself. */
		bool	dataSync() const;


		/** Causes the open() and create() methods to call
		 *  getCurrentProperties() internally so other methods of this
		 *  class such as getSize() will return valid data about the
		 *  file immediately.  This is the default behavior. */
		void	dontGetCurrentPropertiesOnOpen();

		/** Causes the open() and create() methods to not to call
		 *  getCurrentProperties() internally.  This offers a small
		 *  performance improvement which can become significant if you
		 *  intend to open lots of files but don't need to know the size
		 *  or any other properties about them.
		 *
		 *  If this method is called and a method such as getSize() is
		 *  called afterwards, then it will call getCurrentProperties()
		 *  internally. */
		void	getCurrentPropertiesOnOpen();

		/** Refreshes the current file properties as returned by
		 *  getSize(), getPermissions(), etc.
		 *  
		 *  Returns true on success and false on failure. */
		bool	getCurrentProperties();

		/** Returns the permissions settings for the file. */
		mode_t		getPermissions() const;

		/** Returns the user id of the file's owner. */
		uid_t		getOwnerUserId() const;

		/** Returns the group id of the file's owner. */
		gid_t		getOwnerGroupId() const;

		/** Returns the number of bytes in the file. */
		off64_t		getSize() const;

		/** Returns the blocksize to use for most
		 *  efficient I/O with the file or -1 for
		 *  systems don't support this. */
		blksize_t	getBlockSize() const;

		/** Returns the number of blocks allocated for
		 *  the file or -1 for systems don't support this. */
		blkcnt_t	getBlockCount() const;

		/** Returns 1 if the file is a socket,
		 *  0 if it's not or -1 on error. */
		int32_t		isSocket() const;

		/** Returns 1 if the file is a symbolic link,
		 *  0 if it's not or -1 on error. */
		int32_t		isSymbolicLink() const;

		/** Returns 1 if the file is a regular file,
		 *  0 if it's not or -1 on error. */
		int32_t		isRegularFile() const;

		/** Returns 1 if the file is a block device,
		 *  0 if it's not or -1 on error. */
		int32_t		isBlockDevice() const;

		/** Returns 1 if the file is a directory,
		 *  0 if it's not or -1 on error. */
		int32_t		isDirectory() const;
		/** Returns 1 if the file is a character device,
		 *  0 if it's not or -1 on error. */
		int32_t		isCharacterDevice() const;

		/** Returns 1 if the file is a fifo,
		 *  0 if it's not or -1 on error. */
		int32_t		isFifo() const;

		/** Returns the time of last access of the file. */
		time_t		getLastAccessTime() const;

		/** Returns the time of last modification of the file.
		 *
		 *  Modification time IS NOT affected by changing file
		 *  permissions, ownership or creating links. */
		time_t		getLastModificationTime() const;

		/** Returns the time of last change of the file.
		 *
		 *  Change time IS affected by changing file
		 *  permissions, ownership or creating links. */
		time_t		getLastChangeTime() const;

		/** Returns the device of the file. */
		dev_t		getDevice() const;

		/** Returns the device type of the file. */
		dev_t		getDeviceType() const;

		/** Returns the base inode of the file. */
		uint64_t	getInode() const;

		/** Returns the number of hard links to the file. */
		nlink_t		getNumberOfHardLinks() const;


		/** Sets the permissions of the file to "perms".
		 *  Returns true on success and false on failure. */
		bool	setPermissions(mode_t perms) const;


		/** Changes the user and/or group that owns the file.
		 *  Returns true on success and false on failure. */
		bool	changeOwner(const char *newuser,
					const char *newgroup) const;

		/** Changes the user that owns the file.
		 *  Returns true on success and false on failure. */
		bool	changeOwner(uid_t uid, gid_t gid) const;

		/** Returns true if any of the changeOwner()
		 *  methods are allowed on "filename" and
		 *  false otherwise. */
		bool	canChangeOwner() const;

		/** Returns the maximum number of links that can be
		 *  created to "filename". */
		int64_t	maxLinks() const;


		/** Returns a pointer to the file stats structure
		 *  used internally.  (struct stat * on unix) */
		void 	*getInternalFileStatisticsStructure();


		/** Create a file named "name" with permissions "perms".  If
		 *  the file already exists, it will be truncated.
		 *  Returns true on success and false on failure. */
		static bool	createFile(const char *name, mode_t perms);

		/** Creates a fifo called "filename" with permissions "perms".
		 *  Returns true on success and false on failure. */
		static bool	createFifo(const char *filename, mode_t perms);

		/** Creates a pipe and sets "readfd" to the read side and
		 *  "writefd" to the write side. */
		static bool	createPipe(filedescriptor *readfd,
						filedescriptor *writefd);

		/** Creates a temporary file using "templatefilename"
		 *  as a template.  The last 6 characters of
		 *  "templatefilename" must be XXXXXX and
		 *  "templatefilename" will be modified to contain the
		 *  name of the file that was actually created.
		 * 
		 *  Opens and returns the file descriptor handle of the
		 *  file on success and -1 on failure. */
		static int32_t	createTemporaryFile(char *templatefilename);

		/** Creates a hard link between "oldpath" and "newpath".
		 *  Returns true on success and false on failure. */
		static bool	createHardLink(const char *oldpath,
							const char *newpath);

		/** Creates a symbolic link between "oldpath" and
		 *  "newpath". Returns true on success and false on
		 *  failure. */
		static bool	createSymbolicLink(const char *oldpath,
							const char *newpath);

		/** Returns the pathname of the file that the symbolic
		 *  link "filename" points to.  Returns NULL on failure.
		 * 
		 *  Note that this method allocates a buffer for the
		 *  path internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static char	*resolveSymbolicLink(const char *filename);


		/** Renames "oldpath" to "newpath".  Returns true on
		 *  success and false on failure. */
		static bool	rename(const char *oldpath,
					const char *newpath);

		/** Removes file "filename".  Returns true on success
		 *  and false on failure.  (Will not remove a directory). */
		static bool	remove(const char *filename);


		/** Truncates the file. Returns true on success and false
		 *  on failure. */
		static	bool	truncate(const char *filename);

		/** Truncates the file after the first "length" bytes.
		 *  Returns true on success and false on failure. */
		static	bool	truncate(const char *filename, off64_t length);


		/** Allocates a string large enough to accommodate the
		 *  contents of the file "name" , reads the contents of
		 *  the file into the string, terminates it with a NULL
		 *  and returns the string.
		 *  The string must be freed by the calling program.
		 * 
		 *  If the file contains no data, then a string of length
		 *  zero is returned.  If an error occurs then a NULL
		 *  is returned. */
		static	char	*getContents(const char *name);

		/** Reads the contents of the file "name" into
		 *  "buffer" of length "buffersize".  If the file is
		 *  larger than "buffersize" then only "buffersize" bytes
		 *  of the file will be read into "buffer".
		 * 
		 *  Returns the number of bytes copied into "buffer" or
		 *  -1 on error. */
		static	ssize_t	getContents(const char *name,
						unsigned char *buffer,
						size_t buffersize);


		/** Returns true if the file exists and false otherwise. */
		static bool	exists(const char *filename);

		/** Returns true if "filename" is readable by the user
		 *  or false otherwise. */
		static bool	readable(const char *filename);

		/** Returns true if "filename" is writeable by the user
		 *  or false otherwise. */
		static bool	writeable(const char *filename);

		/** Returns true if "filename" is executable by the user
		 *  or false otherwise. */
		static bool	executable(const char *filename);

		/** Checks to see if "filename" exists, is readable,
		 *  is writeable and/or is executable by the user, based
		 *  on the value of "mode".  Mode should be an or'ed
		 *  combination of F_OK (exists), R_OK (readable),
		 *  W_OK (writable) and X_OK (executable) flags.
		 * 
		 *  Returns true if the file meets the conditions set
		 *  by the mode and false otherwise. */
		static bool	accessible(const char *filename, int32_t mode);

		/** Sets "ctime" to the last change time of "filename".
		 *  Returns true on success and false on failure.
		 *
		 *  Change time IS affected by changing file
		 *  permissions, ownership or creating links. */
		static bool	getLastChangeTime(const char *filename,
							time_t *ctime);

		/** Sets the permissions of the file to "perms".
		 *  Returns true on success and false on failure. */
		static bool	setPermissions(const char *filename,
							mode_t perms);

		/** Changes the user and/or group that owns the file.
		 *  Returns true on success and false on failure. */
		static bool	changeOwner(const char *filename,
						const char *newuser,
						const char *newgroup);

		/** Changes the user and/or group that owns the file.
		 *  Returns true on success and false on failure. */
		static bool	changeOwner(const char *filename,
						uid_t uid, gid_t gid);

		/** Returns true if any of the changeOwner methods are
 		 *  allowed on "filename" and false otherwise. */
		static bool	canChangeOwner(const char *filename);


		/** Overrides the last access time of the file, setting it to
		 *  "lastaccesstime".  Returns true on success and false on
		 *  failure. */
		static bool	setLastAccessTime(const char *filename,
							time_t lastaccesstime);

		/** Overrides the last modification time of the file, setting
		 *  it to "lastaccesstime".  Returns true on success and false
		 *  on failure. */
		static bool	setLastModificationTime(const char *filename,
							time_t lastmodtime);

		/** Overrides the last access and modification times of the
		 *  file, setting them to "lastaccesstime" and "lastmodtime"
		 *  respectively.  Returns true on success and false on
		 *  failure. */
		static bool	setLastAccessAndModificationTimes(
							const char *filename,
							time_t lastaccesstime,
							time_t lastmodtime);

		/** Overrides the last access and modification times of the
		 *  file, setting them both to the current system time. 
		 * Returns true on success and false on failure. */
		static bool	setLastAccessAndModificationTimes(
							const char *filename);


		/** Returns the directory portion of "filename".
		 *  This method allocates a buffer internally
		 *  and returns it.  The calling program must
		 *  deallocate the buffer. */
		static char	*dirname(const char *filename);

		/** Returns the non-directory portion of "filename".
		 *  This method allocates a buffer internally and returns it.
		 *  The calling program must deallocate the buffer. */
		static char	*basename(const char *filename);

		/** Returns the non-directory portion of
		 *  "filename", truncating "suffix".
		 *  This method allocates a buffer internally
		 *  and returns it.  The calling program must
		 *  deallocate the buffer. */
		static char	*basename(const char *filename,
						const char *suffix);

		/** Translates the basename of "filename" to an 8.3 format,
		 *  performing substitutions in the same way that Windows
		 *  does.  This method allocates a buffer internally and
		 *  returns it.  The calling program must deallocate the
		 *  buffer. */
		static char	*eightDotThree(const char *filename);


		/** Generates a key based on "filename" and the
		 *  least signifigant 8 bits of id (which must
		 *  be non-zero) suitable for use with
		 *  semaphores, shared memory segments and
		 *  message queues. */
		static key_t	generateKey(const char *filename, int32_t id);

		/** Returns the maximum number of links that
		 *  can be created to "filename". */
		static int64_t	maxLinks(const char *filename);

	#include <rudiments/private/file.h>
};

#endif
