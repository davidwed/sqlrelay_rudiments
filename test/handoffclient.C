// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>

#include <stdio.h>

#include <time.h>

int main(int argc, const char **argv) {

	char			buffer[11];
	int			sizeread;
	inetclientsocket	clnt;

	time_t	starttime=time(NULL);

	for (int i=0; i<100000; i++) {
		clnt.connectToServer("localhost",10000,-1,-1,0,1);
		sizeread=clnt.read(buffer,5);
		buffer[sizeread]=(char)NULL;
		//printf("%s\n",buffer);
		clnt.close();
	}

	printf("total real time: %ld\n",time(NULL)-starttime);
}
