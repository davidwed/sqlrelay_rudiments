// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHARACTER_H
#define RUDIMENTS_CHARACTER_H

#include <rudiments/private/characterincludes.h>

// The character class provides methods for evaluating and manipulating
// characters.

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

class character {
	public:
		static bool	isAlphanumeric(int c);
		static bool	isAlphabetical(int c);

		static bool	isLowerCase(int c);
		static bool	isUpperCase(int c);

		static bool	isPunctuation(int c);

		static bool	isPrintable(int c);
		static bool	isPrintableNonSpace(int c);

		static bool	isControlCharacter(int c);

		static bool	isDigit(int c);
		static bool	isHexDigit(int c);

		static bool	isSpace(int c);
		static bool	isBlank(int c);
		static bool	isWhitespace(int c);

		static bool	isAscii(int c);

		static int	toUpperCase(int c);
		static int	toLowerCase(int c);
		static int	toAscii(int c);

		static void	safePrint(char c);
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
