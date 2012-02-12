// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STRINGBUFFER_H
#define RUDIMENTS_STRINGBUFFER_H

#include <rudiments/private/stringbufferincludes.h>

// The stringbuffer class can be used to store strings of arbitrary length.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class stringbufferprivate;

class DLLSPEC stringbuffer : public variablebuffer {
	public:
				stringbuffer();
				stringbuffer(char *initialcontents,
						size_t initialsize,
						size_t increment);
				// Creates a new buffer which will grow as
				// necessary to accomodate the string written
				// to it.
				stringbuffer(const stringbuffer &s);
		stringbuffer	&operator=(const stringbuffer &s);
				~stringbuffer();

		void	setPosition(size_t pos);
			// Sets the position at which the next write will
			// occur to "pos".  If the position is set beyond the
			// end of the buffer, the buffer will grow but the data
			// between the current end of the buffer and the new
			// position will be undefined.

		// The write() and append() methods return a pointer to the
		// variablebuffer instance.  This enables chaining:
		//
		//	sb->write("numbers: ")->write(5)->write(5.5);
		//	 	or
		//	sb->append("numbers: ")->append(5)->append(5.5);
		stringbuffer	*write(const unsigned char *string);
		stringbuffer	*write(const unsigned char *string,
							size_t size);
		stringbuffer	*write(const char *string);
		stringbuffer	*write(const char *string, size_t size);
		stringbuffer	*write(char character);
		stringbuffer	*write(int16_t number);
		stringbuffer	*write(int32_t number);
		stringbuffer	*write(int64_t number);
		stringbuffer	*write(unsigned char number);
		stringbuffer	*write(uint16_t number);
		stringbuffer	*write(uint32_t number);
		stringbuffer	*write(uint64_t number);
		stringbuffer	*write(float number);
		stringbuffer	*write(float number, uint16_t scale);
		stringbuffer	*write(float number, uint16_t precision,
							uint16_t scale);
		stringbuffer	*write(double number);
		stringbuffer	*write(double number, uint16_t scale);
		stringbuffer	*write(double number, uint16_t precision,
							uint16_t scale);
			// Writes the data to the stringbuffer at the current
			// position.  If necessary, the buffer will grow to
			// accommodate the new data.
		stringbuffer	*append(const unsigned char *string);
		stringbuffer	*append(const unsigned char *string,
							size_t size);
		stringbuffer	*append(const char *string);
		stringbuffer	*append(const char *string, size_t size);
		stringbuffer	*append(char character);
		stringbuffer	*append(int16_t character);
		stringbuffer	*append(int32_t number);
		stringbuffer	*append(int64_t number);
		stringbuffer	*append(unsigned char number);
		stringbuffer	*append(uint16_t number);
		stringbuffer	*append(uint32_t number);
		stringbuffer	*append(uint64_t number);
		stringbuffer	*append(int16_t number, uint16_t zeropadding);
		stringbuffer	*append(int32_t number, uint16_t zeropadding);
		stringbuffer	*append(int64_t number, uint16_t zeropadding);
		stringbuffer	*append(uint16_t number, uint16_t zeropadding);
		stringbuffer	*append(uint32_t number, uint16_t zeropadding);
		stringbuffer	*append(uint64_t number, uint16_t zeropadding);
		stringbuffer	*append(float number);
		stringbuffer	*append(float number, uint16_t scale);
		stringbuffer	*append(float number, uint16_t precision,
							uint16_t scale);
		stringbuffer	*append(double number);
		stringbuffer	*append(double number, uint16_t scale);
		stringbuffer	*append(double number, uint16_t precision,
							uint16_t scale);
			// Appends the data to the stringbuffer.  The buffer
			// will grow to accommodate the new data.

		void	clear();
			// Empties the stringbuffer.

		const char	*getString();
				// Returns the string currently stored in the
				// stringbuffer.

		char	*detachString();
			// Returns a pointer to the string currently stored
			// in the stringbuffer, then resets the stringbuffer
			// such that it will no longer use that string.
			//
			// The calling program must deallocate the string
			// returned from this method.

		size_t	getStringLength();
			// Returns the length of the string currently stored
			// in the stringbuffer.

		size_t	getPosition();
			// Returns the position in the buffer at which
			// the next write will occur.

	#include <rudiments/private/stringbuffer.h>
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
