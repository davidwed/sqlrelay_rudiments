// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <rudiments/semaphoreset.h>
#include <rudiments/permissions.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int	main() {

	unlink("/tmp/sem");
	int	fd=open("/tmp/sem",O_CREAT|O_TRUNC|O_RDWR,
			permissions::evalPermString("rw-------"));
	close(fd);

        int     vals[2]={0,1};
        semaphoreset	sem;
        sem.create(ftok("/tmp/sem",0),
			permissions::evalPermString("rw-------"),2,vals);

        for (int i=0; i<10; i++) {
                sem.wait(0);
                printf("2\n");
                sem.signal(1);
        
                sem.wait(0);
                printf("4\n");
                sem.signal(1);
        }
}
