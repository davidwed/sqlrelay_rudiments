// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <sys/types.h>
#include <sys/ipc.h>
#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <unistd.h>
#include <string.h>

int	main() {
        sharedmemory    shm;
        shm.create(ftok("/tmp/shm",0),128,
                                permissions::evalPermString("rw-------"));

        char    *shmptr=(char *)shm.getPointer();
        strcpy(shmptr,"This string is in shared memory.");

        sleep(1000);
}
