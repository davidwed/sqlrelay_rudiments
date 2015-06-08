#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// configure the process to "wait on child processes"
	process::waitForChildren();

	// five times...
	for (uint16_t i=0; i<5; i++) {

		// fork the process
		pid_t	pid=process::fork();

		if (pid) {
			// the parent process will run this code...
			stdoutput.printf("forked a child with pid: %d\n",pid);
		} else {
			// the child process will run this code...
			stdoutput.write("I'm the child, and I'm exiting...\n");
			process::exit(0);
		}

		// wait a second
		snooze::macrosnooze(1);
	}

	stdoutput.write('\n');
	stdoutput.write("check process list, there should be no zombies\n");
	stdoutput.write("kill the process or press ctrl-C to exit\n");

	// loop forever, waiting for ctrl-C or to be killed
	for (;;) {
		snooze::macrosnooze(1);
	}
}
