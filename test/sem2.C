// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <rudiments/semaphoreset.h>
#include <rudiments/permissions.h>
#include <stdio.h>
#include <unistd.h>

int	main() {

        semaphoreset *sem=new semaphoreset();
	sem->attach(ftok("/tmp/sem",0),2);

        for (int i=0; i<10; i++) {
                sem->wait(1);
                printf("1\n");
                sem->signal(0);
        
                sem->wait(1);
                printf("3\n");
                sem->signal(0);
        }

	delete sem;

	unlink("/tmp/sem");
}
