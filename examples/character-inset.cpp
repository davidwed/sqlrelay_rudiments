#include <rudiments/character.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// define the set
	const char	set[]="abcdefg";

	// loop through the alphabet...
	for (char c='a'; c<='z'; c++) {
		stdoutput.printf("is %c is in the set \"%s\"?  %s\n",
				c,set,
				(character::inSet(c,set))?"yes":"no");
	}
}
