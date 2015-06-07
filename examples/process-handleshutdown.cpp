#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

// define a function to run when shut down occurs
void shutDown(int32_t signal) {
	stdoutput.printf("shutting down gracefully...\n");
	process::exit(1);
}

int main(int argc, const char **argv) {

	// configure the process to call shutDown on shut down
	process::handleShutDown(shutDown);

	stdoutput.write("kill the process or press ctrl-C to exit\n");
	stdoutput.write("(should not display \"Terminated\")\n");

	// loop forever, waiting for ctrl-C or to be killed
	for (;;) { 
		snooze::macrosnooze(1);
	}
}
