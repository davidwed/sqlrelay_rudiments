// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <stdio.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	// attach to a shared memory segment keyed to /tmp/shm
        sharedmemory    shm;
        shm.attach(file::generateKey("/tmp/shm",1));

	// display the data contained in the shared memory segment
        char    *shmptr=(char *)shm.getPointer();
        printf("%s\n",shmptr);
}
