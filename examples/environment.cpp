#include <rudiments/environment.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// Print the PATH environment variable.
	stdoutput.printf("PATH=%s\n\n",environment::getValue("PATH"));


	// Set TEST=testvalue and print the TEST.
	environment::setValue("TEST","testvalue");
	stdoutput.printf("TEST=%s\n\n",environment::getValue("TEST"));


	// Remove TEST and print it.
	environment::remove("TEST");
	stdoutput.printf("TEST=%s\n\n",environment::getValue("TEST"));


	// Print all environment variable/value pairs.
	stdoutput.write("All Variables:\n");
	for (const char * const *v=environment::variables(); v && *v; v++) {
		stdoutput.printf("%s\n",*v);
	}
	stdoutput.write('\n');


	// Clear the environment and atttempt to print all
	// environment variable/value pairs.  Should print nothing.
	environment::clear();
	stdoutput.write("All Variables (should be empty now):\n");
	for (const char * const *v=environment::variables(); v && *v; v++) {
		stdoutput.printf("%s\n",*v);
	}
	stdoutput.write('\n');

	process::exit(0);
}
