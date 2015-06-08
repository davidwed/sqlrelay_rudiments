// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHARACTER_H
#define RUDIMENTS_CHARACTER_H

#include <rudiments/private/characterincludes.h>

/** The character class provides static methods for evaluating and manipulating
 *  characters. */
class RUDIMENTS_DLLSPEC character {
	public:
		/** Returns true if "c" is an alphanumeric character
		 *  and false otherwise. */
		static bool	isAlphanumeric(int32_t c);

		/** Returns true if "c" is alphabetical character
		 *  and false otherwise. */
		static bool	isAlphabetical(int32_t c);

		/** Returns true if "c" is lower case and false otherwise. */
		static bool	isLowerCase(int32_t c);

		/** Returns true if "c" is upper case and false otherwise. */
		static bool	isUpperCase(int32_t c);

		/** Returns true if "c" is a printable character that is not
		 *  a space or alphanumeric character and false otherwise. */
		static bool	isPunctuation(int32_t c);

		/** Returns true if "c" is a printable character
		 *  and false otherwise. */
		static bool	isPrintable(int32_t c);

		/** Returns true if "c" is a printable character
		 *  other than a space and false otherwise. */
		static bool	isPrintableNonSpace(int32_t c);

		/** Returns true if "c" is a control character
		 *  and false otherwise. */
		static bool	isControlCharacter(int32_t c);

		/** Returns true if "c" is a digit and false otherwise. */
		static bool	isDigit(int32_t c);

		/** Returns true if "c" is a hexadecimal digit
		 *  and false otherwise. */
		static bool	isHexDigit(int32_t c);

		/** Returns true if "c" is a space or tab
		 *  and false otherwise. */
		static bool	isBlank(int32_t c);

		/** Returns true if "c" is any whitespace
		 *  character and false otherwise. */
		static bool	isWhitespace(int32_t c);

		/** Returns true if "c" is one of the values
		 *  in the ascii character set or false otherwise. */
		static bool	isAscii(int32_t c);

		/** Returns the upper case version of "c". */
		static int32_t	toUpperCase(int32_t c);

		/** Returns the lower case version of "c". */
		static int32_t	toLowerCase(int32_t c);

		/** Converts "c" to a 7-bit value by clearing
		 *  the higher order bits. */
		static int32_t	toAscii(int32_t c);

		/** Returns true if "c" is in the set of
		 *  characters in "set" and false otherwise. */
		static bool	inSet(char c, const char *set);
};

#endif
