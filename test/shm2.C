// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <sys/types.h>
#include <sys/ipc.h>
#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <stdio.h>

int main(int argc, const char **argv) {

	// attach to a shared memory segment keyed to /tmp/shm
        sharedmemory    shm;
        shm.attach(ftok("/tmp/shm",0));

	// display the data contained in the shared memory segment
        char    *shmptr=(char *)shm.getPointer();
        printf("%s\n",shmptr);
}
