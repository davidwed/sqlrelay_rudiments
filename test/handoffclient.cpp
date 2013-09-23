// Copyright (c) 2001  David Muse
// See the file COPYING for more information

#include <rudiments/inetsocketclient.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include <time.h>

int main(int argc, const char **argv) {

	char			buffer[11];
	int			sizeread;
	inetsocketclient	clnt;

	time_t	starttime=time(NULL);

	for (int i=0; i<100000; i++) {
		int result=clnt.connect("127.0.0.1",10000,-1,-1,0,1);
		if (result!=RESULT_SUCCESS) {
			stdoutput.printf("connect failed\n");
			snooze::macrosnooze(2);
			continue;
		}
		sizeread=clnt.read(buffer,5);
		if (sizeread!=5) {
			stdoutput.printf("read failed\n");
			process::exit(0);
		}
		buffer[sizeread]=(char)NULL;
		stdoutput.printf("%d: %s\n",i,buffer);
		clnt.close();
	}

	stdoutput.printf("total real time: %ld\n",time(NULL)-starttime);
}
