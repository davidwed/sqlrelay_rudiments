// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <rudiments/sharedmemory.h>
#include <rudiments/permissions.h>
#include <stdio.h>

int	main() {
        sharedmemory    shm;
        shm.attach(ftok("/tmp/shm",0));

        char    *shmptr=(char *)shm.getPointer();
        printf("%s\n",shmptr);
}
