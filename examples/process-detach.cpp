#include <rudiments/process.h>
#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// detach from the controlling terminal
	process::detach();

	// print "I'm running in the background 5 times",
	// snoozing 1 second between each line...
	for (uint16_t i=0; i<5; i++) {
		stdoutput.write("I'm running in the background...\n");
		snooze::macrosnooze(1);
	}

	process::exit(0);
}
