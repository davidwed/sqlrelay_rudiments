#include <rudiments/regularexpression.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	const char	*string="hello there everyone";
	const char	*pattern="(\\w+) (\\w+)";

	regularexpression	re;
	re.compile(pattern);
	re.match(string);

	stdoutput.printf("\"%s\" matches \"%s\" %d times\n",
				string,pattern,re.getSubstringCount());

	for (int32_t i=0; i<re.getSubstringCount(); i++) {

		stdoutput.printf("  match %d starts at offset %2d: ",
					i,re.getSubstringStartOffset(i));
		stdoutput.printf("\"%s\"\n",re.getSubstringStart(i));

		stdoutput.printf("  match %d ends at offset %2d  : ",
					i,re.getSubstringEndOffset(i));
		stdoutput.printf("\"%s\"\n",re.getSubstringEnd(i));
	}
}
