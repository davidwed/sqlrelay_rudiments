#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// attach to the shared memory segment keyed to the file "shm"
        sharedmemory    shm;
        if (!shm.attach(file::generateKey("shm",1),128)) {
		stdoutput.printf("error: %s\n",error::getErrorString());
		process::exit(1);
	}

	// display the data contained in the shared memory segment
        stdoutput.printf("%s\n",(char *)shm.getPointer());
}
