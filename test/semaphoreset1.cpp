// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/semaphoreset.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a file called /tmp/sem
	file::remove("/tmp/sem");
	file	fd;
	fd.create("/tmp/sem",permissions::evalPermString("rw-------"));
	fd.close();

	// Create a semaphore set keyed to /tmp/sem containing 2 individual
	// semaphores.  Initialize them to 0 and 1 respectively.
        int     vals[2]={0,1};
        semaphoreset	sem;
        sem.create(file::generateKey("/tmp/sem",1),
			permissions::evalPermString("rw-------"),2,vals);

	// loop 10 times, printing 2 and 4, synchronizing with another process
	// using the semaphores.
        for (int i=0; i<10; i++) {
                sem.wait(0);
                stdoutput.printf("2\n");
                sem.signal(1);
        
                sem.wait(0);
                stdoutput.printf("4\n");
                sem.signal(1);
        }
}
