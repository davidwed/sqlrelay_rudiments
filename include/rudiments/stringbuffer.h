// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

#include <rudiments/private/config.h>

// The stringbuffer class can be used to store strings of arbitrary length.

class stringbuffer {
	public:
			stringbuffer();
			~stringbuffer();

		// The append() methods return a pointer to the stringbuffer
		// instance.  This enables append chaining:
		// sb->append("numbers: ")->append(5)->append(5.5);
		stringbuffer	*append(const char *string);
			// Appends "string" to the stringbuffer.
		stringbuffer	*append(char character);
			// Appends "character" to the stringbuffer.
		stringbuffer	*append(long number);
			// Appends "number" to the stringbuffer.
		stringbuffer	*append(double number);
			// Appends "number" to the stringbuffer.
		stringbuffer	*append(double number,
						unsigned short precision,
						unsigned short scale);
			// Appends "number" to the stringbuffer using the
			// supplied "precision" and "scale".

		void	clear();
			// Empties the stringbuffer.

		char	*getString();
			// Returns the current string stored in the
			// stringbuffer.

	#include <rudiments/private/stringbuffer.h>
};

#ifdef ENABLE_INLINES
	#include <rudiments/private/stringbufferinlines.h>
#endif

#endif
