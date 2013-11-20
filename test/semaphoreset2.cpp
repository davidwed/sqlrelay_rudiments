// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

void test() {

	// attach to the semaphore set keyed to sem containing 2 semaphores
	semaphoreset	sem;
	if (!sem.attach(file::generateKey("sem",1),2)) {
		stdoutput.printf("semaphore attach failed\n");
		process::exit(1);
	}

	// loop 10 times, printing 1 and 2, synchronizing with another process
	// using the semaphores
	for (int i=0; i<10; i++) {
		if (!sem.wait(1)) {
			stdoutput.printf("wait(1) (1) failed\n");
			process::exit(1);
		}
		stdoutput.printf("1\n");
		if (!sem.signal(0)) {
			stdoutput.printf("signal(0) (1) failed\n");
			process::exit(1);
		}
	
		if (!sem.wait(1)) {
			stdoutput.printf("wait(1) (2) failed\n");
			process::exit(1);
		}
		stdoutput.printf("3\n");
		if (!sem.signal(0)) {
			stdoutput.printf("signal(0) (2) failed\n");
			process::exit(1);
		}
	}

	file::remove("sem");
}

int main(int argc, const char **argv) {
	test();
	process::exit(0);
}
