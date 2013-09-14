#include <rudiments/stdio.h>
#include <rudiments/process.h>

void test(const char *printstring, bool result) {
	stdoutput.printf("\t");
	stdoutput.printf(printstring);
	stdoutput.printf((result)?": success":": failed");
	stdoutput.printf("\n");
	if (!result) {
		process::exit(0);
	}
}
