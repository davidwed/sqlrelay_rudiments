#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// configure the process to exit gracefully on shut down
	process::exitOnShutDown();

	stdoutput.write("kill the process or press ctrl-C to exit\n");
	stdoutput.write("(should not display \"Terminated\")\n");

	// loop forever, waiting for ctrl-C or to be killed
	for (;;) { 
		snooze::macrosnooze(1);
	}
}
