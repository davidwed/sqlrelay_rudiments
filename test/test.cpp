#include <rudiments/stdio.h>
#include <rudiments/process.h>

void header(const char *title) {
	stdoutput.printf("\n===============================================================================\n");
	stdoutput.printf("\n%s:\n\n",title);
}

void test(const char *printstring, bool result) {
	stdoutput.printf("\t");
	stdoutput.printf(printstring);
	stdoutput.printf((result)?": success":": failed");
	stdoutput.printf("\n");
	if (!result) {
		process::exit(0);
	}
}
