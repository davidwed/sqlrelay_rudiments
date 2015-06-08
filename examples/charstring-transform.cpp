#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	char	hellothere[]="   hello there!   ";

	// upper-case...
	charstring::upper(hellothere);
	stdoutput.printf("upper cased:   \"%s\"\n",hellothere);

	// lower-case...
	charstring::lower(hellothere);
	stdoutput.printf("lower cased:   \"%s\"\n",hellothere);

	// capitalized...
	charstring::capitalize(hellothere);
	stdoutput.printf("capitalized:   \"%s\"\n",hellothere);

	// right trimmed...
	charstring::rightTrim(hellothere);
	stdoutput.printf("right trimmed: \"%s\"\n",hellothere);

	// left trimmed...
	charstring::leftTrim(hellothere);
	stdoutput.printf("left trimmed:  \"%s\"\n",hellothere);
	stdoutput.write('\n');



	char	paragraph[]="Hello there.\n This is a paragraph\n "
				"with random\n carriage returns\n "
				"scattered throughout.";

	// original...
	stdoutput.printf("original text:\n%s\n\n",paragraph);

	// stripped of carraige returns...
	charstring::strip(paragraph,'\n');
	stdoutput.printf("text without carriage returns:\n%s\n\n",paragraph);

	// stripped of "Hello there."...
	charstring::strip(paragraph,"Hello there. ");
	stdoutput.printf("text without \"Hello There. \":\n%s\n\n",paragraph);

	// with replacements...
	charstring::replace(paragraph,' ','_');
	stdoutput.printf("text with spaces replaced by underscores:\n%s\n\n",
								paragraph);



	char	paddedtext[]="   hello   ";

	// original...
	stdoutput.printf("original text:   \"%s\"\n",paddedtext);

	// left-justified...
	charstring::leftJustify(paddedtext,charstring::length(paddedtext));
	stdoutput.printf("left-justified:  \"%s\"\n",paddedtext);

	// right-justified...
	charstring::rightJustify(paddedtext,charstring::length(paddedtext));
	stdoutput.printf("right-justified: \"%s\"\n",paddedtext);

	// centered...
	charstring::center(paddedtext,charstring::length(paddedtext));
	stdoutput.printf("centered:        \"%s\"\n",paddedtext);
	stdoutput.write('\n');



	const char	unpaddedtext[]="hellothere";

	// original...
	stdoutput.printf("original text:      \"%s\"\n",unpaddedtext);

	// left-padded
	char	*leftpadded=charstring::pad(unpaddedtext,' ',-1,15);
	stdoutput.printf("left padded text:   \"%s\"\n",leftpadded);
	delete[] leftpadded;

	// right-padded
	char	*rightpadded=charstring::pad(unpaddedtext,' ',1,15);
	stdoutput.printf("right padded text:  \"%s\"\n",rightpadded);
	delete[] rightpadded;

	// center-padded
	char	*centerpadded=charstring::pad(unpaddedtext,' ',0,15);
	stdoutput.printf("center padded text: \"%s\"\n",centerpadded);
	delete[] centerpadded;

	process::exit(0);
}
