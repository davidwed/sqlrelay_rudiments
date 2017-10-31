#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

void header(const char *title) {
	stdoutput.printf("\n===============================================================================\n");
	stdoutput.printf("\n%s:\n\n",title);
}

void test(const char *printstring, bool result) {
	stdoutput.write("\t");
	stdoutput.write(printstring);
	stdoutput.write((result)?": success":": failed");
	stdoutput.write("\n");
	if (!result) {
		process::exit(1);
	}
}
