#include <rudiments/character.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// print information about all ascii characters
	for (unsigned char uc=0; uc<128; uc++) {

		char	c=(char)uc;
		
		stdoutput.write("character ");
		stdoutput.safePrint(c);
		stdoutput.write(":\n");

		stdoutput.printf("	is alphanumeric		: %s\n",
				(character::isAlphanumeric(c))?"yes":"no");
		stdoutput.printf("	is alphabetical		: %s\n",
				(character::isAlphabetical(c))?"yes":"no");
		stdoutput.printf("	is lower case		: %s\n",
				(character::isLowerCase(c))?"yes":"no");
		stdoutput.printf("	is upper case		: %s\n",
				(character::isUpperCase(c))?"yes":"no");
		stdoutput.printf("	is punctuation		: %s\n",
				(character::isPunctuation(c))?"yes":"no");
		stdoutput.printf("	is printable		: %s\n",
				(character::isPrintable(c))?"yes":"no");
		stdoutput.printf("	is printable (non-space): %s\n",
				(character::isPrintableNonSpace(c))?"yes":"no");
		stdoutput.printf("	is control chraracter	: %s\n",
				(character::isControlCharacter(c))?"yes":"no");
		stdoutput.printf("	is digit		: %s\n",
				(character::isDigit(c))?"yes":"no");
		stdoutput.printf("	is hex digit		: %s\n",
				(character::isHexDigit(c))?"yes":"no");
		stdoutput.printf("	is blank		: %s\n",
				(character::isBlank(c))?"yes":"no");
		stdoutput.printf("	is whitespace		: %s\n",
				(character::isWhitespace(c))?"yes":"no");
		stdoutput.printf("	is ascii		: %s\n",
				(character::isAscii(c))?"yes":"no");
	}

	process::exit(0);
}
