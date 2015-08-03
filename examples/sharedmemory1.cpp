#include <rudiments/sharedmemory.h>
#include <rudiments/charstring.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/error.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a file called "shm"
	file::createFile("shm",permissions::evalPermString("rw-------"));

	// create a 128 byte shared memory segment, keyed to the file "shm"
        sharedmemory    shm;
        if (!shm.create(file::generateKey("shm",1),128,
                                permissions::evalPermString("rw-------"))) {
		stdoutput.printf("error: %s\n",error::getErrorString());
		process::exit(1);
	}

	// write a string into the shared memory
        char    *shmptr=(char *)shm.getPointer();
        charstring::copy(shmptr,"This string is in shared memory.");

	// sleep for 10 seconds, giving another process some time to access
	// the shared memory...
        snooze::macrosnooze(10);

	// remove the file "shm"
	file::remove("shm");
}
