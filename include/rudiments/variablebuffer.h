// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_VARIABLEBUFFER_H
#define RUDIMENTS_VARIABLEBUFFER_H

#include <rudiments/private/config.h>

// The variablebuffer class can be used to store raw data of arbitrary length.

class variablebuffer {
	public:
			variablebuffer(unsigned long initialsize,
						unsigned long increment);
			// Creates a new buffer of size "initialsize"
			// which will grow in chunks of size "increment".
			// The position at which the first write or append
			// will occur is initialized to 0.
		virtual	~variablebuffer();

		void	setPosition(unsigned long pos);
			// Sets the position at which the next read or write
			// will occur to "pos".  If the position is set beyond
			// the end of the buffer, the buffer will grow but the
			// data between the current end of the buffer and the
			// new position will be undefined.

		// The write() and append() methods return a pointer to the
		// variablebuffer instance.  This enables chaining:
		//
		//	sb->write(data1)->write(data2)->write(data3);
		//	 	or
		//	sb->append(data1)->append(data2)->append(data3);
		variablebuffer	*write(const unsigned char *data,
						unsigned long size);
			// Writes "data" to the variablebuffer at the current
			// position.  If necessary, the buffer will grow to
			// accommodate the new data.
		variablebuffer	*append(const unsigned char *data,
						unsigned long size);
			// Appends "data" to the variablebuffer.  The buffer
			// will grow to accommodate the new data.

		unsigned long	read(unsigned char *data,
						unsigned long size);
			// Reads "size" bytes from the variablebuffer at the
			// current position into "data".  Also increments the
			// current position by "size" bytes.

		void	clear();
			// Empties the variablebuffer.

		unsigned char	*getBuffer();
				// Returns the current data stored in the
				// variablebuffer.
		unsigned long	getSize();
				// Returns the current amount of data stored
				// in the variablebuffer.
		unsigned long	getPosition();
				// Returns the position in the buffer at which
				// the next write will occur.
		unsigned long	getEnd();
				// Returns the position in the buffer at which
				// the next append will occur.
		unsigned long	getActualSize();
				// Returns the actual size of the buffer which
				// may be larger than the value returned by
				// getSize() since the buffer grows in chunks.

	#include <rudiments/private/variablebuffer.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/variablebufferinlines.h>
#endif

#endif
