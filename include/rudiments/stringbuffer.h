// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STRINGBUFFER_H
#define RUDIMENTS_STRINGBUFFER_H

#include <rudiments/private/stringbufferincludes.h>

// The stringbuffer class can be used to store strings of arbitrary length.

class stringbuffer : public variablebuffer {
	public:
			stringbuffer();
			stringbuffer(char *initialcontents,
					size_t initialsize,
					size_t increment);
			// Creates a new buffer which will grow as necessary
			// to accomodate the string written to it.
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
		stringbuffer	*write(short number);
		stringbuffer	*write(long number);
		stringbuffer	*write(long long number);
		stringbuffer	*write(unsigned char number);
		stringbuffer	*write(unsigned short number);
		stringbuffer	*write(unsigned long number);
		stringbuffer	*write(unsigned long long number);
		stringbuffer	*write(float number);
		stringbuffer	*write(float number,
						unsigned short scale);
		stringbuffer	*write(float number,
						unsigned short precision,
						unsigned short scale);
		stringbuffer	*write(double number);
		stringbuffer	*write(double number,
						unsigned short scale);
		stringbuffer	*write(double number,
						unsigned short precision,
						unsigned short scale);
			// Writes the data to the stringbuffer at the current
			// position.  If necessary, the buffer will grow to
			// accommodate the new data.
		stringbuffer	*append(const unsigned char *string);
		stringbuffer	*append(const unsigned char *string,
							size_t size);
		stringbuffer	*append(const char *string);
		stringbuffer	*append(const char *string, size_t size);
		stringbuffer	*append(char character);
		stringbuffer	*append(short character);
		stringbuffer	*append(long number);
		stringbuffer	*append(long long number);
		stringbuffer	*append(unsigned char number);
		stringbuffer	*append(unsigned short number);
		stringbuffer	*append(unsigned long number);
		stringbuffer	*append(unsigned long long number);
		stringbuffer	*append(short number,
					unsigned short zeropadding);
		stringbuffer	*append(long number,
					unsigned short zeropadding);
		stringbuffer	*append(long long number,
					unsigned short zeropadding);
		stringbuffer	*append(unsigned short number,
					unsigned short zeropadding);
		stringbuffer	*append(unsigned long number,
					unsigned short zeropadding);
		stringbuffer	*append(unsigned long long number,
					unsigned short zeropadding);
		stringbuffer	*append(float number);
		stringbuffer	*append(float number,
						unsigned short scale);
		stringbuffer	*append(float number,
						unsigned short precision,
						unsigned short scale);
		stringbuffer	*append(double number);
		stringbuffer	*append(double number,
						unsigned short scale);
		stringbuffer	*append(double number,
						unsigned short precision,
						unsigned short scale);
			// Appends the data to the stringbuffer.  The buffer
			// will grow to accommodate the new data.

		void	clear();
			// Empties the stringbuffer.

		char	*getString();
			// Returns the string currently stored in the
			// stringbuffer.

		size_t	getStringLength();
			// Returns the length of the string currently stored
			// in the stringbuffer.

		size_t	getPosition();
			// Returns the position in the buffer at which
			// the next write will occur.

		char	*detachString();
			// Returns a pointer to the string currently stored
			// in the stringbuffer, then resets the stringbuffer
			// such that it will no longer use that string.
			//
			// The calling program must deallocate the string
			// returned from this method.

	#include <rudiments/private/stringbuffer.h>
};

#endif
