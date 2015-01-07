// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_BYTEBUFFER_H
#define RUDIMENTS_BYTEBUFFER_H

#include <rudiments/private/bytebufferincludes.h>

class bytebufferprivate;

/** The bytebuffer class can be used to store binary data of arbitrary length.
 *
 *  Internally, the class stores this data in a series of extents which are
 *  coalesced when getBuffer() or detachBuffer() are called.  The size of the
 *  initial and incremental extents are defined in the constructor.
 *
 *  When cleared, all extents except for the initial extent are freed.
 *
 *  It is generally more efficient to reuse the same bytebuffer over and
 *  over than to allocate a new one for each operation. */
class RUDIMENTS_DLLSPEC bytebuffer {
	public:

		/** Creates an instance of the bytebuffer class with
		 *  the specified "initialsize" and "increment" parameters. */
		bytebuffer(size_t initialsize, size_t increment);

		/** Creates an instance of the bytebuffer class.
		 *  The buffer "initialcontents" will be attached to the
		 *  instance and used as the initial contents.  This buffer
		 *  should not be freed by the calling program.  The initial
		 *  size and increment may be specified respectively by the
		 *  "initialsize" and "increment" parameters. */
		bytebuffer(unsigned char *initialcontents,
					size_t initialsize, size_t increment);

		/** Creates an instance of the bytebuffer class
		 *  that is a copy of "v". */
		bytebuffer(const bytebuffer &v);

		/** Makes this instance of the bytebuffer class
		 *  identical to "v". */
		bytebuffer	&operator=(const bytebuffer &v);

		/** Deletes this instance of the bytebuffer class. */
		virtual	~bytebuffer();

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
		 *  bytebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(const unsigned char *data, size_t size);

		/** Writes "string" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(const char *string);

		/** Writes the first "size" bytes of "string" to the
		 *  bytebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(const char *string, size_t size);

		/** Writes "character" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(char character);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(int16_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(int32_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(int64_t number);

		/** Writes "character" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(unsigned char character);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(uint16_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(uint32_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(uint64_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(float number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->write("numbers: ")->write(5)->write(5.5); */
		bytebuffer	*write(double number);

		/** Writes "..." to the byte buffer using "format"
		 *  which should comply with standard printf formatting
		 *  rules. */
		bytebuffer	*writeFormatted(const char *format, ...);

		/** Writes "argp" to the byte buffer using "format"
		 *  which should comply with standard printf formatting
		 *  rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list. */
		bytebuffer	*writeFormatted(const char *format,
							va_list *argp);


		/** Appends the first "size" bytes of "data" to the
		 *  bytebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const unsigned char *data, size_t size);

		/** Appends "string" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const char *string);

		/** Appends the first "size" bytes of "string" to the
		 *  bytebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const char *string, size_t size);

		/** Appends "character" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(char character);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(int16_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(int32_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(int64_t number);

		/** Appends "character" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(unsigned char character);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(uint16_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(uint32_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(uint64_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(float number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(double number);

		/** Appends "..." to the byte buffer using "format"
		 *  which should comply with standard printf formatting
		 *  rules. */
		bytebuffer	*appendFormatted(const char *format, ...);

		/** Appends "argp" to the byte buffer using "format"
		 *  which should comply with standard printf formatting
		 *  rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list. */
		bytebuffer	*appendFormatted(const char *format,
							va_list *argp);

		/** Truncates the bytebuffer at position "pos". */
		virtual void	truncate(size_t pos);

		/** Truncates the bytebuffer at the current position. */
		virtual void	truncate();


		/** Reads "size" bytes from the bytebuffer at the
		 *  current position into "data".  Also increments the
		 *  current position by "size" bytes. */
		ssize_t	read(unsigned char *data, size_t size);

		/** Empties the bytebuffer. */
		void	clear();

		/** Returns the current data stored in the bytebuffer. */
		const unsigned char	*getBuffer();

		/** Returns the amount of data currently stored in the
		 *  bytebuffer. */
		size_t		getSize();

		/** Returns the position in the buffer at which the next
		 *  append will occur. */
		size_t		getEnd();

		/** Returns the actual size of the buffer which
		 *  may be larger than the value returned by
		 *  getSize() since the buffer grows in chunks. */
		size_t		getActualSize();

		/** Returns a pointer to the buffer currently
		 *  stored in the bytebuffer, then resets
		 *  the bytebuffer such that it will no
		 *  longer use that buffer.
		 * 
		 *  The calling program must deallocate the
		 *  buffer returned from this method. */
		unsigned char	*detachBuffer();

	#include <rudiments/private/bytebuffer.h>
};

#endif
