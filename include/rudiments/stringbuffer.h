// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STRINGBUFFER_H
#define RUDIMENTS_STRINGBUFFER_H

#include <rudiments/private/config.h>
#include <rudiments/variablebuffer.h>

#include <sys/types.h>

// The stringbuffer class can be used to store strings of arbitrary length.

class stringbuffer : public variablebuffer {
	public:
			stringbuffer();
			// Creates a new buffer which will grow as necessary
			// to accomodate the string written to it.
		virtual	~stringbuffer();

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
		stringbuffer	*write(const char *string);
		stringbuffer	*write(char character);
		stringbuffer	*write(long number);
		stringbuffer	*write(double number);
		stringbuffer	*write(double number,
						unsigned short precision,
						unsigned short scale);
			// Writes the data to the stringbuffer at the current
			// position.  If necessary, the buffer will grow to
			// accommodate the new data.
		stringbuffer	*append(const char *string);
		stringbuffer	*append(char character);
		stringbuffer	*append(long number);
		stringbuffer	*append(double number);
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

	#include <rudiments/private/stringbuffer.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/stringbufferinlines.h>
#endif

#endif
