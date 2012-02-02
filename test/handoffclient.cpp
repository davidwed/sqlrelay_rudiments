// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetclientsocket.h>
#include <rudiments/snooze.h>
#include <stdio.h>
#include <time.h>

#ifdef RUDIMENTS_NAMESPACE
using namespace rudiments;
#endif

int main(int argc, const char **argv) {

	char			buffer[11];
	int			sizeread;
	inetclientsocket	clnt;

	time_t	starttime=time(NULL);

	for (int i=0; i<100000; i++) {
		int result=clnt.connect("127.0.0.1",10000,-1,-1,0,1);
		if (result!=RESULT_SUCCESS) {
			printf("connect failed\n");
			snooze::macrosnooze(2);
			continue;
		}
		sizeread=clnt.read(buffer,5);
		buffer[sizeread]=(char)NULL;
		printf("%d: %s\n",i,buffer);
		clnt.close();
	}

	printf("total real time: %ld\n",time(NULL)-starttime);
}