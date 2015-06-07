#include <rudiments/snooze.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// declare some variables that we'll use later
	uint32_t	remainingseconds;
	uint32_t	remainingmicroseconds;
	uint32_t	remainingnanoseconds;


	// snooze for 2 seconds, auto-recovering from interruptions
	stdoutput.printf("snoozing for 2 seconds...\n");
	snooze::macrosnooze(2);

	// snooze for 2 seconds, allowing interruptions
	stdoutput.printf("snoozing for 2 seconds (interruptable)...\n");
	if (!snooze::macrosnooze(2,&remainingseconds)) {
		stdoutput.printf("interrupted... %d seconds remained\n",
							remainingseconds);
	}


	// snooze for 200 microseconds, auto-recovering from interruptions
	stdoutput.printf("snoozing for 200 microseconds...\n");
	snooze::microsnooze(0,200);

	// snooze for 200 microseconds, allowing interruptions
	stdoutput.printf("snoozing for 200 microseconds (interruptable)...\n");
	if (!snooze::microsnooze(0,200,
				&remainingseconds,
				&remainingmicroseconds)) {
		stdoutput.printf("interrupted... %d seconds and "
					"%d microseconds remained\n",
							remainingseconds,
							remainingmicroseconds);
	}


	// snooze for 200 nanoseconds, auto-recovering from interruptions
	stdoutput.printf("snoozing for 200 nanoseconds...\n");
	snooze::nanosnooze(0,200);

	// snooze for 200 nanoseconds, allowing interruptions
	stdoutput.printf("snoozing for 200 nanoseconds (interruptable)...\n");
	if (!snooze::nanosnooze(0,200,
				&remainingseconds,
				&remainingnanoseconds)) {
		stdoutput.printf("interrupted... %d seconds and "
					"%d nanoseconds remained\n",
							remainingseconds,
							remainingnanoseconds);
	}
}
