// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>
#include <stdio.h>
#include <time.h>

int main(int argv, const char **argc) {

	unsigned short	port;
	char	buffer[11];
	inetclientsocket	clnt;

	time_t	starttime=time(NULL);

	for (int i=0; i<100000; i++) {
		clnt.connectToServer("localhost",10000,0,1);
		clnt.read(&port);
		clnt.close();

		clnt.connectToServer("localhost",port,0,1);
		clnt.read(buffer,5);
		clnt.close();
	}

	printf("total real time: %d\n",time(NULL)-starttime);
}
