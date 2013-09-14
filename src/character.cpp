// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#include <rudiments/character.h>

#ifdef RUDIMENTS_HAVE_CTYPE_H
	#include <ctype.h>
#endif

#include <stdio.h>

bool character::isAlphanumeric(int32_t c) {
	return isalnum(c)!=0;
}

bool character::isAlphabetical(int32_t c) {
	return isalpha(c)!=0;
}

bool character::isControlCharacter(int32_t c) {
	return iscntrl(c)!=0;
}

bool character::isDigit(int32_t c) {
	return isdigit(c)!=0;
}

bool character::isLowerCase(int32_t c) {
	return islower(c)!=0;
}

bool character::isPrintableNonSpace(int32_t c) {
	return isgraph(c)!=0;
}

bool character::isPrintable(int32_t c) {
	return isprint(c)!=0;
}

bool character::isPunctuation(int32_t c) {
	return ispunct(c)!=0;
}

bool character::isUpperCase(int32_t c) {
	return isupper(c)!=0;
}

bool character::isHexDigit(int32_t c) {
	return isxdigit(c)!=0;
}

bool character::isBlank(int32_t c) {
	// don't be tempted to use isblank() here, it's too challenging to get
	// it to work on every platform without a bunch of confusing #defines
	// before including ctype.h
	return (c==' ' || c=='	');
}

bool character::isWhitespace(int32_t c) {
	return isspace(c)!=0;
}

bool character::isAscii(int32_t c) {
	return isascii(c)!=0;
}


int32_t character::toUpperCase(int32_t c) {
	return toupper(c);
}

int32_t character::toLowerCase(int32_t c) {
	return tolower(c);
}

int32_t character::toAscii(int32_t c) {
	return toascii(c);
}

bool character::inSet(char c, const char *set) {
	for (uint16_t i=0; set[i]; i++) {
		if (set[i]==c) {
			return true;
		}
	}
	return false;
}
