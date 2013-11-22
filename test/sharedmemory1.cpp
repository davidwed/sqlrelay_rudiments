// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/charstring.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/error.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a file called shm
	file::remove("shm");
	file	fd;
	fd.create("shm",permissions::evalPermString("rw-------"));
	fd.close();

	// create a shared memory segment, keyed to shm, 128 bytes long
        sharedmemory    shm;
        if (!shm.create(file::generateKey("shm",1),128,
                                permissions::evalPermString("rw-------"))) {
		stdoutput.printf("error: %s\n",error::getErrorString());
		process::exit(1);
	}

	// write a string into the shared memory
        char    *shmptr=(char *)shm.getPointer();
        charstring::copy(shmptr,"This string is in shared memory.");

	// sleep while another process accesses the shared memory...
        snooze::macrosnooze(1000);
}
