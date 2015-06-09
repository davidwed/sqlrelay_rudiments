#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	const char	str[]="All along the untrodden paths of the future...";

	// split...
	char		**parts;
	uint64_t	partcount;
	charstring::split(str," ",true,&parts,&partcount);

	stdoutput.printf("original string:\n  %s\n",str);
	stdoutput.printf("split on space:\n");
	for (uint64_t i=0; i<partcount; i++) {
		stdoutput.printf("  %s\n",parts[i]);
	}
	stdoutput.write('\n');

	for (uint64_t i=0; i<partcount; i++) {
		delete[] parts[i];
	}
	delete[] parts;



	// substring...
	char	*substring1=charstring::subString(str,14);
	char	*substring2=charstring::subString(str,14,28);

	stdoutput.printf("string starting at index 14: %s\n",substring1);
	stdoutput.printf("string from index 14 to 21 : %s\n",substring2);
	stdoutput.write('\n');

	delete[] substring1;
	delete[] substring2;



	// insert string...
	char	*newstr=charstring::insertString(str,
			", I can see the footprints of an unseen hand",43);

	stdoutput.printf("string after insert:\n  %s\n",newstr);
	stdoutput.write('\n');

	delete[] newstr;
}
