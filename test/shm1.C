// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <sys/types.h>
#include <sys/ipc.h>
#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <unistd.h>
#include <string.h>

int main(int argv, const char **argc) {

	// create a file called /tmp/shm
	unlink("/tmp/shm");
	file	fd;
	fd.create("/tmp/shm",permissions::evalPermString("rw-------"));
	fd.close();

	// create a shared memory segment, keyed to /tmp/shm, 128 bytes long
        sharedmemory    shm;
        shm.create(ftok("/tmp/shm",0),128,
                                permissions::evalPermString("rw-------"));

	// write a string into the shared memory
        char    *shmptr=(char *)shm.getPointer();
        strcpy(shmptr,"This string is in shared memory.");

	// sleep while another process accesses the shared memory...
        sleep(1000);
}
