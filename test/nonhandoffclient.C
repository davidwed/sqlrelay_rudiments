// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>
#include <stdio.h>
#include <time.h>

int main(int argc, const char **argv) {

	unsigned short	port;
	char	buffer[11];
	inetclientsocket	clnt;

	time_t	starttime=time(NULL);

	for (int i=0; i<100000; i++) {
		clnt.connect("localhost",10000,-1,-1,0,1);
		clnt.read(&port);
		clnt.close();

		clnt.connect("localhost",port,-1,-1,0,1);
		clnt.read(buffer,5);
		clnt.close();
	}

	printf("total real time: %ld\n",time(NULL)-starttime);
}
