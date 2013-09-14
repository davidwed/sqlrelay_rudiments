// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// attach to the semaphore set keyed to /tmp/sem containing 2 semaphores
        semaphoreset	sem;
	sem.attach(file::generateKey("/tmp/sem",1),2);

	// loop 10 times, printing 1 and 2, synchronizing with another process
	// using the semaphores
        for (int i=0; i<10; i++) {
                sem.wait(1);
                stdoutput.printf("1\n");
                sem.signal(0);
        
                sem.wait(1);
                stdoutput.printf("3\n");
                sem.signal(0);
        }

	file::remove("/tmp/sem");
}
