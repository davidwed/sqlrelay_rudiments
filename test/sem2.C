// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <sys/types.h>
#include <sys/ipc.h>
#include <rudiments/semaphoreset.h>
#include <rudiments/permissions.h>
#include <stdio.h>
#include <unistd.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// attach to the semaphore set keyed to /tmp/sem containing 2 semaphores
        semaphoreset	sem;
	sem.attach(ftok("/tmp/sem",0),2);

	// loop 10 times, printing 1 and 2, synchronizing with another process
	// using the semaphores
        for (int i=0; i<10; i++) {
                sem.wait(1);
                printf("1\n");
                sem.signal(0);
        
                sem.wait(1);
                printf("3\n");
                sem.signal(0);
        }

	unlink("/tmp/sem");
}
