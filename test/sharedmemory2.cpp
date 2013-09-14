// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/error.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// attach to a shared memory segment keyed to /tmp/shm
        sharedmemory    shm;
        if (!shm.attach(file::generateKey("/tmp/shm",1))) {
		stdoutput.printf("error: %s\n",error::getErrorString());
		process::exit(1);
	}

	// display the data contained in the shared memory segment
        char    *shmptr=(char *)shm.getPointer();
        stdoutput.printf("%s\n",shmptr);
}
