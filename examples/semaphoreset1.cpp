#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/semaphoreset.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a file named "sem"
	file::createFile("sem",permissions::evalPermString("rw-------"));

	// generate a unique semaphore id based on the file
	key_t	semid=file::generateKey("sem",1);

	// Create a semaphore set, containing 2 semaphores.
	// Initialize them to 0 and 1 respectively.
	int32_t	vals[2]={0,1};
	semaphoreset	sem;
	if (sem.create(semid,permissions::evalPermString("rw-------"),2,vals)) {

		// loop 10 times, printing 2 and 4, using the
		// semaphores to synchronize with another process
		for (int i=0; i<10; i++) {
	
			sem.wait(0);
			stdoutput.printf("2\n");
			sem.signal(1);
	
			sem.wait(0);
			stdoutput.printf("4\n");
			sem.signal(1);
		}

	} else {

		// an error must have occurred
		char	*err=error::getErrorString();
		stdoutput.printf("error: %s\n",err);
		delete[] err;
	}

	// remove the file "sem"
	file::remove("sem");
}
