// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/character.h>

#include <ctype.h>

#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
namespace rudiments {
#endif

bool character::isAlphanumeric(int c) {
	return isalnum(c)!=0;
}

bool character::isAlphabetical(int c) {
	return isalpha(c)!=0;
}

bool character::isControlCharacter(int c) {
	return iscntrl(c)!=0;
}

bool character::isDigit(int c) {
	return isdigit(c)!=0;
}

bool character::isLowerCase(int c) {
	return islower(c)!=0;
}

bool character::isPrintableNonSpace(int c) {
	return isgraph(c)!=0;
}

bool character::isPrintable(int c) {
	return isprint(c)!=0;
}

bool character::isPunctuation(int c) {
	return ispunct(c)!=0;
}

bool character::isUpperCase(int c) {
	return isupper(c)!=0;
}

bool character::isHexDigit(int c) {
	return isxdigit(c)!=0;
}

bool character::isBlank(int c) {
	// don't be tempted to use isblank() here, it's too challenging to get
	// it to work on every platform without a bunch of confusing #defines
	// before including ctype.h
	return (c==' ' || c=='	');
}

bool character::isWhitespace(int c) {
	return isspace(c)!=0;
}

bool character::isAscii(int c) {
	return isascii(c)!=0;
}


int character::toUpperCase(int c) {
	return toupper(c);
}

int character::toLowerCase(int c) {
	return tolower(c);
}

int character::toAscii(int c) {
	return toascii(c);
}

void character::safePrint(char c) {
	if (c=='\r') {
		printf("\\r");
	} else if (c=='\n') {
		printf("\\n");
	} else if (c=='	') {
		printf("\\t");
	} else if (c>=' ' && c<='~') {
		printf("%c",c);
	} else {
		printf("(0x%02x)",(unsigned int)c);
	}
}

#ifdef RUDIMENTS_NAMESPACE
}
#endif
