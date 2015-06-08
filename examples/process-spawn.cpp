#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	stdoutput.write("running \"ls -l /\"...\n");

	// spawn the comand:  ls -l /
	const char * const arguments[]={"ls","-l","/",NULL};
	process::spawn("ls",arguments,false);

	process::exit(0);
}
