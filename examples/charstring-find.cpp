#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// first/last instances of a character or string...
	const char	phrase[]="1 and 2 and 3 and 4";
	const char	numbers[]="1234";

	const char	*firsta=charstring::findFirst(phrase,'a');
	const char	*firstand=charstring::findFirst(phrase,"and");

	const char	*lasta=charstring::findLast(phrase,'a');
	const char	*lastand=charstring::findLast(phrase,"and");

	const char	*firstnum=charstring::findFirstOfSet(phrase,numbers);

	stdoutput.printf("in the phrase: \"%s\"...\n",phrase);
	stdoutput.printf("  the first 'a' is	: \"%s\"\n",firsta);
	stdoutput.printf("  the first \"and\" is	: \"%s\"\n",firstand);
	stdoutput.printf("  the last 'a' is	: \"%s\"\n",lasta);
	stdoutput.printf("  the last \"and\" is	: \"%s\"\n",lastand);
	stdoutput.printf("  the first number is	: \"%s\"\n",firstnum);
	stdoutput.write('\n');

	process::exit(0);
}
