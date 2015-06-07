#include <rudiments/process.h>
#include <rudiments/stdio.h>

// define a function to run at exit
void runAtExit() {
	stdoutput.write("running at exit!\n");
}

int main(int argc, const char **argv) {

	// configure runAtExit to run at exit
	process::atExit(runAtExit);

	stdoutput.write("nothing should be printed below...\n");

	// exit immediately, bypassing functions configured to run at exit
	process::exitImmediately(0);
}
