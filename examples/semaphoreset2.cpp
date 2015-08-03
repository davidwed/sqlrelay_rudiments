#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/semaphoreset.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// generate a semaphore id based on the file "sem"
	// (which was created by the other process)
	key_t	semid=file::generateKey("sem",1);

	// attach to the semaphore set keyed to sem containing 2 semaphores
	semaphoreset	sem;
	if (sem.attach(semid,2)) {

		// loop 10 times, printing 1 and 3, using the
		// semaphores to synchronize with another process
		for (int i=0; i<10; i++) {

			sem.wait(1);
			stdoutput.printf("1\n");
			sem.signal(0);

			sem.wait(1);
			stdoutput.printf("3\n");
			sem.signal(0);
		}

	} else {

		// an error must have occurred
		char	*err=error::getErrorString();
		stdoutput.printf("error: %s\n",err);
		delete[] err;
	}
}
