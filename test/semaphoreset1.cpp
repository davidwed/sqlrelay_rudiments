// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

void test() {

	// create a file called sem
	file::remove("sem");
	file	fd;
	if (!fd.create("sem",permissions::evalPermString("rw-------"))) {
		stdoutput.printf("key file create failed\n");
		process::exit(1);
	}
	fd.close();

	// Create a semaphore set keyed to sem containing 2 individual
	// semaphores.  Initialize them to 0 and 1 respectively.
	int     vals[2]={0,1};
	semaphoreset	sem;
	if (!sem.create(file::generateKey("sem",1),
			permissions::evalPermString("rw-------"),2,vals)) {
		stdoutput.printf("semaphore create failed\n");
		file::remove("sem");
		process::exit(1);
	}

	// loop 10 times, printing 2 and 4, synchronizing with another process
	// using the semaphores.
	for (int i=0; i<10; i++) {
		if (!sem.wait(0)) {
			stdoutput.printf("wait(0) (1) failed\n");
			file::remove("sem");
			process::exit(1);
		}
		stdoutput.printf("2\n");
		if (!sem.signal(1)) {
			stdoutput.printf("signal(1) (1) failed\n");
			file::remove("sem");
			process::exit(1);
		}

		if (!sem.wait(0)) {
			stdoutput.printf("wait(0) (2) failed\n");
			file::remove("sem");
			process::exit(1);
		}
		stdoutput.printf("4\n");
		if (!sem.signal(1)) {
			stdoutput.printf("signal(1) (2) failed\n");
			file::remove("sem");
			process::exit(1);
		}
	}
}

int main(int argc, const char **argv) {
	test();
	process::exit(0);
}
