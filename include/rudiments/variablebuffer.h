// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_VARIABLEBUFFER_H
#define RUDIMENTS_VARIABLEBUFFER_H

#include <rudiments/private/variablebufferincludes.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class variablebufferprivate;

/** The variablebuffer class can be used to store raw data of arbitrary length.
 *
 *  The class allocates a buffer internally of either a default or specified
 *  initial size.  If a write or append operation would exceed the size of the
 *  existing buffer, then it is grown by a multiple of either a default or
 *  specified incremental size.  This growth is accomplished by allocating a
 *  new, larger buffer and copying the contents of the old buffer into it along
 *  with whatever data was written or appended.
 *
 *  If the buffer is cleared, then if the existing buffer has grown beyond its
 *  initial size then it is deleted and a new buffer is allocated using the
 *  initial size.  However if the buffer has not grown then no action is
 *  taken except to reset the internal position pointers. 
 *
 *  It is generally more efficient to reuse the same variablebuffer over and
 *  over than to allocate a new one for each operation. */
class RUDIMENTS_DLLSPEC variablebuffer {
	public:

		/** Creates an instance of the variablebuffer class with
		 *  the specified "initialsize" and "increment" parameters. */
		variablebuffer(size_t initialsize, size_t increment);

		/** Creates an instance of the variablebuffer class.
		 *  The buffer "initialcontents" will be attached to the
		 *  instance and used as the initial contents.  This buffer
		 *  should not be freed by the calling program.  The initial
		 *  size and increment may be specified respectively by the
		 *  "initialsize" and "increment" parameters. */
		variablebuffer(unsigned char *initialcontents,
					size_t initialsize, size_t increment);

		/** Creates an instance of the variablebuffer class
		 *  that is a copy of "v". */
		variablebuffer(const variablebuffer &v);

		/** Makes this instance of the variablebuffer class
		 *  identical to "v". */
		variablebuffer	&operator=(const variablebuffer &v);

		/** Deletes this instance of the variablebuffer class. */
		virtual	~variablebuffer();

		/** Sets the position in the internal buffer at which the next
		 *  read or write will occur to "pos".  If the position is set
		 *  beyond the end of the buffer, the buffer will grow but the
		 *  data between the current end of the buffer and the new
		 *  position will be undefined. */
		void	setPosition(size_t pos);

		/** Returns the position in the buffer at which
		 *  the next write will occur. */
		size_t	getPosition();

		/** Writes the first "size" bytes of "data" to the
		 *  variablebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(const unsigned char *data, size_t size);

		/** Writes "string" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(const char *string);

		/** Writes the first "size" bytes of "string" to the
		 *  variablebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(const char *string, size_t size);

		/** Writes "character" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(char character);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(int16_t number);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(int32_t number);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(int64_t number);

		/** Writes "character" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(unsigned char character);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(uint16_t number);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(uint32_t number);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(uint64_t number);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(float number);

		/** Writes "number" to the variablebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		variablebuffer	*write(double number);

		/** Appends the first "size" bytes of "data" to the
		 *  variablebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(const unsigned char *data, size_t size);

		/** Appends "string" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(const char *string);

		/** Appends the first "size" bytes of "string" to the
		 *  variablebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(const char *string, size_t size);

		/** Appends "character" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(char character);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(int16_t number);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(int32_t number);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(int64_t number);

		/** Appends "character" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(unsigned char character);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(uint16_t number);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(uint32_t number);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(uint64_t number);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(float number);

		/** Appends "number" to the variablebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		variablebuffer	*append(double number);

		/** Reads "size" bytes from the variablebuffer at the
		 *  current position into "data".  Also increments the
		 *  current position by "size" bytes. */
		ssize_t	read(unsigned char *data, size_t size);

		/** Empties the variablebuffer. */
		void	clear();

		/** Returns the current data stored in the variablebuffer. */
		const unsigned char	*getBuffer();

		/** Returns the amount of data currently stored in the
		 *  variablebuffer. */
		size_t		getSize();

		/** Returns the position in the buffer at which the next
		 *  append will occur. */
		size_t		getEnd();

		/** Returns the actual size of the buffer which
		 *  may be larger than the value returned by
		 *  getSize() since the buffer grows in chunks. */
		size_t		getActualSize();

		/** Returns a pointer to the buffer currently
		 *  stored in the variablebuffer, then resets
		 *  the variablebuffer such that it will no
		 *  longer use that buffer.
		 * 
		 *  The calling program must deallocate the
		 *  buffer returned from this method. */
		unsigned char	*detachBuffer();

	#include <rudiments/private/variablebuffer.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
