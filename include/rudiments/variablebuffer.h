// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_VARIABLEBUFFER_H
#define RUDIMENTS_VARIABLEBUFFER_H

#include <rudiments/private/variablebufferincludes.h>

// The variablebuffer class can be used to store raw data of arbitrary length.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class variablebufferprivate;

class RUDIMENTS_DLLSPEC variablebuffer {
	public:
			variablebuffer(size_t initialsize, size_t increment);
			variablebuffer(unsigned char *initialcontents,
					size_t initialsize, size_t increment);
			// Creates a new buffer of size "initialsize"
			// which will grow in chunks of size "increment".
			// The position at which the first write or append
			// will occur is initialized to 0.
			variablebuffer(const variablebuffer &v);
		variablebuffer	&operator=(const variablebuffer &v);
		virtual	~variablebuffer();

		void	setPosition(size_t pos);
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
		variablebuffer	*write(const unsigned char *data, size_t size);
		variablebuffer	*write(const char *string);
		variablebuffer	*write(const char *string, size_t size);
		variablebuffer	*write(char character);
		variablebuffer	*write(int16_t number);
		variablebuffer	*write(int32_t number);
		variablebuffer	*write(int64_t number);
		variablebuffer	*write(unsigned char character);
		variablebuffer	*write(uint16_t number);
		variablebuffer	*write(uint32_t number);
		variablebuffer	*write(uint64_t number);
		variablebuffer	*write(float number);
		variablebuffer	*write(double number);
			// Writes "data" to the variablebuffer at the current
			// position.  If necessary, the buffer will grow to
			// accommodate the new data.
		variablebuffer	*append(const unsigned char *data, size_t size);
		variablebuffer	*append(const char *string);
		variablebuffer	*append(const char *string, size_t size);
		variablebuffer	*append(char character);
		variablebuffer	*append(int16_t number);
		variablebuffer	*append(int32_t number);
		variablebuffer	*append(int64_t number);
		variablebuffer	*append(unsigned char character);
		variablebuffer	*append(uint16_t number);
		variablebuffer	*append(uint32_t number);
		variablebuffer	*append(uint64_t number);
		variablebuffer	*append(float number);
		variablebuffer	*append(double number);
			// Appends "data" to the variablebuffer.  The buffer
			// will grow to accommodate the new data.

		ssize_t	read(unsigned char *data, size_t size);
			// Reads "size" bytes from the variablebuffer at the
			// current position into "data".  Also increments the
			// current position by "size" bytes.

		void	clear();
			// Empties the variablebuffer.

		const unsigned char	*getBuffer();
				// Returns the current data stored in the
				// variablebuffer.
		size_t		getSize();
				// Returns the current amount of data stored
				// in the variablebuffer.
		size_t		getPosition();
				// Returns the position in the buffer at which
				// the next write will occur.
		size_t		getEnd();
				// Returns the position in the buffer at which
				// the next append will occur.
		size_t		getActualSize();
				// Returns the actual size of the buffer which
				// may be larger than the value returned by
				// getSize() since the buffer grows in chunks.

		unsigned char	*detachBuffer();
				// Returns a pointer to the buffer currently
				// stored in the variablebuffer, then resets
				// the variablebuffer such that it will no
				// longer use that buffer.
				//
				// The calling program must deallocate the
				// buffer returned from this method.

	#include <rudiments/private/variablebuffer.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
