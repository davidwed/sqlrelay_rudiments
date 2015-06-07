#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// configure the process to exit gracefully on crash
	process::exitOnCrash();

	stdoutput.write("exiting instead of \"Segmentation Fault\"\n");

	// this will crash...
	void	(*f)(void)=0;
	f();
}
