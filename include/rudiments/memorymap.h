// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MEMORYMAP_H
#define RUDIMENTS_MEMORYMAP_H

#include <rudiments/private/memorymapincludes.h>

// The memorymap class provides methods for mapping a file (or file descriptor)
// to a region of memory and manipulating the mapped region.

class memorymap {
	public:
			memorymap();
			// creates an unattached memorymap
			~memorymap();
			// destroys the memorymap, detaching if necessary

		bool	attach(int fd, off_t offset, size_t len,
						int protection, int flags);
			// Attaches the memorymap to file descriptor "fd" at
			// "offset" for "len" bytes.
			//
			// "protection" may be:
			//	PROT_NONE - pages may not be accessed
			// or an or'ed combination of the following:
			// 	PROT_EXEC - pages may be excuted
			//	PROT_READ - pages may be read
			//	PROT_WRITE - pages may be written
			// "protection" may not conflict with the open mode
			// of the file. (eg. if the file was opened readonly
			// then PROT_WRITE may not be used).
			//
			// flags may be:
			//	MAP_SHARED
			//	or
			//	MAP_PRIVATE
			// and an or'ed combination of the following:
			//	MAP_FIXED 
			// 	MAP_DENYWRITE
			// 	MAP_EXECUTABLE
			// 	MAP_NORESERVE
			//	MAP_LOCKED
			//	MAP_GROWSDOWN
			//	MAP_ANONYMOUS
			//	MAP_ANON
			//	MAP_32BIT
			//	MAP_AUTOGROW
			//	MAP_AUTORESRV
			//	MAP_COPY
			//	MAP_LOCAL
			//
			// Returns true on success and false on failure.

		bool	detach();
			// detaches the memory map from the file descriptor
			//
			// Returns true on success and false on failure.

		bool	setProtection(int protection);
			// Sets the protection of the entire memory map to
			// "protection".  See protect() below for more info.
		bool	setProtection(off_t offset, size_t len, int protection);
			// Sets the protection of the memory map to "protection"
			// for "len" bytes, starting at "offset".
			//
			// "protection" may be:
			//	PROT_NONE - pages may not be accessed
			// or an or'ed combination of the following:
			// 	PROT_EXEC - pages may be excuted
			//	PROT_READ - pages may be read
			//	PROT_WRITE - pages may be written
			// "protection" may not conflict with the open mode
			// of the file. (eg. if the file was opened readonly
			// then PROT_WRITE may not be used).
			//
			// Returns true on success and false on failure.

		void	*getData();
			// Returns a pointer to the region of memory that the
			// file is mapped into.
		size_t	getLength();
			// Returns the length of the region of memory that the
			// file is mapped into.

		bool	sync(bool immediate, bool invalidate);
			// Make sure that changes made to the memory map have
			// been copied to the storage mediam that the mapped
			// file resides on.  See sync() below for more info.
		bool	sync(off_t offset, size_t len,
				bool immediate, bool invalidate);
			// Make sure that changes made to the memory map for
			// "len" bytes, starting at "offset" have been copied
			// to the storage mediam that the mapped file resides
			// on.
			//
			// If "immediate" is true, the method will not return
			// until the synchronization has finished, if it is
			// false, the synchronization will occur in the
			// background.
			//
			// If "invalidate" is true, all other mappings of the
			// file will be invalidated.
			//
			// Returns true on success and false on failure.


		// These methods allow you to advise the kernel that you are
		// going to access a region of a file in a particular manner.
		// The kernel can then perform some optimisations.
		//
		// In these methods, the region of the file begins at "offset"
		// and continues for "len" bytes.
		//
		// These methods return true on success and false on failure.
		bool	sequentialAccess(off_t offset, size_t len);
			// The region will be accessed in sequential order.
		bool	randomAccess(off_t offset, size_t len);
			// The region will be accessed in random order.
		bool	willNeed(off_t offset, size_t len);
			// The region will be accessed in the near future.
		bool	wontNeed(off_t offset, size_t len);
			// The region will not be accessed in the near future.
		bool	normalAccess(off_t offset, size_t len);
			// Removes any advice that has previously been applied
			// to the region.

		#ifdef HAVE_MLOCK
		bool	lock();
			// Disables paging of the entire memory map.
			// Returns true on success and false on failure.
		bool	lock(off_t offset, size_t len);
			// Disables paging of data in the memory map, starting
			// at "offset", for "len" bytes.
			// Returns true on success and false on failure.
		#endif

		#ifdef HAVE_MUNLOCK
		bool	unlock();
			// Enables paging of the entire memory map.
			// Returns true on success and false on failure.
		bool	unlock(off_t offset, size_t len);
			// Enables paging of data in the memory map, starting
			// at "offset", for "len" bytes.
			// Returns true on success and false on failure.
		#endif

		#ifdef HAVE_MINCORE
		bool	inMemory();
			// Returns true if all pages of the memory map are
			// currently cached in system ram.
			// Returns true on success and false on failure.
		bool	inMemory(off_t offset, size_t len);
			// Returns true if all pages of the memory map starting
			// at "offset", for "len" bytes are currently cached in
			// system ram.
			// Returns true on success and false on failure.
		#endif


		#ifdef HAVE_MLOCK
		static bool	lockAll();
				// Disables paging (storing to swap partition)
				// of all pages of memory currently used for
				// memorymaps by the process and all newly
				// allocated pages.
				// Returns true on success and false on failure.
		static bool	lockAllCurrent();
				// Disables paging (storing to swap partition)
				// of all pages of memory currently used for
				// memorymaps by the process.  Newly allocated
				// pages may still be paged out.
				// Returns true on success and false on failure.
		static bool	lockAllFuture();
				// Disables paging (storing to swap partition)
				// of all newly allocated pages of memory used
				// for memorymaps by the process.  Currently
				// allocated pages may still be paged out.
				// Returns true on success and false on failure.
		#endif
		#ifdef HAVE_MUNLOCK
		static bool	unlockAll();
				// Enables paging of data stored in all
				// memorymaps currently open by the process.
				// Returns true on success and false on failure.
		#endif

	#include <rudiments/private/memorymap.h>
};

#endif
