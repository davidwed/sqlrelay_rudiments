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

class DLLSPEC character {
	public:
		static bool	isAlphanumeric(int32_t c);
		static bool	isAlphabetical(int32_t c);

		static bool	isLowerCase(int32_t c);
		static bool	isUpperCase(int32_t c);

		static bool	isPunctuation(int32_t c);

		static bool	isPrintable(int32_t c);
		static bool	isPrintableNonSpace(int32_t c);

		static bool	isControlCharacter(int32_t c);

		static bool	isDigit(int32_t c);
		static bool	isHexDigit(int32_t c);

		static bool	isBlank(int32_t c);
		static bool	isWhitespace(int32_t c);

		static bool	isAscii(int32_t c);

		static int32_t	toUpperCase(int32_t c);
		static int32_t	toLowerCase(int32_t c);
		static int32_t	toAscii(int32_t c);

		static bool	inSet(char c, const char *set);

		static void	safePrint(char c);
};

#ifdef RUDIMENTS_NAMESPACE
}
#endif

#endif
