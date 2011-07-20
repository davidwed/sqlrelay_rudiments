// Copyright (c) 2003  David Muse
// See the file COPYING for more information

#include <rudiments/character.h>
#include "test.C"

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	bool	isalphanumeric[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,false,false,false,false,false,false,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,false,false,false,false,false
	};

	bool	isalphabetical[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,false,false,false,false,false,false,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,false,false,false,false,false
	};

	bool	islowercase[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,false,false,false,false,false
	};

	bool	isuppercase[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	ispunctuation[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,true,true,true,true,true,true,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,true,true,true,true,false
	};

	bool	isprintable[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,false
	};

	bool	isprintablenonspace[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,false
	};

	bool	iscontrolchar[]={
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,true
	};

	bool	isdigit[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	ishexdigit[]={
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,true,true,
		true,true,true,true,true,true,true,true,false,false,
		false,false,false,false,false,true,true,true,true,true,
		true,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,true,true,true,
		true,true,true,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	isblank[]={
		false,false,false,false,false,false,false,false,false,true,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,true,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	iswhitespace[]={
		false,false,false,false,false,false,false,false,false,true,
		true,true,true,true,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,true,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false,false,false,
		false,false,false,false,false,false,false,false
	};

	bool	isascii[]={
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true,true,true,
		true,true,true,true,true,true,true,true
	};

	char	touppercase[]={
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52,53,54,55,56,57,58,59,
		60,61,62,63,64,65,66,67,68,69,
		70,71,72,73,74,75,76,77,78,79,
		80,81,82,83,84,85,86,87,88,89,
		90,91,92,93,94,95,96,'A','B','C',
		'D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W',
		'X','Y','Z',123,124,125,126,127
	};

	char	tolowercase[]={
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52,53,54,55,56,57,58,59,
		60,61,62,63,64,'a','b','c','d','e',
		'f','g','h','i','j','k','l','m','n','o',
		'p','q','r','s','t','u','v','w','x','y',
		'z',91,92,93,94,95,96,97,98,99,
		100,101,102,103,104,105,106,107,108,109,
		110,111,112,113,114,115,116,117,118,119,
		120,121,122,123,124,125,126,127
	};

	char	toascii[]={
		0,1,2,3,4,5,6,7,8,9,
		10,11,12,13,14,15,16,17,18,19,
		20,21,22,23,24,25,26,27,28,29,
		30,31,32,33,34,35,36,37,38,39,
		40,41,42,43,44,45,46,47,48,49,
		50,51,52,53,54,55,56,57,58,59,
		60,61,62,63,64,65,66,67,68,69,
		70,71,72,73,74,75,76,77,78,79,
		80,81,82,83,84,85,86,87,88,89,
		90,91,92,93,94,95,96,97,98,99,
		100,101,102,103,104,105,106,107,108,109,
		110,111,112,113,114,115,116,117,118,119,
		120,121,122,123,124,125,126,127
	};

	printf("character:\n");

	for (unsigned char c=0; c<128; c++) {

		printf("\tsafePrint: ");
		character::safePrint(c);
		printf("\n");

		test("\tisAlphanumeric",
			character::isAlphanumeric(c)==isalphanumeric[c]);
		test("\tisAlphabetical",
			character::isAlphabetical(c)==isalphabetical[c]);
		test("\tisLowerCase",
			character::isLowerCase(c)==islowercase[c]);
		test("\tisUpperCase",
			character::isUpperCase(c)==isuppercase[c]);
		test("\tisPunctuation",
			character::isPunctuation(c)==ispunctuation[c]);
		test("\tisPrintable",
			character::isPrintable(c)==isprintable[c]);
		test("\tisPrintableNonSpace",
			character::isPrintableNonSpace(c)==
					isprintablenonspace[c]);
		test("\tisControlCharacter",
			character::isControlCharacter(c)==iscontrolchar[c]);
		test("\tisDigit",
			character::isDigit(c)==isdigit[c]);
		test("\tisHexDigit",
			character::isHexDigit(c)==ishexdigit[c]);
		test("\tisBlank",
			character::isBlank(c)==isblank[c]);
		test("\tisWhitespace",
			character::isWhitespace(c)==iswhitespace[c]);
		test("\tisAscii",
			character::isAscii(c)==isascii[c]);
		test("\ttoUpperCase",
			character::toUpperCase(c)==touppercase[c]);
		test("\ttoLowerCase",
			character::toLowerCase(c)==tolowercase[c]);
		test("\ttoAscii",
			character::toAscii(c)==toascii[c]);
	}
}
