// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MEMORYPOOL_H
#define RUDIMENTS_MEMORYPOOL_H

#include <rudiments/private/memorypoolincludes.h>

class memorypoolprivate;

/** The memorypool class provides methods for creating and using a memory pool.
 *
 *  If you have an iterative process that requires variable amounts of ram
 *  for each iteration, using a memory pool can be faster than allocating
 *  and deallocating memory on-demand and less resource-intensive than
 *  allocating static buffers that are large enough to accommodate the maximum
 *  amount of data you may have to store. */
class RUDIMENTS_DLLSPEC memorypool {
	public:
		/** Creates a memory pool of initial size "initialsize".
		 * 
		 *  When the pool needs to grow, it will grow by at least
		 *  "increment" bytes.  If more than increment bytes
		 *  are requested, it will grow by that amount instead.
		 * 
		 *  When free() has been called "resizeinterval" times,
		 *  it will evaluate the average amount of memory
		 *  malloc'ed (since the last time it did this) and
		 *  resize the initial buffer size to this size. */
		memorypool(size_t initialsize,
				size_t increment,
				size_t resizeinterval);

		/** Deletes this instance of the memorypool class. */
		~memorypool();

		/** Returns a pointer to a contiguous block of "size"
		 *  bytes in the pool.  The pool will grow as necessary
		 *  to accomodate allocations. */
		unsigned char	*malloc(size_t size);

		/** Returns a pointer to a contiguous block of "size"
		 *  bytes in the pool.  The pool will grow as necessary
		 *  to accomodate allocations.  Each byte of data in
		 *  the block of data is set to NULL. */
		unsigned char	*calloc(size_t size);

		/** Shrinks the pool back down to it's initial size
		 *  and frees all previously allocated blocks. */
		void	free();

		/** Prints a visual representation of the pool. */
		void	print();

	#include <rudiments/private/memorypool.h>
};

#endif
