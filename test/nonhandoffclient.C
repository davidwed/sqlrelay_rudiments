// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/private/config.h>

#include <rudiments/inetclientsocket.h>
#include <stdio.h>
#include <time.h>

int	main() {

	unsigned short	port;
	char	buffer[11];
	inetclientsocket	*clnt=new inetclientsocket();

	time_t	starttime=time(NULL);

	for (int i=0; i<100000; i++) {
		clnt->connectToServer("localhost",10000,0,1);
		clnt->read(&port);
		clnt->close();

		clnt->connectToServer("localhost",port,0,1);
		clnt->read(buffer,5);
		clnt->close();
	}

	printf("total real time: %d\n",time(NULL)-starttime);

	delete clnt;
}
