#include <rudiments/process.h>
#include <rudiments/stdio.h>

// define a function to run when a crash occurs
void shutDown(int32_t signal) {
	stdoutput.printf("shutting down gracefully...\n");
	process::exit(1);
}

int main(int argc, const char **argv) {

	// configure the process to call shutDown on crash
	process::handleCrash(shutDown);

	stdoutput.write("handling crash, instead of \"Segmentation Fault\"\n");

	// this will crash...
	void	(*f)(void)=0;
	f();
}
