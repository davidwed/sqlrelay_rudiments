// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/inetclientsocket.h>

#include <stdio.h>

#include <time.h>

int	main() {

	char			buffer[11];
	int			sizeread;
	inetclientsocket	*clnt=new inetclientsocket();

	time_t	starttime=time(NULL);

	for (int i=0; i<100000; i++) {
		clnt->connectToServer("localhost",10000,0,1);
		sizeread=clnt->read(buffer,5);
		buffer[sizeread]=(char)NULL;
		clnt->close();
	}

	printf("total real time: %d\n",time(NULL)-starttime);

	delete clnt;
}
