// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MEMORYMAP_H
#define RUDIMENTS_MEMORYMAP_H

#include <rudiments/private/memorymap.h>

// wrap:
//	sys/mman.h - mmap(),munmap(),mprotect(),msync()
//			madvise()/posix_madvise()
//			mlock(),munlock(),munlockall(),
//			mremap()
//			mincore()
//	linux only - 
//		sys/mman.h - remap_file_pages()
//	not in bsd - 
//		sys/mman.h - shm_open(),shm_unlink()

class memorymap {
	public:

	#include <rudiments/private/memorymap.h>
};

#endif
