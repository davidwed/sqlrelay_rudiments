// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/character.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	for (unsigned char c=0; c<128; c++) {

		printf("Character: \"%c\" (%d)\n",c,c);

		printf("\tisAlphanumeric: %d\n",
				character::isAlphanumeric(c));
		printf("\tisAlphabetical: %d\n",
				character::isAlphabetical(c));
		printf("\tisLowerCase: %d\n",
				character::isLowerCase(c));
		printf("\tisUpperCase: %d\n",
				character::isUpperCase(c));
		printf("\tisPunctuation: %d\n",
				character::isPunctuation(c));
		printf("\tisPrintable: %d\n",
				character::isPrintable(c));
		printf("\tisPrintableNonSpace: %d\n",
				character::isPrintableNonSpace(c));
		printf("\tisControlCharacter: %d\n",
				character::isControlCharacter(c));
		printf("\tisDigit: %d\n",
				character::isDigit(c));
		printf("\tisHexDigit: %d\n",
				character::isHexDigit(c));
		printf("\tisSpace: %d\n",
				character::isSpace(c));
		printf("\tisBlank: %d\n",
				character::isBlank(c));
		printf("\tisWhitespace: %d\n",
				character::isWhitespace(c));
		printf("\tisAscii: %d\n",
				character::isAscii(c));
		printf("\ttoUpperCase: %c\n",
				character::toUpperCase(c));
		printf("\ttoLowerCase: %c\n",
				character::toLowerCase(c));
		printf("\ttoAscii: %c\n",
				character::toAscii(c));
		printf("\tsafePrint: ");
		character::safePrint(c);
		printf("\n");
	}
}
