#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	stdoutput.write("exiting with status 0 (success)\n");

	process::exit(0);
}
