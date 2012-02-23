// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MEMORYMAP_H
#define RUDIMENTS_MEMORYMAP_H

#include <rudiments/private/memorymapincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class memorymapprivate;

/** The memorymap class provides methods for mapping a file (or file descriptor)
 *  to a region of memory and manipulating the mapped region. */
class RUDIMENTS_DLLSPEC memorymap {
	public:
		
		/** Creates an instance of the memorymap class. */
		memorymap();

		/** Deletes this instance of the memorymap class, calling
 		 *  detach() if necessary. */
		~memorymap();

		/** Attaches the memorymap to file descriptor "fd" at
		 *  "offset" for "len" bytes.
		 * 
		 *  "protection" may be:
		 * 	PROT_NONE - pages may not be accessed
		 *  or an or'ed combination of the following:
		 *  	PROT_EXEC - pages may be excuted
		 * 	PROT_READ - pages may be read
		 * 	PROT_WRITE - pages may be written
		 *  "protection" may not conflict with the open mode
		 *  of the file. (eg. if the file was opened readonly
		 *  then PROT_WRITE may not be used).
		 * 
		 *  flags may be:
		 * 	MAP_SHARED
		 * 	or
		 * 	MAP_PRIVATE
		 *  and an or'ed combination of the following:
		 * 	MAP_FIXED 
		 *  	MAP_DENYWRITE
		 *  	MAP_EXECUTABLE
		 *  	MAP_NORESERVE
		 * 	MAP_LOCKED
		 * 	MAP_GROWSDOWN
		 * 	MAP_ANONYMOUS
		 * 	MAP_ANON
		 * 	MAP_32BIT
		 * 	MAP_AUTOGROW
		 * 	MAP_AUTORESRV
		 * 	MAP_COPY
		 * 	MAP_LOCAL
		 *
		 *  Note that not all filesystems support all of those options.
		 * 
		 *  Returns true on success and false on failure. */
		bool	attach(int32_t fd, off64_t offset, size_t len,
					int32_t protection, int32_t flags);

		/** detaches the memory map from the file descriptor
		 * 
		 *  Returns true on success and false on failure. */
		bool	detach();

		/** Sets the protection of the entire memory map to
		 *  "protection".  See protect() below for more info. */
		bool	setProtection(int32_t protection);

		/** Sets the protection of the memory map to "protection"
		 *  for "len" bytes, starting at "offset".
		 * 
		 *  "protection" may be:
		 * 	PROT_NONE - pages may not be accessed
		 *  or an or'ed combination of the following:
		 *  	PROT_EXEC - pages may be excuted
		 * 	PROT_READ - pages may be read
		 * 	PROT_WRITE - pages may be written
		 *  "protection" may not conflict with the open mode
		 *  of the file. (eg. if the file was opened readonly
		 *  then PROT_WRITE may not be used).
		 *
		 *  Note that not all filesystems support all of those options.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setProtection(off64_t offset, size_t len,
							int32_t protection);

		/** Returns a pointer to the region of memory that the
		 *  file is mapped into. */
		void	*getData();

		/** Returns the length of the region of memory that the
		 *  file is mapped into. */
		size_t	getLength();

		/** Make sure that changes made to the memory map have
		 *  been copied to the storage mediam that the mapped
		 *  file resides on.
		 * 
		 *  If "immediate" is true, the method will not return
		 *  until the synchronization has finished, if it is
		 *  false, the synchronization will occur in the
		 *  background.
		 * 
		 *  If "invalidate" is true, all other mappings of the
		 *  file will be invalidated.
		 * 
		 *  Returns true on success and false on failure. */
		bool	sync(bool immediate, bool invalidate);

		/** Make sure that changes made to the memory map for
		 *  "len" bytes, starting at "offset" have been copied
		 *  to the storage mediam that the mapped file resides
		 *  on.
		 * 
		 *  If "immediate" is true, the method will not return
		 *  until the synchronization has finished, if it is
		 *  false, the synchronization will occur in the
		 *  background.
		 * 
		 *  If "invalidate" is true, all other mappings of the
		 *  file will be invalidated.
		 * 
		 *  Returns true on success and false on failure. */
		bool	sync(off64_t offset, size_t len,
				bool immediate, bool invalidate);


		/** Advises the kernel that you are going to access the region
		 *  of the currently mapped file begining at byte "offset", for
		 *  "len" bytes, sequentially so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	sequentialAccess(off64_t offset, size_t len);

		/** Advises the kernel that you are going to access the region
		 *  of the currently mapped file begining at byte "offset", for
		 *  "len" bytes, randomly so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	randomAccess(off64_t offset, size_t len);

		/** Advises the kernel that you are going to access the region
		 *  of the currently mapped file begining at byte "offset", for
		 *  "len" bytes, in the near future so the kernel can perform
		 *  some optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	willNeed(off64_t offset, size_t len);

		/** Advises the kernel that you are not going to access the
		 *  region of the currently mapped file begining at byte
		 *  "offset", for "len" bytes, in the near future so the kernel
		 *  can perform some optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	wontNeed(off64_t offset, size_t len);

		/** Advises the kernel that you are going to access the
		 *  region of the currently mapped file begining at byte
		 *  "offset", for "len" bytes, normally so the kernel can undo
		 *  any previously applied optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		bool	normalAccess(off64_t offset, size_t len);

		/** Disables paging of the entire memory map.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking, this method
		 *  returns false. */
		bool	lock();

		/** Disables paging of data in the memory map, starting
		 *  at "offset", for "len" bytes.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking, this method
		 *  returns false. */
		bool	lock(off64_t offset, size_t len);

		/** Enables paging of the entire memory map.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking, this method
		 *  returns false. */
		bool	unlock();

		/** Enables paging of data in the memory map, starting
		 *  at "offset", for "len" bytes.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking, this method
		 *  returns false. */
		bool	unlock(off64_t offset, size_t len);

		/** Returns true if all pages of the memory map are
		 *  currently cached in system ram.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support checking whether
		 *  pages of the memory map are currently cached
		 *  in system ram, this method returns false. */
		bool	inMemory();

		/** Returns true if all pages of the memory map starting
		 *  at "offset", for "len" bytes are currently cached in
		 *  system ram.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support checking whether
		 *  pages of the memory map are currently cached
		 *  in system ram, this method returns false. */
		bool	inMemory(off64_t offset, size_t len);


		/** Disables paging (storing to swap partition)
		 *  of all pages of memory currently used for
		 *  memorymaps by the process and all newly
		 *  allocated pages.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking,
		 *  this method returns false; */
		static bool	lockAll();

		/** Disables paging (storing to swap partition)
		 *  of all pages of memory currently used for
		 *  memorymaps by the process.  Newly allocated
		 *  pages may still be paged out.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking,
		 *  this method returns false; */
		static bool	lockAllCurrent();

		/** Disables paging (storing to swap partition)
		 *  of all newly allocated pages of memory used
		 *  for memorymaps by the process.  Currently
		 *  allocated pages may still be paged out.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking,
		 *  this method returns false; */
		static bool	lockAllFuture();

		/** Enables paging of data stored in all
		 *  memorymaps currently open by the process.
		 *  Returns true on success and false on failure.
		 * 
		 *  On systems that don't support locking,
		 *  this method returns false; */
		static bool	unlockAll();

	#include <rudiments/private/memorymap.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
