// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STRINGBUFFER_H
#define RUDIMENTS_STRINGBUFFER_H

#include <rudiments/private/stringbufferincludes.h>

class stringbufferprivate;

/** The stringbuffer class can be used to store strings of arbitrary length.
 *
 *  Internally, the class stores this data in a series of extents which are
 *  coalesced when getString() or detachString() are called.  The size of the
 *  initial and incremental extents may be defined in the constructor.
 *
 *  When cleared, all extents except for the initial extent are freed.
 *
 *  It is generally more efficient to reuse the same stringbuffer over and
 *  over than to allocate a new one for each operation. */
class RUDIMENTS_DLLSPEC stringbuffer : public bytebuffer {
	public:

		/** Creates an instance of the stringbuffer class with
		 *  default initial size and increment parameters. */
		stringbuffer();

		/** Creates an instance of the stringbuffer class with the
		 *  specified "initialsize" and "increment" parameters. */
		stringbuffer(size_t initialsize, size_t increment);

		/** Creates an instance of the stringbuffer class.
		 *  The buffer "initialcontents" will be attached to the
		 *  instance and used as the initial contents.  This buffer
		 *  should not be freed by the calling program.  The initial
		 *  size and increment may be specified respectively by the
		 *  "initialsize" and "increment" parameters. */
		stringbuffer(char *initialcontents,
				size_t initialsize, size_t increment);

		/** Creates an instance of the stringbuffer class
		 *  that is a copy of "s". */
		stringbuffer(const stringbuffer &s);

		/** Makes this instance of the stringbuffer class
		 *  identical to "s". */
		stringbuffer	&operator=(const stringbuffer &s);

		/** Deletes this instance of the stringbuffer class. */
		~stringbuffer();

		/** Sets the position in the internal buffer at which the next
		 *  write will occur to "pos".  If the position is set beyond
		 *  the end of the buffer, the buffer will grow but the data
		 *  between the current end of the buffer and the new position
		 *  will be undefined. */
		void	setPosition(size_t pos);

		/** Returns the position in the internal buffer at which
		 *  the next write will occur. */
		size_t	getPosition();

		/** Writes "string" to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(const unsigned char *string);

		/** Writes the first "size" bytes of "string" to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(const unsigned char *string,
							size_t size);

		/** Writes "string" to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(const char *string);

		/** Writes the first "size" bytes of "string" to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(const char *string, size_t size);

		/** Writes "character" to the stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(char character);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(int16_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(int32_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the inernal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(int64_t number);

		/** Writes "character" to the stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(unsigned char character);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(uint16_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(uint32_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(uint64_t number);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the interna
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the stringbuffer
		 *  at the current position and increments the current position
		 *  to the next byte after the data that was written.  If
		 *  necessary, the internal buffer will grow to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(float number, uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the stringbuffer
		 *  at the current position and increments the current position
		 *  to the next byte after the data that was written.  If
		 *  necessary, the internal buffer will grow to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		stringbuffer	*write(double number, uint16_t precision,
							uint16_t scale);


		/** Appends "string" to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const unsigned char *string);

		/** Appends the first "size" bytes of "string" to the
		 *  stringbuffer, growing the internall buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const unsigned char *string,
							size_t size);

		/** Appends "string" to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const char *string);

		/** Appends the first "size" bytes of "string" to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const char *string, size_t size);

		/** Appends "character" to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(char character);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int16_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int32_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int64_t number);

		/** Appends "character" to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(unsigned char character);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint16_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint32_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint64_t number);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int16_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int32_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int64_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint16_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint32_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint64_t number, uint16_t zeropadding);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the stringbuffer,
		 *  growing the internal buffer as necessary to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(float number, uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the stringbuffer,
		 *  growing the internal buffer as necessary to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(double number, uint16_t precision,
							uint16_t scale);

		/** Trucnates the stringbuffer at position "pos". */
		void	truncate(size_t pos);

		/** Trucnates the stringbuffer at the current position. */
		void	truncate();

		/** Empties the stringbuffer. */
		void	clear();

		/** Returns the string currently stored in the stringbuffer. */
		const char	*getString();

		/** Returns a pointer to the string currently stored
		 *  in the stringbuffer, then resets the stringbuffer
		 *  such that it will no longer use that string.
		 * 
		 *  The calling program must deallocate the string
		 *  returned from this method. */
		char	*detachString();

		/** Returns the length of the string currently stored
		 *  in the stringbuffer. */
		size_t	getStringLength();

	#include <rudiments/private/stringbuffer.h>
};

#endif
