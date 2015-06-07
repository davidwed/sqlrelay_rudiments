#include <rudiments/commandline.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// initialize an instance of the commandline class
	commandline	cmdl(argc,argv);

	// determine the greeting
	const char	*greeting="hi";
	if (cmdl.found("hello")) {
		greeting="hello";
	}

	// get the name
	const char	*name=cmdl.getValue("name");

	// print the greeting
	stdoutput.printf("%s %s\n",greeting,name);

	process::exit(0);
}
