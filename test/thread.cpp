// Copyright (c) 2013  David Muse
// See the file COPYING for more information

#include <rudiments/thread.h>
#include <rudiments/stdio.h>
#include <rudiments/snooze.h>

#include <windows.h>

struct args {
	thread		*th;
	uint16_t	id;
	bool		detach;
};

void count(void *args) {
	struct args	*a=(struct args *)args;
	if (a->detach) {
		stdoutput.printf("  %d: detaching\n",a->id);
		if (!a->th->detach()) {
			stdoutput.printf("  %d: detach failed\n",a->id);
		}
	}
	for (uint16_t c=0; c<100; c++) {
		stdoutput.printf("  %d: %d\n",a->id,c);
	}
	if (!a->detach) {
		stdoutput.printf("  %d: exiting\n",a->id);
		a->th->exit(a->id);
	}
}

int main(int argc, const char **argv) {

	// create thread 1
	thread	t1;
	struct args	a1;
	a1.th=&t1;
	a1.id=1;
	a1.detach=false;
	t1.setFunction((void*(*)(void*))count,&a1);

	// create thread 2
	thread	t2;
	struct args	a2;
	a2.th=&t2;
	a2.id=2;
	a2.detach=false;
	t2.setFunction((void*(*)(void*))count,&a2);

	// run threads in attached mode
	stdoutput.printf("attached:\n");
	if (!t1.create()) {
		stdoutput.printf(" 1: create failed\n");
	}
	if (!t2.create()) {
		stdoutput.printf(" 2: create failed\n");
	}

	// join the threads
	int32_t	t1status;
	if (!t1.join(&t1status)) {
		stdoutput.printf(" 1: join failed\n");
	}
	int32_t	t2status;
	if (!t2.join(&t2status)) {
		stdoutput.printf(" 2: join failed\n");
	}
	stdoutput.printf("t1 status: %d\n",t1status);
	stdoutput.printf("t2 status: %d\n",t2status);

	// reset id's and detach mode
	a1.id=3;
	a1.detach=true;
	a2.id=4;
	a2.detach=true;

	// run threads in detached mode
	stdoutput.printf("detached:\n");
	if (!t1.create()) {
		stdoutput.printf(" 1: create failed\n");
	}
	if (!t2.create()) {
		stdoutput.printf(" 2: create failed\n");
	}

	// we'll have to wait a second
	snooze::macrosnooze(1);
	stdoutput.printf("done\n");
}
