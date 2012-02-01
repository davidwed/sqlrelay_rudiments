// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/snooze.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// create a file called /tmp/shm
	file::remove("/tmp/shm");
	file	fd;
	fd.create("/tmp/shm",permissions::evalPermString("rw-------"));
	fd.close();

	// create a shared memory segment, keyed to /tmp/shm, 128 bytes long
        sharedmemory    shm;
        shm.create(file::generateKey("/tmp/shm",1),128,
                                permissions::evalPermString("rw-------"));

	// write a string into the shared memory
        char    *shmptr=(char *)shm.getPointer();
        strcpy(shmptr,"This string is in shared memory.");

	// sleep while another process accesses the shared memory...
        snooze::macrosnooze(1000);
}
